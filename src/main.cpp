#include <SFML/Graphics.hpp>
#include <deque>
#include <random>
#include <sstream>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int BLOCK_SIZE = 20;
const int INIT_SNAKE_LENGTH = 3;
const float MOVE_INTERVAL = 0.2f;

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

int getRandomNumber(int min, int max)
{
    static std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

class SnakeGame
{
public:
    SnakeGame()
        : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake Game"),
          direction(RIGHT), moveTimer(0.0f), totalTime(0.0f)
    {
        // if (!font.loadFromFile("Arial.ttf"))
        // {
        //     std::cerr << "Error loading font!" << std::endl;
        //     // Handle error, maybe exit or continue without text
        // }

        // instructions.setFont(font);
        instructions.setString("Use arrow keys to move");
        instructions.setCharacterSize(20);
        instructions.setFillColor(sf::Color::White);
        instructions.setPosition(10, 10);

        // timeText.setFont(font);
        timeText.setCharacterSize(20);
        timeText.setFillColor(sf::Color::White);
        timeText.setPosition(10, 40);

        resetGame();
    }

    void run()
    {
        sf::Clock clock;
        while (window.isOpen())
        {
            sf::Time elapsed = clock.restart();
            handleEvents();
            update(elapsed.asSeconds());
            render();
        }
    }

private:
    sf::RenderWindow window;
    std::deque<sf::RectangleShape> snake;
    sf::RectangleShape food;
    Direction direction;
    float moveTimer;
    float totalTime;
    sf::Font font;
    sf::Text instructions;
    sf::Text timeText;

    void resetGame()
    {
        snake.clear();
        for (int i = 0; i < INIT_SNAKE_LENGTH; ++i)
        {
            sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
            block.setFillColor(sf::Color::Green);
            block.setPosition(BLOCK_SIZE * i, 0);
            snake.push_back(block);
        }
        placeFood();
        direction = RIGHT;
        moveTimer = 0.0f;
        totalTime = 0.0f;
    }

    void placeFood()
    {
        food.setSize(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
        food.setFillColor(sf::Color::Red);
        food.setPosition(
            getRandomNumber(0, (WINDOW_WIDTH / BLOCK_SIZE) - 1) * BLOCK_SIZE,
            getRandomNumber(0, (WINDOW_HEIGHT / BLOCK_SIZE) - 1) * BLOCK_SIZE);
    }

    void handleEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::W:
                case sf::Keyboard::Up:
                    if (direction != DOWN)
                        direction = UP;
                    break;
                case sf::Keyboard::S:
                case sf::Keyboard::Down:
                    if (direction != UP)
                        direction = DOWN;
                    break;
                case sf::Keyboard::A:
                case sf::Keyboard::Left:
                    if (direction != RIGHT)
                        direction = LEFT;
                    break;
                case sf::Keyboard::D:
                case sf::Keyboard::Right:
                    if (direction != LEFT)
                        direction = RIGHT;
                    break;
                case sf::Keyboard::Escape:
                    window.close();
                    break;
                default:
                    // Handle other keys or do nothing
                    break;
                }
            }
        }
    }

    void update(float deltaTime)
    {
        totalTime += deltaTime;
        moveTimer += deltaTime;
        if (moveTimer >= MOVE_INTERVAL)
        {
            moveTimer = 0.0f;
            moveSnake();
        }

        std::stringstream timeStream;
        timeStream << "Time: " << static_cast<int>(totalTime) << " seconds";
        timeText.setString(timeStream.str());
    }

    void moveSnake()
    {
        // Move the body
        for (int i = snake.size() - 1; i > 0; --i)
        {
            snake[i].setPosition(snake[i - 1].getPosition());
        }
        // Move the head
        sf::Vector2f newPos = snake.front().getPosition();
        switch (direction)
        {
        case UP:
            newPos.y -= BLOCK_SIZE;
            break;
        case DOWN:
            newPos.y += BLOCK_SIZE;
            break;
        case LEFT:
            newPos.x -= BLOCK_SIZE;
            break;
        case RIGHT:
            newPos.x += BLOCK_SIZE;
            break;
        }
        snake.front().setPosition(newPos);

        // Check for food collision
        if (snake.front().getGlobalBounds().intersects(food.getGlobalBounds()))
        {
            growSnake();
            placeFood();
        }

        // Check for self collision
        for (int i = 1; i < snake.size(); ++i)
        {
            if (snake.front().getPosition() == snake[i].getPosition())
            {
                resetGame();
                return;
            }
        }

        // Check for wall collision
        if (newPos.x < 0 || newPos.x >= WINDOW_WIDTH || newPos.y < 0 || newPos.y >= WINDOW_HEIGHT)
        {
            resetGame();
        }
    }

    void growSnake()
    {
        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
        block.setFillColor(sf::Color::Green);
        block.setPosition(snake.back().getPosition());
        snake.push_back(block);
    }

    void render()
    {
        window.clear();
        for (const auto &block : snake)
        {
            window.draw(block);
        }
        window.draw(food);
        window.draw(instructions);
        window.draw(timeText);
        window.display();
    }
};

int main()
{
    SnakeGame game;
    game.run();
    return 0;
}
