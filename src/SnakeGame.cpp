#include "SnakeGame.hpp"
#include <chrono>	// For seeding random number generator
#include <algorithm>	// For std::find

SnakeGame::SnakeGame() {
    // Seed the random number generator
    m_randomEngine.seed(std::chrono::system_clock::now().time_since_epoch().count());
    resetGame(); // Initialize game state
}

void SnakeGame::resetGame() {
    m_snakeBody.clear();
    // Start snake in the middle, facing right
    m_snakeBody.push_front({GRID_SIZE / 2, GRID_SIZE / 2});
    m_snakeBody.push_front({GRID_SIZE / 2 + 1, GRID_SIZE / 2}); // Head
    m_currentDirection = Direction::Right;
    m_nextDirection = Direction::Right; // Reset buffered direction
    m_score = 0;
    m_gameOver = false;
    m_paused = false;
    generateFood(); // Place initial food
}

void SnakeGame::update() {
    if (m_gameOver || m_paused) {
        return; // Don't update if game is over or paused
    }

    // Apply the buffered direction
    m_currentDirection = m_nextDirection;

    // Calculate new head position based on current direction
    Point newHead = m_snakeBody.front();
    switch (m_currentDirection) {
        case Direction::Up:
            newHead.y++;
            break;
        case Direction::Down:
            newHead.y--;
            break;
        case Direction::Left:
            newHead.x--;
            break;
        case Direction::Right:
            newHead.x++;
            break;
        case Direction::None:
            // Should not happen in normal gameplay after initial move
            break;
    }

    // Add new head to the front of the snake
    m_snakeBody.push_front(newHead);

    // Check for collisions
    if (checkCollision()) {
        m_gameOver = true;
        return; // Stop updating if game is over
    }

    // Check if snake ate food
    if (newHead == m_food) {
        m_score += 10; // Increase score
        generateFood(); // Generate new food
    } else {
        // If no food was eaten, remove the tail segment
        m_snakeBody.pop_back();
    }
}

void SnakeGame::changeDirection(Direction newDirection) {
    // Prevent immediate 180-degree turns
    if (m_currentDirection == Direction::Up && newDirection == Direction::Down) return;
    if (m_currentDirection == Direction::Down && newDirection == Direction::Up) return;
    if (m_currentDirection == Direction::Left && newDirection == Direction::Right) return;
    if (m_currentDirection == Direction::Right && newDirection == Direction::Left) return;

    // Buffer the new direction
    m_nextDirection = newDirection;
}

void SnakeGame::togglePause() {
    m_paused = !m_paused;
}

void SnakeGame::generateFood() {
    std::uniform_int_distribution<int> distribution(0, GRID_SIZE - 1);
    Point potentialFood;
    bool foodPlaced = false;

    while (!foodPlaced) {
        potentialFood.x = distribution(m_randomEngine);
        potentialFood.y = distribution(m_randomEngine);

        // Ensure food does not spawn on the snake's body
        bool onSnake = false;
        for (const auto& segment : m_snakeBody) {
            if (segment == potentialFood) {
                onSnake = true;
                break;
            }
        }
        if (!onSnake) {
            m_food = potentialFood;
            foodPlaced = true;
        }
    }
}

bool SnakeGame::checkCollision() {
    const Point& head = m_snakeBody.front();

    // Wall collision
    if (head.x < 0 || head.x >= GRID_SIZE ||
        head.y < 0 || head.y >= GRID_SIZE) {
        return true;
    }

    // Self-collision (check head against body segments, excluding itself)
    // Start from the second segment (index 1) to avoid checking head against itself
    for (size_t i = 1; i < m_snakeBody.size(); ++i) {
        if (head == m_snakeBody[i]) {
            return true;
        }
    }

    return false;
}

