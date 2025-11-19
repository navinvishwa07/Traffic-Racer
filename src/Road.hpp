#pragma once
#include <SFML/Graphics.hpp>

class Road {
public:
    Road();
    void init(const sf::Texture& texture);
    void update(float dt, float speed);
    void draw(sf::RenderWindow& window);

private:
    sf::Sprite sprite1;
    sf::Sprite sprite2;
};