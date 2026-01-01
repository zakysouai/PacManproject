#pragma once
#include "logic/utils/Position.h"

namespace pacman {

/**
 * @brief Camera - projectie van world coordinates naar screen pixels
 *
 * === COORDINATE SYSTEMEN ===
 *
 * World Space (logic):
 * - Genormaliseerd: [-worldWidth, +worldWidth] x [-1.0, +1.0]
 * - Aspect ratio dependent (worldWidth = mapCols / mapRows)
 * - Resolutie-onafhankelijk
 *
 * Screen Space (representation):
 * - Pixels: [0, windowWidth] x [0, windowHeight]
 * - Origin linksboven
 *
 * === LETTERBOXING ===
 * Wanneer window aspect ratio ≠ world aspect ratio:
 * - Window breder: verticale bars links/rechts (viewportOffsetX > 0)
 * - Window smaller: horizontale bars boven/onder (viewportOffsetY > 0)
 *
 * Dit voorkomt stretching/squashing van sprites.
 *
 * === SPRITE SIZING ===
 * getSpriteSize() berekent sprite size op basis van:
 * - Tile size in world units (2.0 / mapRows)
 * - Scale factor (viewport / world)
 * - Safety margin (0.91x voor gaps tussen sprites)
 */
class Camera {
public:
    Camera(int windowWidth, int windowHeight);

    /**
     * @brief Converteer world positie naar screen pixels
     * @param worldPos Positie in world space
     * @return Position in screen space (pixels)
     *
     * Formule:
     * screenX = viewportOffsetX + (viewportWidth / 2) + worldPos.x * scale
     * screenY = viewportOffsetY + (viewportHeight / 2) + worldPos.y * scale
     */
    Position worldToScreen(const Position& worldPos) const;

    /**
     * @brief Converteer screen pixels naar world positie
     * @param screenX X in pixels
     * @param screenY Y in pixels
     * @return Position in world space
     */
    Position screenToWorld(int screenX, int screenY) const;

    /**
     * @brief Bereken sprite size in pixels
     * @return Size voor één tile (0.91x tile voor gap)
     *
     * Gebruikt door Views om sprites te schalen.
     */
    float getSpriteSize() const;

    /**
     * @brief Update map dimensies (triggert scale recalculatie)
     * @param rows Aantal rijen in map
     * @param cols Aantal kolommen in map
     */
    void setMapDimensions(int rows, int cols);

    /**
     * @brief Update window size (triggert scale recalculatie)
     * @param width Nieuwe window width
     * @param height Nieuwe window height
     */
    void setWindowSize(int width, int height);

    // Getters voor viewport berekeningen
    int getWindowWidth() const { return windowWidth; }
    int getWindowHeight() const { return windowHeight; }
    float getWorldWidth() const { return worldWidth; }
    float getWorldHeight() const { return worldHeight; }

    // Letterbox offsets (voor UI positioning buiten viewport)
    float getViewportOffsetX() const { return viewportOffsetX; }
    float getViewportOffsetY() const { return viewportOffsetY; }
    float getViewportWidth() const { return viewportWidth; }
    float getViewportHeight() const { return viewportHeight; }

private:
    int windowWidth;
    int windowHeight;
    float scale;  // Pixels per world unit

    int mapRows = 11;
    int mapCols = 20;

    // World bounds (genormaliseerd)
    float worldWidth = 1.82f;   // Berekend uit aspect ratio
    float worldHeight = 1.0f;   // Vast

    // Viewport (met letterboxing)
    float viewportOffsetX = 0.0f;
    float viewportOffsetY = 0.0f;
    float viewportWidth = 0.0f;
    float viewportHeight = 0.0f;

    /**
     * @brief Bereken scale en viewport bounds
     *
     * Algoritme:
     * 1. Vergelijk window aspect ratio met world aspect ratio
     * 2. Kies limiting factor (width of height)
     * 3. Bereken scale (pixels/unit)
     * 4. Bereken viewport size en letterbox offsets
     */
    void calculateScale();
};

} // namespace pacman