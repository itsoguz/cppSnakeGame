#include "GameWindow.hpp"
#include <QOpenGLFunctions>	// Provides access to OpenGL functions
#include <QDebug>		// For debugging output

GameWindow::GameWindow(QWidget *parent)
    : QOpenGLWidget(parent),
      m_snakeGame(new SnakeGame()), // Initialize game logic
      m_gameTimer(new QTimer(this)) // Initialize game timer
{
    // Connect the timer's timeout signal to the updateGame slot
    connect(m_gameTimer, &QTimer::timeout, this, &GameWindow::updateGame);
    m_gameTimer->start(100);		// Start the timer, updating every 100ms (10 FPS)
    setFocusPolicy(Qt::StrongFocus);	// Ensure widget can receive key events
}

GameWindow::~GameWindow() {
    delete m_snakeGame; // Clean up game logic object
}

void GameWindow::initializeGL() {
    // Initialize OpenGL functions (required for QOpenGLWidget)
    initializeOpenGLFunctions();

    // Set clear color (background)
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // Enable depth testing (if you were rendering 3D objects)
    // glEnable(GL_DEPTH_TEST);

    qDebug() << "OpenGL initialized.";
}

void GameWindow::resizeGL(int w, int h) {
    // Set the viewport to cover the entire widget
    glViewport(0, 0, w, h);
    setupProjection(); // Recalculate projection when resized
    qDebug() << "Window resized to:" << w << "x" << h;
}

void GameWindow::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffers

    if (m_snakeGame->isGameOver()) {
        drawGameOver();
    } else {
        drawGrid(); // Draw grid lines
        drawSnake(); // Draw the snake
        drawFood();  // Draw the food
    }
    drawScore(); // Always draw the score
}

void GameWindow::keyPressEvent(QKeyEvent *event) {
    // Handle user input to change snake direction
    switch (event->key()) {
        case Qt::Key_Up:
            m_snakeGame->changeDirection(SnakeGame::Direction::Up);
            break;
        case Qt::Key_Down:
            m_snakeGame->changeDirection(SnakeGame::Direction::Down);
            break;
        case Qt::Key_Left:
            m_snakeGame->changeDirection(SnakeGame::Direction::Left);
            break;
        case Qt::Key_Right:
            m_snakeGame->changeDirection(SnakeGame::Direction::Right);
            break;
        case Qt::Key_Space:
            m_snakeGame->togglePause(); // Pause/unpause the game
            break;
        case Qt::Key_R: // Restart game
            if (m_snakeGame->isGameOver()) {
                m_snakeGame->resetGame();
                m_gameTimer->start(100); // Restart timer if it was stopped
            }
            break;
        default:
            QOpenGLWidget::keyPressEvent(event); // Pass unhandled events to base class
            break;
    }
}

void GameWindow::updateGame() {
    if (!m_snakeGame->isPaused() && !m_snakeGame->isGameOver()) {
        m_snakeGame->update(); // Update game logic
        if (m_snakeGame->isGameOver()) {
            m_gameTimer->stop(); // Stop timer if game is over
        }
        update(); // Request a repaint of the widget
    }
}

void GameWindow::setupProjection() {
    // Set up an orthographic projection for 2D rendering
    // This maps game coordinates (0 to GRID_SIZE-1) directly to screen space
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Left, Right, Bottom, Top, Near, Far
    glOrtho(0.0, m_snakeGame->getGridSize(), 0.0, m_snakeGame->getGridSize(), -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GameWindow::drawSnake() {
    glColor3f(0.0f, 1.0f, 0.0f); // Green color for snake

    // Get snake segments from game logic
    const auto& snakeBody = m_snakeGame->getSnakeBody();

    for (const auto& segment : snakeBody) {
        glBegin(GL_QUADS); // Draw a square for each segment
            glVertex2f(segment.x, segment.y);
            glVertex2f(segment.x + 1.0f, segment.y);
            glVertex2f(segment.x + 1.0f, segment.y + 1.0f);
            glVertex2f(segment.x, segment.y + 1.0f);
        glEnd();
    }
}

void GameWindow::drawFood() {
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for food

    // Get food position from game logic
    const auto& foodPos = m_snakeGame->getFoodPosition();

    glBegin(GL_QUADS); // Draw a square for the food
        glVertex2f(foodPos.x, foodPos.y);
        glVertex2f(foodPos.x + 1.0f, foodPos.y);
        glVertex2f(foodPos.x + 1.0f, foodPos.y + 1.0f);
        glVertex2f(foodPos.x, foodPos.y + 1.0f);
    glEnd();
}

void GameWindow::drawGrid() {
    glColor3f(0.5f, 0.5f, 0.5f); // Grey color for grid lines
    glLineWidth(1.0f); // Thin lines

    int gridSize = m_snakeGame->getGridSize();

    glBegin(GL_LINES);
    // Vertical lines
    for (int i = 0; i <= gridSize; ++i) {
        glVertex2f(i, 0.0f);
        glVertex2f(i, (float)gridSize);
    }
    // Horizontal lines
    for (int i = 0; i <= gridSize; ++i) {
        glVertex2f(0.0f, i);
        glVertex2f((float)gridSize, i);
    }
    glEnd();
}

void GameWindow::drawGameOver() {
    // Simple text rendering for Game Over (using OpenGL raster position and bitmap font)
    // For more advanced text, consider using Qt's QPainter on top of OpenGL or a dedicated text rendering library.
    glColor3f(1.0f, 1.0f, 1.0f); // White color for text

    // Position text in the center of the screen (adjust as needed)
    // This is a very basic way to draw text. For better text rendering,
    // you'd typically use QPainter over the OpenGL widget or a texture-based font system.
    glRasterPos2f(m_snakeGame->getGridSize() / 2.0f - 5.0f, m_snakeGame->getGridSize() / 2.0f + 1.0f);
    const char* gameOverText = "GAME OVER!";
    for (const char* p = gameOverText; *p; p++) {
        // glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *p); // Requires GLUT, often not ideal for Qt
    }
    glRasterPos2f(m_snakeGame->getGridSize() / 2.0f - 8.0f, m_snakeGame->getGridSize() / 2.0f - 1.0f);
    const char* restartText = "Press 'R' to Restart";
    for (const char* p = restartText; *p; p++) {
        // glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *p);
    }
    // NOTE: glutBitmapCharacter is part of GLUT, which is not typically linked with Qt projects by default.
    // For a real Qt/OpenGL project, you would draw text using QPainter on top of the QOpenGLWidget,
    // or use a more robust OpenGL text rendering library.
    // This is a placeholder to show where text rendering would go.
    qDebug() << "Game Over!";
}

void GameWindow::drawScore() {
    // Similar to drawGameOver, this is a placeholder.
    // For actual score display, use QPainter or a dedicated text renderer.
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    QString scoreStr = QString("Score: %1").arg(m_snakeGame->getScore());
    // Position score at top-left
    glRasterPos2f(1.0f, m_snakeGame->getGridSize() - 2.0f);
    // Again, using GLUT for simplicity, but not recommended for production Qt apps.
    // For example:
    // QPainter painter(this);
    // painter.setPen(Qt::white);
    // painter.setFont(QFont("Arial", 16));
    // painter.drawText(10, 20, scoreStr);
    // painter.end();
    qDebug() << scoreStr;
}

