#pragma once
#include "Animation.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

namespace pacman::representation {

/**
 * @brief Singleton manager for all sprites and animations
 * 
 * The SpriteManager is responsible for:
 * - Loading the sprite sheet texture (once)
 * - Storing sprite rectangle definitions (where each sprite is in the sheet)
 * - Storing animation definitions (which sprites make up each animation)
 * - Providing access to all of the above for Views
 * 
 * Usage:
 *   auto& mgr = SpriteManager::getInstance();
 *   
 *   // In initialization:
 *   mgr.loadSpriteSheet("resources/sprites/pacman.png");
 *   mgr.defineSpriteRect("pacman_right_open", sf::IntRect(0, 0, 16, 16));
 *   mgr.defineAnimation("pacman_walk", animation);
 *   
 *   // In Views:
 *   sprite.setTexture(mgr.getTexture());
 *   sprite.setTextureRect(mgr.getSpriteRect("pacman_right_open"));
 *   controller.play(mgr.getAnimation("pacman_walk"));
 */
class SpriteManager {
public:
    /**
     * @brief Get the singleton instance
     * 
     * Uses the Meyer's Singleton pattern (thread-safe in C++11+)
     */
    static SpriteManager& getInstance() {
        static SpriteManager instance;
        return instance;
    }
    
    // Delete copy and move constructors/operators
    SpriteManager(const SpriteManager&) = delete;
    SpriteManager& operator=(const SpriteManager&) = delete;
    SpriteManager(SpriteManager&&) = delete;
    SpriteManager& operator=(SpriteManager&&) = delete;
    
    /**
     * @brief Load the sprite sheet texture from file
     * 
     * This should be called once at game initialization.
     * 
     * @param path Path to the PNG file (e.g., "resources/sprites/pacman.png")
     * @throws std::runtime_error if the file cannot be loaded
     */
    void loadSpriteSheet(const std::string& path);
    
    /**
     * @brief Get the loaded sprite sheet texture
     * 
     * All sprites share this single texture for efficiency.
     * 
     * @return Reference to the sprite sheet texture
     */
    const sf::Texture& getTexture() const { return spriteSheet; }
    
    /**
     * @brief Check if the texture has been loaded
     * 
     * @return true if loadSpriteSheet() has been called successfully
     */
    bool isTextureLoaded() const { return textureLoaded; }
    
    /**
     * @brief Define a sprite rectangle
     * 
     * This tells the manager where a specific sprite is located in the sprite sheet.
     * 
     * @param name Unique name for this sprite (e.g., "pacman_right_open")
     * @param rect Rectangle defining position and size in the sprite sheet
     *             IntRect(left, top, width, height) in pixels
     */
    void defineSpriteRect(const std::string& name, const sf::IntRect& rect);
    
    /**
     * @brief Get a sprite rectangle by name
     * 
     * @param name Name of the sprite
     * @return The rectangle for this sprite
     * @throws std::out_of_range if sprite name not found
     */
    sf::IntRect getSpriteRect(const std::string& name) const;
    
    /**
     * @brief Check if a sprite has been defined
     * 
     * @param name Sprite name to check
     * @return true if the sprite exists
     */
    bool hasSpriteRect(const std::string& name) const;
    
    /**
     * @brief Define an animation
     * 
     * This stores an animation definition that can be used by AnimationControllers.
     * 
     * @param name Unique name for this animation (e.g., "pacman_walk_right")
     * @param animation The animation definition
     */
    void defineAnimation(const std::string& name, const Animation& animation);
    
    /**
     * @brief Get an animation by name
     * 
     * @param name Name of the animation
     * @return Reference to the animation
     * @throws std::out_of_range if animation name not found
     */
    const Animation& getAnimation(const std::string& name) const;
    
    /**
     * @brief Check if an animation has been defined
     * 
     * @param name Animation name to check
     * @return true if the animation exists
     */
    bool hasAnimation(const std::string& name) const;
    
    /**
     * @brief Initialize all sprite rectangles and animations
     * 
     * This is called automatically by the constructor to set up all sprites.
     * Can also be called manually if you want to re-initialize.
     */
    void initialize();

private:
    /**
     * @brief Private constructor (Singleton pattern)
     * 
     * Automatically calls initialize() to set up sprites and animations.
     */
    SpriteManager();
    
    /**
     * @brief Destructor
     */
    ~SpriteManager() = default;
    
    /**
     * @brief Define all sprite rectangles (positions in the sprite sheet)
     * 
     * This is where we'll add all our sprite definitions.
     * Called by initialize().
     */
    void defineAllSprites();
    
    /**
     * @brief Define all animations
     * 
     * This is where we'll add all our animation definitions.
     * Called by initialize().
     */
    void defineAllAnimations();
    
    // Data members
    sf::Texture spriteSheet;                           // The sprite sheet texture
    bool textureLoaded = false;                        // Has the texture been loaded?
    std::map<std::string, sf::IntRect> spriteRects;    // Sprite name → rectangle
    std::map<std::string, Animation> animations;       // Animation name → animation
};

} // namespace pacman::representation