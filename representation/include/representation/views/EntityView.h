#pragma once
#include "logic/EntityModel.h"
#include "logic/Camera.h"
#include "logic/patterns/Observer.h"
#include <SFML/Graphics.hpp>
#include <memory>

namespace pacman::representation {

/**
 * @brief Abstracte base voor alle entity views (MVC View layer)
 *
 * === MODEL-VIEW-CONTROLLER PATTERN ===
 * - Model (EntityModel): Game state en logica
 * - View (EntityView): Visuele representatie
 * - Controller (World): Interacties tussen entities
 *
 * === OBSERVER PATTERN ===
 * View observeert Model voor state changes:
 * - ENTITY_UPDATED: Update sprite positie
 * - DIRECTION_CHANGED: Switch animatie
 * - GHOST_STATE_CHANGED: Switch scared/normal sprite
 *
 * === LIFETIME ===
 * 1. Creation: ConcreteFactory maakt View + Model
 * 2. Attachment: View->attach(Model) in factory
 * 3. Updates: Model notify → View onNotify
 * 4. Destruction: ~EntityView() detacht automatisch
 *
 * === CAMERA USAGE ===
 * Camera is weak_ptr:
 * - View ownt camera niet
 * - Camera outlives views (owned door Game/LevelState)
 * - Lock before gebruik: auto cam = camera.lock()
 */
class EntityView : public pacman::Observer {
public:
    /**
     * @brief Constructor
     * @param model Reference naar Model (View observeert deze)
     * @param camera Weak pointer naar Camera (voor coordinate conversie)
     */
    EntityView(pacman::EntityModel& model, std::weak_ptr<pacman::Camera> camera);

    /**
     * @brief Destructor - detach van Model
     *
     * Zeer belangrijk: detach voorkomt dangling observer pointer.
     * Model mag niet notificeren naar destroyed View.
     */
    virtual ~EntityView();

    /**
     * @brief Ontvang event van Model
     * @param event Event data (type, value, deltaTime)
     *
     * Typische gebruik:
     * - ENTITY_UPDATED: updateSpritePosition(), update animation
     * - DIRECTION_CHANGED: switch animation
     * - Custom per subclass
     */
    void onNotify(const pacman::Event& event) override;

    /**
     * @brief Render sprite naar window (pure virtual)
     * @param window SFML window om naar te tekenen
     */
    virtual void draw(sf::RenderWindow& window) = 0;

    /**
     * @brief Update view state
     * @param deltaTime Tijd sinds laatste frame
     *
     * DEPRECATED: Updates gebeuren nu via onNotify(ENTITY_UPDATED).
     * Methode blijft bestaan voor backward compatibility.
     */
    virtual void update(float deltaTime);

protected:
    pacman::EntityModel& model;  // Reference naar Model (non-owning, Model owned door World)
    std::weak_ptr<pacman::Camera> camera;  // Weak ptr: View ownt camera niet

    sf::Sprite sprite;  // SFML sprite voor rendering

    /**
     * @brief Update sprite positie op basis van Model positie
     *
     * Gebruikt Camera::worldToScreen() voor conversie.
     * Roep aan in onNotify(ENTITY_UPDATED).
     */
    void updateSpritePosition();
};

} // namespace pacman::representation