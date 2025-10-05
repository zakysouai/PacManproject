#include "Camera.h"

namespace PacMan {

    Camera::Camera(unsigned int windowWidth, unsigned int windowHeight)
        : m_windowWidth(windowWidth), m_windowHeight(windowHeight) {
    }

    void Camera::setWindowSize(unsigned int windowWidth, unsigned int windowHeight) {
        m_windowWidth = windowWidth;
        m_windowHeight = windowHeight;
    }

    sf::Vector2f Camera::normalizedToScreen(float normalizedX, float normalizedY) const {
        // Convert from [-1, 1] to [0, window_size]
        // Normalized: -1 = left/top, +1 = right/bottom
        float screenX = (normalizedX + 1.0f) * 0.5f * m_windowWidth;
        float screenY = (normalizedY + 1.0f) * 0.5f * m_windowHeight;

        return sf::Vector2f(screenX, screenY);
    }

    sf::Vector2f Camera::screenToNormalized(float screenX, float screenY) const {
        // Convert from [0, window_size] to [-1, 1]
        float normalizedX = (screenX / m_windowWidth) * 2.0f - 1.0f;
        float normalizedY = (screenY / m_windowHeight) * 2.0f - 1.0f;

        return sf::Vector2f(normalizedX, normalizedY);
    }

    sf::Vector2f Camera::getTileSize(int tilesWidth, int tilesHeight) const {
        // Calculate how much screen space each tile should occupy
        // The world spans [-1, 1] (size of 2) in normalized coordinates
        float tileWidth = (2.0f / tilesWidth) * m_windowWidth * 0.5f;
        float tileHeight = (2.0f / tilesHeight) * m_windowHeight * 0.5f;

        return sf::Vector2f(tileWidth, tileHeight);
    }

} // namespace PacMan