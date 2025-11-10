#include "logic/Camera.h"

namespace pacman {

Camera::Camera(int windowWidth, int windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight) {
    calculateScale();
}

void Camera::calculateScale() {
    // World is [-1, 1], so 2 units total
    // We want to use the full window while maintaining aspect ratio

    // Calculate scale based on both dimensions
    float scaleX = windowWidth / 2.0f;   // Pixels per world unit in X
    float scaleY = windowHeight / 2.0f;  // Pixels per world unit in Y

    // Use the smaller scale to ensure everything fits
    scale = std::min(scaleX, scaleY);
}

Position Camera::worldToScreen(const Position& worldPos) const {
    // Transform from [-1, 1] to screen coordinates
    // Center the content in the window

    float screenX = windowWidth / 2.0f + worldPos.x * scale;
    float screenY = windowHeight / 2.0f + worldPos.y * scale;

    return Position(screenX, screenY);
}

Position Camera::screenToWorld(int screenX, int screenY) const {
    float worldX = (screenX - windowWidth / 2.0f) / scale;
    float worldY = (screenY - windowHeight / 2.0f) / scale;
    return Position(worldX, worldY);
}

float Camera::getSpriteSize() const {
    // For a typical map (e.g., 20x11), each tile is about 2.0/11 = 0.18 world units
    // We use height because that's what we normalized to [-1, 1]
    // Assuming approximately 11 rows (typical Pac-Man maze)
    const float approximateTileSize = 2.0f / 11.0f;  // ~0.182 world units per tile

    return scale * approximateTileSize * 0.9;
}

void Camera::setWindowSize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    calculateScale();
}

} // namespace pacman