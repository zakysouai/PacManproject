cmake_minimum_required(VERSION 3.22)

# Collect all source files
set(LOGIC_SOURCES
        src/World.cpp
        src/EntityModel.cpp
        src/Score.cpp
        src/Camera.cpp
        src/entities/PacMan.cpp
        src/entities/Coin.cpp
        src/entities/Fruit.cpp
        src/entities/Wall.cpp
        src/patterns/Observer.cpp
        src/patterns/Subject.cpp
        src/utils/Stopwatch.cpp
        src/utils/Random.cpp
        src/entities/Ghost.cpp
)

# Create static library (NO SFML DEPENDENCY!)
add_library(PacManLogic STATIC ${LOGIC_SOURCES})

# Make include directory public
target_include_directories(PacManLogic PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Compiler warnings
target_compile_options(PacManLogic PRIVATE
        -Wall -Wextra -Wpedantic
)