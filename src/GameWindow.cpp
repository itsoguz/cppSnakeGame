#include "GameWindow.hpp"
#include <QOpenGLFunctions>     // Provides access to OpenGL functions
#include <QDebug>               // For debugging output
#include <QPainter>
#include <QFont>

GameWindow::GameWindow(QWidget* parent)
    : QOpenGLWidget(parent),
      m_snakeGame(new SnakeGame()), // Initialize game logic
      m_gameTimer(new QTimer(this)) // Initialize game timer
{
    // Connect the timer's timeout signal to the updateGame slot
    connect(m_gameTimer, &QTimer::timeout, this, &GameWindow::updateGame);
    m_gameTimer->start(100);            // Start the timer, updating every 100ms (10 FPS)
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

    // Enable depth testing (used if we want to render 3D objects)
    // glEnable(GL_DEPTH_TEST);

    qDebug() << "OpenGL initialized.";  // In QT Creator: qDebug() sends the string message to "3 Application Output" tab on bottom
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
        drawGrid();
        drawSnake();
        drawFood();
    }
    drawScore();        // Always draw the score
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
            m_snakeGame->togglePause();     // Pause/unpause the game
            break;
        case Qt::Key_R: // Restart game
            if (m_snakeGame->isGameOver()) {
                m_snakeGame->resetGame();
                m_gameTimer->start(100);    // Restart timer if it was stopped
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
        glVertex2f(i, static_cast<float>(gridSize));
    }
    // Horizontal lines
    for (int i = 0; i <= gridSize; ++i) {
        glVertex2f(0.0f, i);
        glVertex2f(static_cast<float>(gridSize), i);
    }
    glEnd();
}

void GameWindow::drawGameOver() {
    QPainter painter(this);
    painter.setPen(Qt::white);

    QFont gameOverFont("Arial", 36, QFont::Bold);
    painter.setFont(gameOverFont);
    QString gameOverText = "GAME OVER!";

    // Calculate text bounding rectangle to center it
    QFontMetrics fm(gameOverFont);
    int textWidth = fm.horizontalAdvance(gameOverText);
    int textHeight = fm.height();
    int x = (width() - textWidth) / 2;      // Center horizontally
    int y = (height() - textHeight) / 2;    // Center vertically (approx)
    painter.drawText(x, y, gameOverText);

    qDebug() << "Game Over!";               // Print to console output

    // Draw "Press 'R' to Restart"
    QFont restartFont("Arial", 18);
    painter.setFont(restartFont);
    QString restartText = "Press 'R' to Restart";
    textWidth = fm.horizontalAdvance(restartText);  // Recalculate for new font
    x = (width() - textWidth) / 2 + 120;            // Added 120px from left to center the text
    // Position below "GAME OVER!" message
    y += textHeight + 20;                           // Move down by text height + some padding
    painter.drawText(x, y, restartText);

    painter.end();
    qDebug() << "Press 'R' to Restart.";
}

void GameWindow::drawScore() {
    QPainter painter(this);                 // Create a QPainter instance, drawing on 'this' widget
    painter.setPen(Qt::white);              // Set the text color to white
    painter.setFont(QFont("Arial", 16));    // Set the font and size

    QString scoreStr = QString("Score: %1").arg(m_snakeGame->getScore());

    painter.drawText(10, 18, scoreStr);     // Display the score: 10 pixels from left, 18 pixels from top
    qDebug() << scoreStr;                   // Output scoreStr to console
}
