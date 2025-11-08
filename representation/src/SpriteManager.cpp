#include "representation/SpriteManager.h"
#include <iostream>
#include <stdexcept>

namespace pacman::representation {

SpriteManager::SpriteManager() {
    std::cout << "SpriteManager initialized" << std::endl;
}

void SpriteManager::loadSpriteSheet(const std::string& path) {
    if (textureLoaded) {
        std::cout << "Warning: Sprite sheet already loaded, reloading..." << std::endl;
    }

    if (!spriteSheet.loadFromFile(path)) {
        throw std::runtime_error("Failed to load sprite sheet: " + path);
    }

    textureLoaded = true;
    std::cout << "Sprite sheet loaded: " << path << std::endl;
    std::cout << "Texture size: " << spriteSheet.getSize().x << "x"
              << spriteSheet.getSize().y << " pixels" << std::endl;
}

void SpriteManager::defineSpriteRect(const std::string& name, const sf::IntRect& rect) {
    if (spriteRects.count(name) > 0) {
        std::cout << "Warning: Sprite '" << name << "' already defined, overwriting" << std::endl;
    }

    spriteRects[name] = rect;
}

sf::IntRect SpriteManager::getSpriteRect(const std::string& name) const {
    auto it = spriteRects.find(name);
    if (it == spriteRects.end()) {
        throw std::out_of_range("Sprite not found: " + name);
    }

    return it->second;
}

bool SpriteManager::hasSpriteRect(const std::string& name) const {
    return spriteRects.count(name) > 0;
}

void SpriteManager::defineAnimation(const std::string& name, const Animation& animation) {
    if (!animation.isValid()) {
        std::cerr << "Warning: Attempting to define invalid animation '" << name << "'" << std::endl;
        return;
    }

    if (animations.count(name) > 0) {
        std::cout << "Warning: Animation '" << name << "' already defined, overwriting" << std::endl;
    }

    animations[name] = animation;
}

const Animation& SpriteManager::getAnimation(const std::string& name) const {
    auto it = animations.find(name);
    if (it == animations.end()) {
        throw std::out_of_range("Animation not found: " + name);
    }

    return it->second;
}

bool SpriteManager::hasAnimation(const std::string& name) const {
    return animations.count(name) > 0;
}

void SpriteManager::initialize() {
    std::cout << "Initializing sprites and animations..." << std::endl;

    defineAllSprites();
    defineAllAnimations();

    std::cout << "Defined " << spriteRects.size() << " sprites" << std::endl;
    std::cout << "Defined " << animations.size() << " animations" << std::endl;
}

void SpriteManager::defineAllSprites() {
    // SPRITE SHEET LAYOUT (sprite.png - 936x939 pixels)
    // - Left side: Ghosts in 6 colors × 8 rows
    // - Middle: Fruits, dots, power pellets
    // - Right side: PacMan sprites (yellow and gray variants)

    // Sprite size is approximately 14-16 pixels
    // Using 14×14 as base size with some 16×16 for larger sprites

    // ===== PACMAN SPRITES (Right side - columns 20-22) =====
    // Yellow PacMan facing RIGHT
    defineSpriteRect("pacman_right_closed", sf::IntRect(868, 14, 14, 14));   // Full circle
    defineSpriteRect("pacman_right_open", sf::IntRect(868, 28, 14, 14));      // Mouth open

    // Yellow PacMan facing LEFT
    defineSpriteRect("pacman_left_closed", sf::IntRect(868, 42, 14, 14));
    defineSpriteRect("pacman_left_open", sf::IntRect(868, 56, 14, 14));

    // Yellow PacMan facing UP
    defineSpriteRect("pacman_up_closed", sf::IntRect(868, 70, 14, 14));
    defineSpriteRect("pacman_up_open", sf::IntRect(868, 84, 14, 14));

    // Yellow PacMan facing DOWN
    defineSpriteRect("pacman_down_closed", sf::IntRect(868, 98, 14, 14));
    defineSpriteRect("pacman_down_open", sf::IntRect(868, 112, 14, 14));

    // ===== GHOST SPRITES (Left side - 6 colors) =====
    // Ghost RED (column 0)
    defineSpriteRect("ghost_red_up_1", sf::IntRect(0, 0, 14, 14));
    defineSpriteRect("ghost_red_up_2", sf::IntRect(0, 14, 14, 14));
    defineSpriteRect("ghost_red_down_1", sf::IntRect(0, 28, 14, 14));
    defineSpriteRect("ghost_red_down_2", sf::IntRect(0, 42, 14, 14));
    defineSpriteRect("ghost_red_left_1", sf::IntRect(0, 56, 14, 14));
    defineSpriteRect("ghost_red_left_2", sf::IntRect(0, 70, 14, 14));
    defineSpriteRect("ghost_red_right_1", sf::IntRect(0, 84, 14, 14));
    defineSpriteRect("ghost_red_right_2", sf::IntRect(0, 98, 14, 14));

    // Ghost PINK (column 1)
    defineSpriteRect("ghost_pink_up_1", sf::IntRect(16, 0, 14, 14));
    defineSpriteRect("ghost_pink_up_2", sf::IntRect(16, 14, 14, 14));
    defineSpriteRect("ghost_pink_down_1", sf::IntRect(16, 28, 14, 14));
    defineSpriteRect("ghost_pink_down_2", sf::IntRect(16, 42, 14, 14));
    defineSpriteRect("ghost_pink_left_1", sf::IntRect(16, 56, 14, 14));
    defineSpriteRect("ghost_pink_left_2", sf::IntRect(16, 70, 14, 14));
    defineSpriteRect("ghost_pink_right_1", sf::IntRect(16, 84, 14, 14));
    defineSpriteRect("ghost_pink_right_2", sf::IntRect(16, 98, 14, 14));

    // Ghost CYAN (column 2)
    defineSpriteRect("ghost_cyan_up_1", sf::IntRect(32, 0, 14, 14));
    defineSpriteRect("ghost_cyan_up_2", sf::IntRect(32, 14, 14, 14));
    defineSpriteRect("ghost_cyan_down_1", sf::IntRect(32, 28, 14, 14));
    defineSpriteRect("ghost_cyan_down_2", sf::IntRect(32, 42, 14, 14));
    defineSpriteRect("ghost_cyan_left_1", sf::IntRect(32, 56, 14, 14));
    defineSpriteRect("ghost_cyan_left_2", sf::IntRect(32, 70, 14, 14));
    defineSpriteRect("ghost_cyan_right_1", sf::IntRect(32, 84, 14, 14));
    defineSpriteRect("ghost_cyan_right_2", sf::IntRect(32, 98, 14, 14));

    // Ghost ORANGE (column 3)
    defineSpriteRect("ghost_orange_up_1", sf::IntRect(48, 0, 14, 14));
    defineSpriteRect("ghost_orange_up_2", sf::IntRect(48, 14, 14, 14));
    defineSpriteRect("ghost_orange_down_1", sf::IntRect(48, 28, 14, 14));
    defineSpriteRect("ghost_orange_down_2", sf::IntRect(48, 42, 14, 14));
    defineSpriteRect("ghost_orange_left_1", sf::IntRect(48, 56, 14, 14));
    defineSpriteRect("ghost_orange_left_2", sf::IntRect(48, 70, 14, 14));
    defineSpriteRect("ghost_orange_right_1", sf::IntRect(48, 84, 14, 14));
    defineSpriteRect("ghost_orange_right_2", sf::IntRect(48, 98, 14, 14));

    // Ghost SCARED (gray/blue - middle area)
    defineSpriteRect("ghost_scared_1", sf::IntRect(352, 14, 14, 14));
    defineSpriteRect("ghost_scared_2", sf::IntRect(352, 28, 14, 14));

    // ===== COLLECTIBLES =====
    // Dot/Coin (small white dot)
    defineSpriteRect("coin", sf::IntRect(304, 98, 8, 8));

    // Fruits (middle-right area)
    defineSpriteRect("fruit_cherry", sf::IntRect(560, 28, 14, 14));
    defineSpriteRect("fruit_strawberry", sf::IntRect(560, 42, 14, 14));
    defineSpriteRect("fruit_orange", sf::IntRect(560, 56, 14, 14));
    defineSpriteRect("fruit_apple", sf::IntRect(560, 70, 14, 14));
}

void SpriteManager::defineAllAnimations() {
    // ===== PACMAN ANIMATIONS =====

    // Walking RIGHT
    defineAnimation("pacman_walk_right", Animation{
        {"pacman_right_closed", "pacman_right_open"},
        0.1f,  // 10 FPS
        true   // Loop
    });

    // Walking LEFT
    defineAnimation("pacman_walk_left", Animation{
        {"pacman_left_closed", "pacman_left_open"},
        0.1f,
        true
    });

    // Walking UP
    defineAnimation("pacman_walk_up", Animation{
        {"pacman_up_closed", "pacman_up_open"},
        0.1f,
        true
    });

    // Walking DOWN
    defineAnimation("pacman_walk_down", Animation{
        {"pacman_down_closed", "pacman_down_open"},
        0.1f,
        true
    });

    // ===== GHOST ANIMATIONS =====

    // Ghost RED
    defineAnimation("ghost_red_walk_up", Animation{
        {"ghost_red_up_1", "ghost_red_up_2"},
        0.15f,
        true
    });

    defineAnimation("ghost_red_walk_down", Animation{
        {"ghost_red_down_1", "ghost_red_down_2"},
        0.15f,
        true
    });

    defineAnimation("ghost_red_walk_left", Animation{
        {"ghost_red_left_1", "ghost_red_left_2"},
        0.15f,
        true
    });

    defineAnimation("ghost_red_walk_right", Animation{
        {"ghost_red_right_1", "ghost_red_right_2"},
        0.15f,
        true
    });

    // Ghost PINK
    defineAnimation("ghost_pink_walk_up", Animation{
        {"ghost_pink_up_1", "ghost_pink_up_2"},
        0.15f,
        true
    });

    defineAnimation("ghost_pink_walk_down", Animation{
        {"ghost_pink_down_1", "ghost_pink_down_2"},
        0.15f,
        true
    });

    defineAnimation("ghost_pink_walk_left", Animation{
        {"ghost_pink_left_1", "ghost_pink_left_2"},
        0.15f,
        true
    });

    defineAnimation("ghost_pink_walk_right", Animation{
        {"ghost_pink_right_1", "ghost_pink_right_2"},
        0.15f,
        true
    });

    // Ghost CYAN
    defineAnimation("ghost_cyan_walk_up", Animation{
        {"ghost_cyan_up_1", "ghost_cyan_up_2"},
        0.15f,
        true
    });

    defineAnimation("ghost_cyan_walk_down", Animation{
        {"ghost_cyan_down_1", "ghost_cyan_down_2"},
        0.15f,
        true
    });

    defineAnimation("ghost_cyan_walk_left", Animation{
        {"ghost_cyan_left_1", "ghost_cyan_left_2"},
        0.15f,
        true
    });

    defineAnimation("ghost_cyan_walk_right", Animation{
        {"ghost_cyan_right_1", "ghost_cyan_right_2"},
        0.15f,
        true
    });

    // Ghost ORANGE
    defineAnimation("ghost_orange_walk_up", Animation{
        {"ghost_orange_up_1", "ghost_orange_up_2"},
        0.15f,
        true
    });

    defineAnimation("ghost_orange_walk_down", Animation{
        {"ghost_orange_down_1", "ghost_orange_down_2"},
        0.15f,
        true
    });

    defineAnimation("ghost_orange_walk_left", Animation{
        {"ghost_orange_left_1", "ghost_orange_left_2"},
        0.15f,
        true
    });

    defineAnimation("ghost_orange_walk_right", Animation{
        {"ghost_orange_right_1", "ghost_orange_right_2"},
        0.15f,
        true
    });

    // Ghost SCARED mode (same for all ghosts)
    defineAnimation("ghost_scared", Animation{
        {"ghost_scared_1", "ghost_scared_2"},
        0.2f,  // Slower animation
        true
    });
}

} // namespace pacman::representation