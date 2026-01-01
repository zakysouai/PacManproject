#pragma once
#include "EntityView.h"

namespace pacman::representation {

/**
 * @brief View voor Door - special case zonder Model
 *
 * === PROBLEEM ===
 * Door is geen Entity (geen logica, alleen visual blokkade).
 * Maar EntityView verwacht EntityModel& in constructor.
 *
 * === OPLOSSING (HACK) ===
 * 1. Creëer static DummyModel (leeg EntityModel)
 * 2. Pass reference naar DummyModel aan EntityView constructor
 * 3. DoorView gebruikt eigen doorPosition ipv model.getPosition()
 *
 * === ALTERNATIEF (NIET GEBRUIKT) ===
 * Refactor EntityView hierarchy:
 * - StaticView (geen model)
 * - DynamicView (met model)
 *
 * Keuze: Hack is sneller en minder invasive change.
 *
 * === VISUAL ===
 * Brown rectangle (spawn exit).
 * Blokkeert ghosts die al door zijn geweest (logica in World).
 */
class DoorView : public EntityView {
public:
    /**
     * @brief Constructor zonder model parameter
     * @param camera Weak pointer naar Camera
     * @param doorPos Positie van door in world coords
     *
     * Gebruikt internal static DummyModel voor EntityView base.
     */
    DoorView(std::weak_ptr<pacman::Camera> camera, const pacman::Position& doorPos);
    ~DoorView() override = default;

    void draw(sf::RenderWindow& window) override;

    /**
     * @brief Empty implementation - door reageert niet op events
     */
    void onNotify(const pacman::Event&) override {}

private:
    pacman::Position doorPosition;  // Eigen positie (niet van model)
    sf::RectangleShape rectangle;
};

} // namespace pacman::representation