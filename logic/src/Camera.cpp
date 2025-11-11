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

    // ✅ Bereken world bounds op basis van aspect ratio
    float mapAspectRatio = static_cast<float>(cols) / static_cast<float>(rows);
    worldWidth = mapAspectRatio;
    worldHeight = 1.0f;

    // Herbereken scale
    calculateScale();

    std::cout << "Camera: Map " << rows << "x" << cols
              << " → World bounds [" << -worldWidth << "," << worldWidth
              << "] x [" << -worldHeight << "," << worldHeight << "]" << std::endl;
}

void Camera::calculateScale() {
    // ✅ World bounds zijn nu [-worldWidth, worldWidth] x [-worldHeight, worldHeight]
    // Totale world grootte: 2*worldWidth x 2*worldHeight

    float scaleX = windowWidth / (2.0f * worldWidth);   // Pixels per world unit in X
    float scaleY = windowHeight / (2.0f * worldHeight); // Pixels per world unit in Y

    // Use the smaller scale to ensure everything fits
    scale = std::min(scaleX, scaleY);

    std::cout << "Camera: Scale = " << scale << " pixels/unit" << std::endl;
}

Position Camera::worldToScreen(const Position& worldPos) const {
    // ✅ Transform from [-worldWidth, worldWidth] x [-worldHeight, worldHeight]
    //    to screen coordinates

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
    // ✅ Gebruik mapRows voor tile grootte
    const float approximateTileSize = 2.0f * worldHeight / mapRows;
    return scale * approximateTileSize * 0.9f;
}

void Camera::setWindowSize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    calculateScale();
}

} // namespace pacman