cmake_minimum_required(VERSION 3.30)
project(PacManAP)

set(CMAKE_CXX_STANDARD 20)

add_executable(PacManAP main.cpp
                        Game.cpp
                        Game.h
                        Camera.cpp
                        Camera.h
                        Random.cpp
                        Random.h
                        Score.cpp
                        Score.h
                        World.cpp
                        World.h)
