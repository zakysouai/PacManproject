#pragma once
#include "EntityView.h"
#include "logic/entities/Coin.h"

namespace pacman::representation {

class CoinView : public EntityView {
public:
    CoinView(pacman::Coin* model, std::weak_ptr<pacman::Camera> camera);  // ✅ CHANGED
    ~CoinView() override = default;

    void draw(sf::RenderWindow& window) override;

private:
    pacman::Coin* coinModel;
    sf::CircleShape circle;
    
    void setupCircle();
};

} // namespace pacman::representation