#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;
const int BLOCK_SIZE = 20;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

class SnakeGame {
public:
    SnakeGame();
    void run();

private:
    void processInput();
    void update();
    void render();
    void resetGame();
    void spawnFood();
    void checkCollision();

    sf::RenderWindow window;
    Direction dir;
    vector<sf::RectangleShape> snake;
    sf::RectangleShape food;
    int score;
    bool isGameOver;
    sf::Clock clock;
    float speed;
    sf::SoundBuffer eatBuffer;
    sf::Sound eatSound;
};

SnakeGame::SnakeGame() : window(sf::VideoMode(WIDTH, HEIGHT), "Snake Game"), dir(STOP), score(0), isGameOver(false), speed(0.1f) {
    resetGame();
    // Load sound
    if (!eatBuffer.loadFromFile("eat_sound.wav")) {
        // Handle error
    }
 ⬤eatSound.setBuffer(eatBuffer);
}

void SnakeGame::run() {
    while (window.isOpen()) {
        processInput();
        if (!isGameOver) {
            update();
        }
        render();
    }
}

void SnakeGame::processInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Left: if (dir != RIGHT) dir = LEFT; break;
                case sf::Keyboard::Right: if (dir != LEFT) dir = RIGHT; break;
                case sf::Keyboard::Up: if (dir != DOWN) dir = UP; break;
                case sf::Keyboard::Down: if (dir != UP) dir = DOWN; break;
                case sf::Keyboard::Escape: window.close(); break;
            }
        }
    }
}

void SnakeGame::update() {
    if (clock.getElapsedTime().asSeconds() >= speed) {
        clock.restart();

        // Move the snake
        sf::Vector2f headPos = snake.front().getPosition();
        switch (dir) {
            case LEFT: headPos.x -= BLOCK_SIZE; break;
            case RIGHT: headPos.x += BLOCK_SIZE; break;
            case UP: headPos.y -= BLOCK_SIZE; break;
            case DOWN: headPos.y += BLOCK_SIZE; break;
            default: break;
        }

        // Check for food collision
        if (headPos == food.getPosition()) {
            score++;
            eatSound.play();
            spawnFood();
            snake.push_back(snake.back()); // Grow the snake
        }

        // Move the snake body
        for (size_t i = snake.size() - 1; i > 0; --i) {
            snake[i].setPosition(snake[i - 1].getPosition());
        }
        snake.front().setPosition(headPos);

        // Check for collisions
        checkCollision();
    }
}

void SnakeGame::render() {
    window.clear();
    for (const auto& segment : snake) {
        window.draw(segment);
    }
    window.draw(food);
    window.display();
}

void SnakeGame::resetGame() {
    snake.clear();
    snake.push_back(sf::RectangleShape(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE)));
    snake.front().setFillColor(sf::Color::Green);
    snake.front().setPosition(WIDTH / 2, HEIGHT / 2);
    dir = STOP;
    score = 0;
    isGameOver = false;
    spawnFood();
}

void SnakeGame::spawnFood() {
    int x = (rand() % (WIDTH / BLOCK_SIZE)) * BLOCK_SIZE;
    int y = (rand() % (HEIGHT / BLOCK_SIZE)) * BLOCK_SIZE;
    food.setSize(sf::Vector2f(BLOCK_SIZE, BLOCK _SIZE));
    food.setFillColor(sf::Color::Red);
    food.setPosition(x, y);
}

void SnakeGame::checkCollision() {
    // Check wall collisions
    sf::Vector2f headPos = snake.front().getPosition();
    if (headPos.x < 0 || headPos.x >= WIDTH || headPos.y < 0 || headPos.y >= HEIGHT) {
        isGameOver = true;
    }

    // Check self-collision
    for (size_t i = 1; i < snake.size(); ++i) {
        if (headPos == snake[i].getPosition()) {
            isGameOver = true;
        }
    }
}

int main() {
    srand(static_cast<unsigned>(time(0))); // Seed for random food generation
    SnakeGame game;
    game.run();
    return 0;
}
