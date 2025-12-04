cmake_minimum_required(VERSION 3.22)

# Find SFML
set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Collect all representation sources
set(REPRESENTATION_SOURCES
        # Main
        ${CMAKE_SOURCE_DIR}/main.cpp
        src/Game.cpp
        src/ConcreteFactory.cpp
        include/representation/Animation.h
        src/AnimationController.cpp
        src/SpriteManager.cpp

        # States
        src/states/State.cpp
        src/states/StateManager.cpp
        src/states/MenuState.cpp
        src/states/LevelState.cpp
        src/states/PausedState.cpp
        src/states/VictoryState.cpp

        # Views
        src/views/EntityView.cpp
        src/views/PacManView.cpp
        src/views/CoinView.cpp
        src/views/FruitView.cpp
        src/views/WallView.cpp
        src/views/GhostView.cpp
)

# Create executable
add_executable(PacManAP ${REPRESENTATION_SOURCES})

# Link with logic library and SFML
target_link_libraries(PacManAP PRIVATE
        PacManLogic
        sfml-graphics
        sfml-window
        sfml-system
)

# Include directories
target_include_directories(PacManAP PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Compiler warnings
target_compile_options(PacManAP PRIVATE
        -Wall -Wextra -Wpedantic
)

# Copy SFML DLLs on Windows
if(WIN32)
    file(GLOB SFML_DLLS "${SFML_DIR}/../../bin/*.dll")
    file(COPY ${SFML_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()

# ✅ Maak een custom target die ALTIJD runt
add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        ${CMAKE_BINARY_DIR}/resources
        COMMENT "Copying resources..."
)

# Zorg dat PacManAP afhankelijk is van copy_resources
add_dependencies(PacManAP copy_resources)