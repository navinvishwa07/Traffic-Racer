#include "Player.hpp"
#include "Constants.hpp" // We need this for LANE_MIDDLE, etc.

Player::Player() {
    // We only set variables here. 
    // We DO NOT load the texture here anymore.
    laneIndex = 1; 
    currentX = LANE_MIDDLE;
    targetX = LANE_MIDDLE;
}

void Player::init(const sf::Texture& texture) {
    // The Game class gives us the texture now
    sprite.setTexture(texture);
    sprite.setScale(0.5f, 0.5f); // Resize if needed
    
    // Center the sprite
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    
    sprite.setPosition(currentX, PLAYER_Y);
}

void Player::moveLeft() {
    if (laneIndex > 0) {
        laneIndex--;
        if (laneIndex == 0) targetX = LANE_LEFT;
        if (laneIndex == 1) targetX = LANE_MIDDLE;
    }
}

void Player::moveRight() {
    if (laneIndex < 2) {
        laneIndex++;
        if (laneIndex == 1) targetX = LANE_MIDDLE;
        if (laneIndex == 2) targetX = LANE_RIGHT;
    }
}

void Player::update(float dt) {
    // Smooth glide logic
    currentX += (targetX - currentX) * LANE_SMOOTHING * dt;
    
    // Tilt effect
    float tilt = (targetX - currentX) * 0.15f;
    sprite.setRotation(tilt);

    sprite.setPosition(currentX, PLAYER_Y);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}