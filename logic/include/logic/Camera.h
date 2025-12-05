#pragma once
#include "logic/utils/Position.h"

namespace pacman {

class Camera {
public:
    Camera(int windowWidth, int windowHeight);

    Position worldToScreen(const Position& worldPos) const;
    Position screenToWorld(int screenX, int screenY) const;
    float getSpriteSize() const;
    void setMapDimensions(int rows, int cols);
    void setWindowSize(int width, int height);

    int getWindowWidth() const { return windowWidth; }
    int getWindowHeight() const { return windowHeight; }
    float getWorldWidth() const { return worldWidth; }
    float getWorldHeight() const { return worldHeight; }

    // ✅ NIEUW: letterbox offsets
    float getViewportOffsetX() const { return viewportOffsetX; }
    float getViewportOffsetY() const { return viewportOffsetY; }
    float getViewportWidth() const { return viewportWidth; }
    float getViewportHeight() const { return viewportHeight; }

private:
    int windowWidth;
    int windowHeight;
    float scale;

    int mapRows = 11;
    int mapCols = 20;

    float worldWidth = 1.82f;
    float worldHeight = 1.0f;

    // ✅ NIEUW: viewport voor letterboxing
    float viewportOffsetX = 0.0f;
    float viewportOffsetY = 0.0f;
    float viewportWidth = 0.0f;
    float viewportHeight = 0.0f;
    
    void calculateScale();
};

} // namespace pacman