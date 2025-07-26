#ifndef SNAKEGAME_HPP
#define SNAKEGAME_HPP

#include <deque>        // For efficient adding/removing from snake body
//#include <utility>	// If we use std::pair for Point(x, y) struct
#include <random>       // For random food generation

// Define a simple point structure for coordinates
struct Point {
    int x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

class SnakeGame {
public:
    // Enum for snake direction
    enum class Direction {
        Up, Down, Left, Right, None
    };

    static const int GRID_SIZE = 30;	// Define the size of the game grid

    SnakeGame(); // Constructor

    void update();	// Main game update logic (move snake, check collisions, etc.)
    void changeDirection(Direction newDirection);	// Change snake's direction
    void togglePause();	// Pause/unpause the game
    void resetGame();	// Reset game to initial state

    // Getters for rendering
    const std::deque<Point>& getSnakeBody() const { return m_snakeBody; }
    const Point& getFoodPosition() const { return m_food; }
    int getScore() const { return m_score; }
    bool isGameOver() const { return m_gameOver; }
    bool isPaused() const { return m_paused; }
    int getGridSize() const { return GRID_SIZE; }

private:
    std::deque<Point> m_snakeBody; // Stores the segments of the snake
    Point m_food;                  // Position of the food
    Direction m_currentDirection;  // Current direction of the snake
    Direction m_nextDirection;     // Stores the next desired direction (for input buffering)
    int m_score;                   // Player's score
    bool m_gameOver;               // Game over flag
    bool m_paused;                 // Game paused flag

    std::mt19937 m_randomEngine;   // Random number generator engine

    void generateFood();	// Generates new food at a random valid position
    bool checkCollision();	// Checks for collisions (wall or self)
};

#endif // SNAKEGAME_HPP

