#include "logic/Camera.h"
#include <algorithm>
#include <iostream>

namespace pacman {

Camera::Camera(int windowWidth, int windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight) {
    calculateScale();
}

void Camera::setMapDimensions(int rows, int cols) {
    mapRows = rows;
    mapCols = cols;

    float mapAspectRatio = static_cast<float>(cols) / static_cast<float>(rows);
    worldWidth = mapAspectRatio;
    worldHeight = 1.0f;

    calculateScale();

    std::cout << "Camera: Map " << rows << "x" << cols
              << " → World bounds [" << -worldWidth << "," << worldWidth
              << "] x [" << -worldHeight << "," << worldHeight << "]" << std::endl;
}

void Camera::calculateScale() {
    // World size
    float worldPixelWidth = 2.0f * worldWidth;
    float worldPixelHeight = 2.0f * worldHeight;

    // Aspect ratios
    float windowAspect = static_cast<float>(windowWidth) / windowHeight;
    float worldAspect = worldPixelWidth / worldPixelHeight;

    // ✅ MAXIMALE SCHAAL ZONDER VERVORMING
    if (windowAspect > worldAspect) {
        // Window is breder → letterbox links/rechts
        scale = windowHeight / worldPixelHeight;
        viewportWidth = worldPixelWidth * scale;
        viewportHeight = windowHeight;
        viewportOffsetX = (windowWidth - viewportWidth) / 2.0f;
        viewportOffsetY = 0.0f;
    } else {
        // Window is hoger → letterbox boven/onder
        scale = windowWidth / worldPixelWidth;
        viewportWidth = windowWidth;
        viewportHeight = worldPixelHeight * scale;
        viewportOffsetX = 0.0f;
        viewportOffsetY = (windowHeight - viewportHeight) / 2.0f;
    }

    std::cout << "Camera: Scale = " << scale << " pixels/unit" << std::endl;
    std::cout << "Viewport: offset(" << viewportOffsetX << ", " << viewportOffsetY
              << ") size(" << viewportWidth << "x" << viewportHeight << ")" << std::endl;
}

Position Camera::worldToScreen(const Position& worldPos) const {
    // ✅ MAP NAAR VIEWPORT + OFFSET
    float screenX = viewportOffsetX + (viewportWidth / 2.0f) + worldPos.x * scale;
    float screenY = viewportOffsetY + (viewportHeight / 2.0f) + worldPos.y * scale;

    return Position(screenX, screenY);
}

Position Camera::screenToWorld(int screenX, int screenY) const {
    float worldX = (screenX - viewportOffsetX - viewportWidth / 2.0f) / scale;
    float worldY = (screenY - viewportOffsetY - viewportHeight / 2.0f) / scale;
    return Position(worldX, worldY);
}

float Camera::getSpriteSize() const {
    const float approximateTileSize = 2.0f * worldHeight / mapRows;
    return scale * approximateTileSize * 0.9f;
}

void Camera::setWindowSize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    calculateScale();
}

} // namespace pacman