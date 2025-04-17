# Snake-Game
To create a graphical snake game in C++ using SFML or SDL, start by setting up your development environment with the necessary libraries. Implement the game loop, handle user input for snake movement, and incorporate sound effects for actions like eating food. For increasing difficulty, adjust the snake's speed or add obstacles as the score increases.

Setting Up the Development Environment
Install SFML or SDL:

For SFML, download from SFML's official website.
For SDL, download from SDL's official website.
Configure Your IDE:

Set up your project in an IDE like Visual Studio or Code::Blocks.
Link the SFML or SDL libraries in your project settings.
Game Structure
Main Components
Game Loop:

Continuously updates the game state, renders graphics, and processes user input.
User Input Handling:

Capture keyboard events to control the snake's direction.
Game State Management:

Track the snake's position, length, and score.


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;
const int BLOCK_SIZE = 20;
Includes: The code includes the necessary SFML headers for graphics and audio, as well as standard libraries for vectors, random number generation, and time.
Constants: WIDTH and HEIGHT define the dimensions of the game window, while BLOCK_SIZE defines the size of each segment of the snake and the food.


2. Direction Enum
cpp
Run
Copy code
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
This enumeration defines the possible directions the snake can move. The STOP state is used when the game starts or when the snake is not moving.
3. SnakeGame Class
The SnakeGame class encapsulates all the game logic and state.

cpp
Run
Copy code
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
Public Methods:

SnakeGame(): Constructor that initializes the game.
run(): Main game loop that processes input, updates the game state, and renders the graphics.
Private Methods:

processInput(): Handles user input for controlling the snake.
update(): Updates the game state, including snake movement and collision detection.
render(): Draws the snake and food on the window.
resetGame(): Resets the game state to start a new game.
spawnFood(): Randomly places food on the screen.
checkCollision(): Checks for collisions with walls and the snake itself.
Member Variables:

window: The SFML window where the game is displayed.
dir: The current direction of the snake.
snake: A vector of sf::RectangleShape representing the segments of the snake.
food: A rectangle representing the food.
score: The player's score.
isGameOver: A boolean indicating if the game is over.
clock: An SFML clock to manage the game speed.
speed: The speed of the snake.
eatBuffer and eatSound: Used for sound effects when the snake eats food.
4. Constructor
cpp
Run
Copy code
SnakeGame::SnakeGame() : window(sf::VideoMode(WIDTH, HEIGHT), "Snake Game"), dir(STOP), score(0), isGameOver(false), speed(0.1f) {
    resetGame();
    // Load sound
    if (!eatBuffer.loadFromFile("eat_sound.wav")) {
        // Handle error
    }
    eatSound.setBuffer(eatBuffer);
}
Initializes the game window and sets the initial state of the game.
Calls resetGame() to set up the initial game state.
Loads the sound effect for eating food.
5. Game Loop
cpp
Run
Copy code
void SnakeGame::run() {
    while (window.isOpen()) {
        processInput();
        if (!isGameOver) {
            update();
        }
        render();
    }
}
The main game loop runs as long as the window is open.
It processes user input, updates the game state if the game is not over, and renders the graphics.
6. Input Handling
cpp
Run
Copy code
void SnakeGame::processInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Left: if (dir != RIGHT) dir = LEFT; break;
                case sf:: Keyboard::Right: if (dir != LEFT) dir = RIGHT; break;
                case sf::Keyboard::Up: if (dir != DOWN) dir = UP; break;
                case sf::Keyboard::Down: if (dir != UP) dir = DOWN; break;
                case sf::Keyboard::Escape: window.close(); break;
            }
        }
    }
}
This method handles user input by checking for key presses.
It updates the direction of the snake based on the arrow keys, ensuring that the snake cannot reverse direction directly.
7. Game Update Logic
cpp
Run
Copy code
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
The update method is called at regular intervals based on the speed of the game.
It moves the snake in the current direction and checks for collisions with food.
If the snake eats food, it grows by adding a new segment.
The body of the snake follows the head by updating each segment's position.
8. Rendering Graphics
cpp
Run
Copy code
void SnakeGame::render() {
    window.clear();
    for (const auto& segment : snake) {
        window.draw(segment);
    }
    window.draw(food);
    window.display();
}
This method clears the window, draws all segments of the snake and the food, and then displays the updated frame.
9. Resetting the Game
cpp
Run
Copy code
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
Resets the game state, clearing the snake and initializing it at the center of the window.
Sets the initial direction to STOP and resets the score.
10. Food Generation
cpp
Run
Copy code
void SnakeGame::spawnFood() {
    int x = (rand() % (WIDTH / BLOCK_SIZE)) * BLOCK_SIZE;
    int y = (rand() % (HEIGHT / BLOCK_SIZE)) * BLOCK_SIZE;
    food.setSize(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    food.setFillColor(sf::Color::Red);
    food.setPosition(x, y);
}
Randomly generates the position of the food within the game window.
Ensures that the food is placed on a grid defined by BLOCK_SIZE.
11. Collision Detection
cpp
Run
Copy code
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
Checks if the snake's head collides with the walls of the window or with its own body.
If a collision is detected, the game is marked as over.
12. Main Function
cpp
Run
Copy code
int main() {
    srand(static_cast<unsigned>(time(0))); // Seed for random food generation
    SnakeGame game;
    game.run();
    return 0;
}
The entry point of the program initializes the random number generator and starts the game loop.

