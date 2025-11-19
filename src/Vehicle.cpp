#include "Vehicle.hpp"
#include "Constants.hpp"
#include <cstdlib>

Vehicle::Vehicle(const sf::Texture& texture, int laneIndex, float spd) 
    : speed(spd) 
{
    sprite.setTexture(texture);
    
    // --- SCALE FIX ---
    // 0.28f makes the cars smaller to fit the lanes better
    sprite.setScale(0.28f, 0.28f); 
    
    // Center the sprite
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    // Position in lane
    float startX = LANE_MIDDLE;
    if (laneIndex == 0) startX = LANE_LEFT;
    if (laneIndex == 2) startX = LANE_RIGHT;

    sprite.setPosition(startX, -150.f); // Start above screen
}

void Vehicle::update(float dt) {
    sprite.move(0, speed * dt);
}

void Vehicle::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Vehicle::getBounds() const {
    return sprite.getGlobalBounds();
}

bool Vehicle::isOffScreen() const {
    return sprite.getPosition().y > WINDOW_HEIGHT + 100;
}