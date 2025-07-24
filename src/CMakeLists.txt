set(PROJECT_SOURCES
	${SOURCE_DIR}/main.cpp
	${SOURCE_DIR}/GameWindow.cpp
	${SOURCE_DIR}/SnakeGame.cpp
	)

set(PROJECT_HEADERS
    ${CMAKE_SOURCE_DIR}/include/GameWindow.hpp
    ${CMAKE_SOURCE_DIR}/include/SnakeGame.hpp
    )

qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

# Link against Qt6 libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets # Use Qt6::OpenGLWidgets for OpenGL functionality
)

# Include directories for Qt and project headers
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

