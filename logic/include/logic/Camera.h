#pragma once
#include "logic/utils/Position.h"

namespace pacman {

class Camera {
public:
    Camera(int windowWidth, int windowHeight);

    // Convert world coordinates to screen pixels
    Position worldToScreen(const Position& worldPos) const;

    // Convert screen pixels to world coordinates
    Position screenToWorld(int screenX, int screenY) const;

    // Get sprite size in pixels
    float getSpriteSize() const;

    // ✅ Set map dimensions (moet aangeroepen worden na map laden!)
    void setMapDimensions(int rows, int cols);

    // Update window size
    void setWindowSize(int width, int height);

    int getWindowWidth() const { return windowWidth; }
    int getWindowHeight() const { return windowHeight; }

    // ✅ Getters voor world bounds
    float getWorldWidth() const { return worldWidth; }
    float getWorldHeight() const { return worldHeight; }

private:
    int windowWidth;
    int windowHeight;
    float scale;  // Pixels per world unit

    // ✅ Map dimensies
    int mapRows = 11;
    int mapCols = 20;

    // ✅ World bounds (gebaseerd op aspect ratio van map)
    float worldWidth = 1.82f;   // Default: 20/11
    float worldHeight = 1.0f;
    
    void calculateScale();
};

} // namespace pacman