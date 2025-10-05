cmake_minimum_required(VERSION 3.22)
project(PacManAP)

set(CMAKE_CXX_STANDARD 20)

# BELANGRIJK: SFML 2.5.1, niet 3.0.2!
set(SFML_DIR "SFML/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(PacManAP
        main.cpp
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

target_link_libraries(PacManAP sfml-graphics sfml-window sfml-system)

# Voor Windows: kopieer SFML DLL's naar build directory
if(WIN32)
    file(GLOB SFML_DLLS "${SFML_DIR}/../../bin/*.dll")
    file(COPY ${SFML_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()