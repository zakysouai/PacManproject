#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>

namespace PacMan {

/**
 * @brief Camera class for projecting normalized coordinates to screen pixels
 * 
 * Converts between the game's normalized coordinate system [-1, 1] 
 * and screen pixel coordinates.
 */
class Camera {
public:
    /**
     * @brief Construct a Camera with window dimensions
     * @param windowWidth Width of the render window in pixels
     * @param windowHeight Height of the render window in pixels
     */
    Camera(unsigned int windowWidth, unsigned int windowHeight);
    ~Camera() = default;

    /**
     * @brief Update camera dimensions (e.g., when window is resized)
     * @param windowWidth New window width
     * @param windowHeight New window height
     */
    void setWindowSize(unsigned int windowWidth, unsigned int windowHeight);

    /**
     * @brief Convert normalized coordinates to screen pixels
     * @param normalizedX X coordinate in range [-1, 1]
     * @param normalizedY Y coordinate in range [-1, 1]
     * @return Screen position in pixels
     */
    sf::Vector2f normalizedToScreen(float normalizedX, float normalizedY) const;

    /**
     * @brief Convert screen pixels to normalized coordinates
     * @param screenX X coordinate in pixels
     * @param screenY Y coordinate in pixels
     * @return Normalized position in range [-1, 1]
     */
    sf::Vector2f screenToNormalized(float screenX, float screenY) const;

    /**
     * @brief Calculate the size of a tile in pixels
     * @param tilesWidth Number of tiles in width
     * @param tilesHeight Number of tiles in height
     * @return Size of one tile in pixels
     */
    sf::Vector2f getTileSize(int tilesWidth, int tilesHeight) const;

    /**
     * @brief Get the width of the viewing area
     * @return Width in pixels
     */
    unsigned int getWidth() const { return m_windowWidth; }

    /**
     * @brief Get the height of the viewing area
     * @return Height in pixels
     */
    unsigned int getHeight() const { return m_windowHeight; }

private:
    unsigned int m_windowWidth;
    unsigned int m_windowHeight;
};

} // namespace PacMan

#endif // CAMERA_H