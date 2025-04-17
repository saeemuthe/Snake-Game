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
 ⬤
