#pragma once
#include <SFML/Graphics.hpp>

class Vehicle {
public:
    Vehicle(const sf::Texture& texture, int laneIndex, float speed);
    
    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool isOffScreen() const;

private:
    sf::Sprite sprite;
    float speed;
};