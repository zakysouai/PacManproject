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

    spriteSheet.setSmooth(false);

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

// ========================================
// UPDATED SpriteManager::defineAllSprites()
// Replace the existing method in representation/src/SpriteManager.cpp
// ========================================

void SpriteManager::defineAllSprites() {
    // ===== PACMAN SPRITES =====
    // Closed mouth (circle - used for all directions when closed)
    defineSpriteRect("pacman_closed", sf::IntRect(853, 5, 33, 33));

    // RIGHT direction
    defineSpriteRect("pacman_right_closed", sf::IntRect(853, 5, 33, 33));
    defineSpriteRect("pacman_right_half", sf::IntRect(853, 55, 30, 33));    // Half open
    defineSpriteRect("pacman_right_open", sf::IntRect(853, 105, 23, 33));   // Wide open

    // DOWN direction
    defineSpriteRect("pacman_down_closed", sf::IntRect(853, 5, 33, 33));
    defineSpriteRect("pacman_down_half", sf::IntRect(852, 205, 33, 30));    // Half open
    defineSpriteRect("pacman_down_open", sf::IntRect(852, 255, 33, 23));    // Wide open

    // LEFT direction
    defineSpriteRect("pacman_left_closed", sf::IntRect(853, 5, 33, 33));
    defineSpriteRect("pacman_left_half", sf::IntRect(855, 355, 30, 33));    // Half open
    defineSpriteRect("pacman_left_open", sf::IntRect(862, 405, 23, 33));    // Wide open

    // UP direction
    defineSpriteRect("pacman_up_closed", sf::IntRect(853, 5, 33, 33));
    defineSpriteRect("pacman_up_half", sf::IntRect(853, 507, 33, 30));      // Half open
    defineSpriteRect("pacman_up_open", sf::IntRect(853, 564, 33, 23));      // Wide open

    // ===== PACMAN DEATH ANIMATION =====
    defineSpriteRect("pacman_death_0", sf::IntRect(353, 9, 33, 23));
    defineSpriteRect("pacman_death_1", sf::IntRect(351, 62, 37, 20));
    defineSpriteRect("pacman_death_2", sf::IntRect(351, 117, 37, 15));
    defineSpriteRect("pacman_death_3", sf::IntRect(351, 170, 37, 12));
    defineSpriteRect("pacman_death_4", sf::IntRect(351, 222, 37, 13));
    defineSpriteRect("pacman_death_5", sf::IntRect(351, 272, 37, 15));
    defineSpriteRect("pacman_death_6", sf::IntRect(353, 322, 33, 18));
    defineSpriteRect("pacman_death_7", sf::IntRect(358, 372, 23, 18));
    defineSpriteRect("pacman_death_8", sf::IntRect(363, 422, 13, 18));
    defineSpriteRect("pacman_death_9", sf::IntRect(368, 472, 3, 15));
    defineSpriteRect("pacman_death_10", sf::IntRect(356, 517, 27, 25));

    // ===== GHOST RED =====
    defineSpriteRect("ghost_red_right_1", sf::IntRect(1, 4, 35, 35));
    defineSpriteRect("ghost_red_right_2", sf::IntRect(1, 4, 35, 35));  // Same sprite (no animation frames)
    defineSpriteRect("ghost_red_down_1", sf::IntRect(1, 104, 35, 35));
    defineSpriteRect("ghost_red_down_2", sf::IntRect(1, 104, 35, 35));
    defineSpriteRect("ghost_red_left_1", sf::IntRect(1, 204, 35, 35));
    defineSpriteRect("ghost_red_left_2", sf::IntRect(1, 204, 35, 35));
    defineSpriteRect("ghost_red_up_1", sf::IntRect(1, 304, 35, 35));
    defineSpriteRect("ghost_red_up_2", sf::IntRect(1, 304, 35, 35));

    // ===== GHOST PINK =====
    defineSpriteRect("ghost_pink_right_1", sf::IntRect(51, 4, 35, 35));
    defineSpriteRect("ghost_pink_right_2", sf::IntRect(51, 4, 35, 35));
    defineSpriteRect("ghost_pink_down_1", sf::IntRect(51, 104, 35, 35));
    defineSpriteRect("ghost_pink_down_2", sf::IntRect(51, 104, 35, 35));
    defineSpriteRect("ghost_pink_left_1", sf::IntRect(51, 204, 35, 35));
    defineSpriteRect("ghost_pink_left_2", sf::IntRect(51, 204, 35, 35));
    defineSpriteRect("ghost_pink_up_1", sf::IntRect(51, 304, 35, 35));
    defineSpriteRect("ghost_pink_up_2", sf::IntRect(51, 304, 35, 35));

    // ===== GHOST CYAN =====
    defineSpriteRect("ghost_cyan_right_1", sf::IntRect(101, 4, 35, 35));
    defineSpriteRect("ghost_cyan_right_2", sf::IntRect(101, 4, 35, 35));
    defineSpriteRect("ghost_cyan_down_1", sf::IntRect(101, 104, 35, 35));
    defineSpriteRect("ghost_cyan_down_2", sf::IntRect(101, 104, 35, 35));
    defineSpriteRect("ghost_cyan_left_1", sf::IntRect(101, 204, 35, 35));
    defineSpriteRect("ghost_cyan_left_2", sf::IntRect(101, 204, 35, 35));
    defineSpriteRect("ghost_cyan_up_1", sf::IntRect(101, 304, 35, 35));
    defineSpriteRect("ghost_cyan_up_2", sf::IntRect(101, 304, 35, 35));

    // ===== GHOST ORANGE =====
    defineSpriteRect("ghost_orange_right_1", sf::IntRect(151, 4, 35, 35));
    defineSpriteRect("ghost_orange_right_2", sf::IntRect(151, 4, 35, 35));
    defineSpriteRect("ghost_orange_down_1", sf::IntRect(151, 104, 35, 35));
    defineSpriteRect("ghost_orange_down_2", sf::IntRect(151, 104, 35, 35));
    defineSpriteRect("ghost_orange_left_1", sf::IntRect(151, 204, 35, 35));
    defineSpriteRect("ghost_orange_left_2", sf::IntRect(151, 204, 35, 35));
    defineSpriteRect("ghost_orange_up_1", sf::IntRect(151, 304, 35, 35));
    defineSpriteRect("ghost_orange_up_2", sf::IntRect(151, 304, 35, 35));

    // ===== GHOST GREEN =====
    defineSpriteRect("ghost_green_right_1", sf::IntRect(201, 4, 35, 35));
    defineSpriteRect("ghost_green_right_2", sf::IntRect(201, 4, 35, 35));
    defineSpriteRect("ghost_green_down_1", sf::IntRect(201, 104, 35, 35));
    defineSpriteRect("ghost_green_down_2", sf::IntRect(201, 104, 35, 35));
    defineSpriteRect("ghost_green_left_1", sf::IntRect(201, 204, 35, 35));
    defineSpriteRect("ghost_green_left_2", sf::IntRect(201, 204, 35, 35));
    defineSpriteRect("ghost_green_up_1", sf::IntRect(201, 304, 35, 35));
    defineSpriteRect("ghost_green_up_2", sf::IntRect(201, 304, 35, 35));

    // ===== GHOST PURPLE =====
    defineSpriteRect("ghost_purple_right_1", sf::IntRect(251, 4, 35, 35));
    defineSpriteRect("ghost_purple_right_2", sf::IntRect(251, 4, 35, 35));
    defineSpriteRect("ghost_purple_down_1", sf::IntRect(251, 104, 35, 35));
    defineSpriteRect("ghost_purple_down_2", sf::IntRect(251, 104, 35, 35));
    defineSpriteRect("ghost_purple_left_1", sf::IntRect(251, 204, 35, 35));
    defineSpriteRect("ghost_purple_left_2", sf::IntRect(251, 204, 35, 35));
    defineSpriteRect("ghost_purple_up_1", sf::IntRect(251, 304, 35, 35));
    defineSpriteRect("ghost_purple_up_2", sf::IntRect(251, 304, 35, 35));

    // ===== GHOST SCARED (fear mode) =====
    defineSpriteRect("ghost_scared_1", sf::IntRect(1, 554, 35, 35));
    defineSpriteRect("ghost_scared_2", sf::IntRect(51, 554, 35, 35));

    // ===== COLLECTIBLES =====
    defineSpriteRect("coin", sf::IntRect(601, 153, 36, 36));

    // ===== FRUITS =====
    defineSpriteRect("fruit",sf::IntRect(601, 153, 36, 36));
}

void SpriteManager::defineAllAnimations() {
    // ===== PACMAN ANIMATIONS =====
    // Using closed → open → closed for smooth animation

    defineAnimation("pacman_walk_right", Animation{
        {"pacman_right_closed", "pacman_right_half", "pacman_right_open", "pacman_right_half"},
        0.1f,  // 10 FPS
        true   // Loop
    });

    defineAnimation("pacman_walk_left", Animation{
        {"pacman_left_closed", "pacman_left_half", "pacman_left_open", "pacman_left_half"},
        0.1f,
        true
    });

    defineAnimation("pacman_walk_up", Animation{
        {"pacman_up_closed", "pacman_up_half", "pacman_up_open", "pacman_up_half"},
        0.1f,
        true
    });

    defineAnimation("pacman_walk_down", Animation{
        {"pacman_down_closed", "pacman_down_half", "pacman_down_open", "pacman_down_half"},
        0.1f,
        true
    });

    // ===== GHOST ANIMATIONS =====
    // Note: Ghosts have no animation frames in this sprite sheet
    // So we just use the same sprite twice for consistency with existing code

    // Ghost RED
    defineAnimation("ghost_red_walk_right", Animation{
        {"ghost_red_right_1", "ghost_red_right_2"},
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

    defineAnimation("ghost_red_walk_up", Animation{
        {"ghost_red_up_1", "ghost_red_up_2"},
        0.15f,
        true
    });

    // Ghost PINK
    defineAnimation("ghost_pink_walk_right", Animation{
        {"ghost_pink_right_1", "ghost_pink_right_2"},
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

    defineAnimation("ghost_pink_walk_up", Animation{
        {"ghost_pink_up_1", "ghost_pink_up_2"},
        0.15f,
        true
    });

    // Ghost CYAN
    defineAnimation("ghost_cyan_walk_right", Animation{
        {"ghost_cyan_right_1", "ghost_cyan_right_2"},
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

    defineAnimation("ghost_cyan_walk_up", Animation{
        {"ghost_cyan_up_1", "ghost_cyan_up_2"},
        0.15f,
        true
    });

    // Ghost ORANGE
    defineAnimation("ghost_orange_walk_right", Animation{
        {"ghost_orange_right_1", "ghost_orange_right_2"},
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

    defineAnimation("ghost_orange_walk_up", Animation{
        {"ghost_orange_up_1", "ghost_orange_up_2"},
        0.15f,
        true
    });

    // Ghost SCARED mode (same for all ghosts)
    defineAnimation("ghost_scared", Animation{
        {"ghost_scared_1", "ghost_scared_2"},
        0.2f,  // Slower animation
        true
    });

    defineAnimation("pacman_walk_down", Animation{
    {"pacman_down_closed", "pacman_down_half", "pacman_down_open", "pacman_down_half"},
    0.1f,
    true
    });

    // ===== PACMAN DEATH ANIMATION =====
    // Non-looping animation showing PacMan's death
    defineAnimation("pacman_death", Animation{
        {
            "pacman_death_0",
            "pacman_death_1",
            "pacman_death_2",
            "pacman_death_3",
            "pacman_death_4",
            "pacman_death_5",
            "pacman_death_6",
            "pacman_death_7",
            "pacman_death_8",
            "pacman_death_9",
            "pacman_death_10"
        },
        0.1f,  // 10 FPS - adjust for desired speed
        false  // DON'T loop - play once and stay on last frame
    });
}

} // namespace pacman::representation