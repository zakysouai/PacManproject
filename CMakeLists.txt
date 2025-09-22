cmake_minimum_required(VERSION 3.30)
project(PacManAP)

set(CMAKE_CXX_STANDARD 20)

add_executable(PacManAP main.cpp
        src/Game.cpp
        src/Game.h
        src/Camera.cpp
        src/Camera.h
        src/Random.cpp
        src/Random.h
        src/Score.cpp
        src/Score.h
        src/World.cpp
        src/World.h
)
