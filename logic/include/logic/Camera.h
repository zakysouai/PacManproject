#pragma once
#include "logic/utils/Position.h"

namespace pacman {

class Camera {
public:
    Camera(int windowWidth, int windowHeight);
    
    // Convert world coordinates [-1, 1] to screen pixels
    Position worldToScreen(const Position& worldPos) const;
    
    // Convert screen pixels to world coordinates
    Position screenToWorld(int screenX, int screenY) const;
    
    // Get sprite size in pixels
    float getSpriteSize() const;
    
    // Update window size
    void setWindowSize(int width, int height);
    
    int getWindowWidth() const { return windowWidth; }
    int getWindowHeight() const { return windowHeight; }
    
private:
    int windowWidth;
    int windowHeight;
    float scale;  // Pixels per world unit
    
    void calculateScale();
};

} // namespace pacman