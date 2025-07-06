// main.cpp
// A classic Snake game built with C++ and SFML 3.0
//
// Key Improvements:
// - Migrated to SFML 3.0 syntax.
// - Refactored into a Game class (Object-Oriented Programming).
// - Replaced C-style arrays with std::vector for safety and flexibility.
// - Added proper collision detection (walls and self).
// - Implemented a Game Over and Restart state.
// - Cleaned up the game loop for stability.
// - Added comprehensive comments.

#include <SFML/Graphics.hpp>
#include <vector>
#include <list> // Using list can be slightly more efficient for snake body manipulation
#include <cstdlib>
#include <ctime>
#include <iostream>

// --- Game Configuration Constants ---
// Use constexpr for values known at compile-time.
constexpr int GRID_SIZE = 30;
constexpr int GRID_WIDTH = 25;
constexpr int GRID_HEIGHT = 20;
constexpr int SCREEN_WIDTH = GRID_WIDTH * GRID_SIZE;
constexpr int SCREEN_HEIGHT = GRID_HEIGHT * GRID_SIZE;

// --- Enum for Type-Safe Directions ---
// An enum class is safer than using plain integers.
enum class Direction { Up, Down, Left, Right };

class Game {
public:
    // Constructor: Initializes the game window and resets the game state.
    Game() : m_window({SCREEN_WIDTH, SCREEN_HEIGHT}, "RLSnake | SFML 3.0", sf::Style::Titlebar | sf::Style::Close) {
        // Load a font for displaying text.
        // A "sansation.ttf" font file is expected in the same directory.
        // You can download it from the SFML website or use any other .ttf font.
        if (!m_font.loadFromFile("sansation.ttf")) {
            // If the font fails to load, the game can't run.
            std::cerr << "Error: Could not load font 'sansation.ttf'. Make sure it's in the same directory as the executable." << std::endl;
            // A more robust solution might be to exit or draw without text.
        }

        // Configure the score text
        m_scoreText.setFont(m_font);
        m_scoreText.setCharacterSize(24);
        m_scoreText.setFillColor(sf::Color::White);
        m_scoreText.setPosition(10.f, 10.f);

        // Configure the "Game Over" text
        m_gameOverText.setFont(m_font);
        m_gameOverText.setString("Game Over!\nPress 'R' to Restart");
        m_gameOverText.setCharacterSize(50);
        m_gameOverText.setFillColor(sf::Color::Red);
        m_gameOverText.setStyle(sf::Text::Bold);
        // Center the game over text
        sf::FloatRect textRect = m_gameOverText.getLocalBounds();
        m_gameOverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        m_gameOverText.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);

        resetGame();
    }

    // Main game loop
    void run() {
        sf::Clock clock;
        sf::Time timeSinceLastUpdate = sf::Time::Zero;
        const sf::Time timePerFrame = sf::seconds(0.1f); // Snake moves every 0.1 seconds

        while (m_window.isOpen()) {
            timeSinceLastUpdate += clock.restart();

            processEvents();

            // Use a fixed timestep for consistent game speed
            while (timeSinceLastUpdate > timePerFrame) {
                timeSinceLastUpdate -= timePerFrame;
                if (!m_isGameOver) {
                    update();
                }
            }
            render();
        }
    }

private:
    // --- Member Variables ---
    sf::RenderWindow m_window;
    std::list<sf::Vector2i> m_snake; // A list is efficient for adding to the front (head) and removing from the back (tail)
    sf::Vector2i m_applePosition;
    Direction m_direction;
    bool m_isGameOver;
    int m_score;

    sf::Font m_font;
    sf::Text m_scoreText;
    sf::Text m_gameOverText;

    // --- Game Logic Methods ---

    // Handles user input (closing window, changing direction, restarting)
    void processEvents() {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::EventType::Closed) {
                m_window.close();
            }

            // Handle keyboard input for changing direction
            if (event.type == sf::Event::EventType::KeyPressed) {
                if (m_isGameOver && event.key.code == sf::Keyboard::Key::R) {
                    resetGame();
                } else {
                    handlePlayerInput(event.key.code);
                }
            }
        }
    }

    // Updates direction based on key presses, preventing 180-degree turns
    void handlePlayerInput(sf::Keyboard::Key key) {
        if (key == sf::Keyboard::Key::Up || key == sf::Keyboard::Key::W) {
            if (m_direction != Direction::Down) m_direction = Direction::Up;
        } else if (key == sf::Keyboard::Key::Down || key == sf::Keyboard::Key::S) {
            if (m_direction != Direction::Up) m_direction = Direction::Down;
        } else if (key == sf::Keyboard::Key::Left || key == sf::Keyboard::Key::A) {
            if (m_direction != Direction::Right) m_direction = Direction::Left;
        } else if (key == sf::Keyboard::Key::Right || key == sf::Keyboard::Key::D) {
            if (m_direction != Direction::Left) m_direction = Direction::Right;
        }
    }

    // Updates the game state (moves snake, checks for collisions)
    void update() {
        sf::Vector2i newHeadPosition = m_snake.front();

        // Move the head in the current direction
        switch (m_direction) {
            case Direction::Up:    newHeadPosition.y -= 1; break;
            case Direction::Down:  newHeadPosition.y += 1; break;
            case Direction::Left:  newHeadPosition.x -= 1; break;
            case Direction::Right: newHeadPosition.x += 1; break;
        }

        // --- Collision Detection ---
        // 1. Wall Collision
        if (newHeadPosition.x < 0 || newHeadPosition.x >= GRID_WIDTH ||
            newHeadPosition.y < 0 || newHeadPosition.y >= GRID_HEIGHT) {
            m_isGameOver = true;
            return;
        }

        // 2. Self Collision
        // Check if the new head position collides with any part of the body
        for (auto it = std::next(m_snake.begin()); it != m_snake.end(); ++it) {
            if (*it == newHeadPosition) {
                m_isGameOver = true;
                return;
            }
        }

        // Add the new head
        m_snake.push_front(newHeadPosition);

        // --- Apple Collision ---
        if (newHeadPosition == m_applePosition) {
            // Grow the snake (don't remove tail), increase score, and spawn a new apple
            m_score++;
            m_scoreText.setString("Score: " + std::to_string(m_score));
            spawnApple();
        } else {
            // If no apple was eaten, remove the tail segment to simulate movement
            m_snake.pop_back();
        }
    }

    // Draws all game elements to the window
    void render() {
        m_window.clear(sf::Color::Black); // Clear the screen with a black background

        // Draw snake and apple using a single shape for efficiency
        sf::RectangleShape segment({GRID_SIZE - 1, GRID_SIZE - 1}); // -1 for a grid effect

        // Draw the snake
        for (const auto& pos : m_snake) {
            segment.setPosition(pos.x * GRID_SIZE, pos.y * GRID_SIZE);
            segment.setFillColor(sf::Color::Green);
            m_window.draw(segment);
        }

        // Draw the apple
        segment.setPosition(m_applePosition.x * GRID_SIZE, m_applePosition.y * GRID_SIZE);
        segment.setFillColor(sf::Color::Red);
        m_window.draw(segment);
        
        // Draw the score
        m_window.draw(m_scoreText);

        // If the game is over, draw the game over message
        if (m_isGameOver) {
            m_window.draw(m_gameOverText);
        }

        m_window.display(); // Show the new frame
    }

    // --- Helper Methods ---

    // Spawns an apple in a random location, avoiding the snake's body
    void spawnApple() {
        bool onSnake;
        do {
            onSnake = false;
            m_applePosition.x = rand() % GRID_WIDTH;
            m_applePosition.y = rand() % GRID_HEIGHT;

            // Check if the apple spawned on the snake
            for (const auto& segment : m_snake) {
                if (segment == m_applePosition) {
                    onSnake = true;
                    break;
                }
            }
        } while (onSnake);
    }

    // Resets the game to its initial state
    void resetGame() {
        m_snake.clear();
        m_snake.push_front({GRID_WIDTH / 2, GRID_HEIGHT / 2}); // Start in the middle
        m_direction = Direction::Right;
        m_isGameOver = false;
        m_score = 0;
        m_scoreText.setString("Score: 0");
        spawnApple();
    }
};

// --- Main Function ---
int main() {
    // Seed the random number generator once at the start
    srand(static_cast<unsigned int>(time(nullptr)));

    Game game;
    game.run();

    return 0;
}
