cmake_minimum_required(VERSION 3.22)
project(PacManAP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML 2.5.1 (not 3.0.2!)
set(SFML_DIR "SFML/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# ============================================
# LOGIC LIBRARY (No SFML dependencies)
# ============================================
add_library(PacManLogic STATIC
        src/logic/World.cpp
        src/logic/World.h
        src/logic/TileType.h
)

target_include_directories(PacManLogic PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/logic
)

# Logic library should compile WITHOUT SFML
# target_link_libraries(PacManLogic) # No SFML here!

# ============================================
# REPRESENTATION LIBRARY (SFML dependent)
# ============================================
add_library(PacManRepresentation STATIC
        src/representation/Camera.cpp
        src/representation/Camera.h
        src/representation/State.h
        src/representation/StateManager.cpp
        src/representation/StateManager.h
        src/representation/MenuState.cpp
        src/representation/MenuState.h
        src/representation/LevelState.cpp
        src/representation/LevelState.h
)

target_include_directories(PacManRepresentation PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/representation
        ${CMAKE_CURRENT_SOURCE_DIR}/src/logic
)

target_link_libraries(PacManRepresentation PUBLIC
        PacManLogic
        sfml-graphics
        sfml-window
        sfml-system
)

# ============================================
# UTILITIES (Stopwatch, Random)
# ============================================
add_library(PacManUtils STATIC
        src/utils/Stopwatch.cpp
        src/utils/Stopwatch.h
)

target_include_directories(PacManUtils PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
)

# ============================================
# GAME CLASS (Ties everything together)
# ============================================
add_library(PacManGame STATIC
        src/Game.cpp
        src/Game.h
)

target_include_directories(PacManGame PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/representation
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
)

target_link_libraries(PacManGame PUBLIC
        PacManRepresentation
        PacManUtils
        sfml-graphics
        sfml-window
        sfml-system
)

# ============================================
# EXECUTABLE
# ============================================
add_executable(PacManAP main.cpp)

target_link_libraries(PacManAP
        PacManGame
)

# For Windows: copy SFML DLLs to build directory
if(WIN32)
    file(GLOB SFML_DLLS "${SFML_DIR}/../../bin/*.dll")
    file(COPY ${SFML_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()