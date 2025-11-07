#include "logic/Camera.h"

namespace pacman {

Camera::Camera(int windowWidth, int windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight) {
    calculateScale();
}

void Camera::calculateScale() {
    // World is [-1, 1], so 2 units total
    // Use the smaller dimension to fit the square world
    int minDimension = std::min(windowWidth, windowHeight);
    scale = minDimension / 2.0f;  // Pixels per world unit
}

Position Camera::worldToScreen(const Position& worldPos) const {
    // Transform from [-1, 1] to screen coordinates
    float screenX = (worldPos.x + 1.0f) * scale;
    float screenY = (worldPos.y + 1.0f) * scale;
    return Position(screenX, screenY);
}

Position Camera::screenToWorld(int screenX, int screenY) const {
    float worldX = (screenX / scale) - 1.0f;
    float worldY = (screenY / scale) - 1.0f;
    return Position(worldX, worldY);
}

float Camera::getSpriteSize() const {
    // Size of one grid cell in pixels
    return scale * 0.1f;  // Adjust based on your grid size
}

void Camera::setWindowSize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    calculateScale();
}

} // namespace pacman