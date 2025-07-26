#include <QApplication>
#include "GameWindow.hpp"           // Include the game window header

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);   // Initialize the Qt application

    GameWindow gameWindow;		// Create an instance of your game window
    gameWindow.setWindowTitle("qtSnake: OpenGL Snake Game");    // Set window title
    gameWindow.resize(800, 600);    // Set initial window size
    gameWindow.show();              // Display the window

    return app.exec();              // Start the Qt event loop
}

