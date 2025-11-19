#include "Road.hpp"
#include "Constants.hpp"

Road::Road() {}

void Road::init(const sf::Texture& texture) {
    sprite1.setTexture(texture);
    sprite2.setTexture(texture);

    // --- SCALE FIX ---
    // Calculate how much we need to shrink the 1024px image to fit 800px
    // 800 / 1024 = 0.78125
    float scaleX = static_cast<float>(WINDOW_WIDTH) / texture.getSize().x;
    float scaleY = scaleX; // Keep aspect ratio

    sprite1.setScale(scaleX, scaleY);
    sprite2.setScale(scaleX, scaleY);
    
    // Reset positions
    sprite1.setPosition(0, 0);
    
    // Place sprite2 directly above sprite1 based on the NEW scaled height
    // texture.getSize().y * scaleY gives the actual height in pixels on screen
    float scaledHeight = texture.getSize().y * scaleY;
    sprite2.setPosition(0, -scaledHeight);
}

void Road::update(float dt, float speed) {
    sprite1.move(0, speed * dt);
    sprite2.move(0, speed * dt);

    // We need the scaled height to know when to reset
    float scaledHeight = sprite1.getGlobalBounds().height;

    // If a segment goes off the bottom, move it to the top
    if (sprite1.getPosition().y >= WINDOW_HEIGHT) {
        sprite1.setPosition(0, sprite2.getPosition().y - scaledHeight);
    }

    if (sprite2.getPosition().y >= WINDOW_HEIGHT) {
        sprite2.setPosition(0, sprite1.getPosition().y - scaledHeight);
    }
}

void Road::draw(sf::RenderWindow& window) {
    window.draw(sprite1);
    window.draw(sprite2);
}