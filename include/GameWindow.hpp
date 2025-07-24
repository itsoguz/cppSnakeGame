#ifndef GAMEWINDOW_HPP
#define GAMEWINDOW_HPP

#include <QOpenGLWidget>	// Base class for OpenGL rendering
#include <QTimer>           // For game loop updates
#include <QKeyEvent>		// For keyboard input
#include <QOpenGLFunctions>
#include "SnakeGame.hpp"	// Include the game logic header

class GameWindow : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT // Required for Qt's meta-object system (signals/slots)

public:
    explicit GameWindow(QWidget *parent = nullptr); // Constructor
    ~GameWindow() override; // Destructor

protected:
    // OpenGL rendering functions, overridden from QOpenGLWidget
    void initializeGL() override; // Called once to set up OpenGL context
    void resizeGL(int w, int h) override; // Called when widget is resized
    void paintGL() override; // Called to render the scene

    // Event handlers
    void keyPressEvent(QKeyEvent *event) override; // Handles key presses

private slots:
    void updateGame();	// Slot to update game state and trigger repaint

private:
    SnakeGame *m_snakeGame; // Pointer to the game logic instance
    QTimer *m_gameTimer;    // Timer for game updates

    // OpenGL drawing functions
    void drawSnake();
    void drawFood();
    void drawGrid();
    void drawGameOver();
    void drawScore();

    // Helper for setting up projection matrix
    void setupProjection();
};

#endif // GAMEWINDOW_HPP

