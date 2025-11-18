#pragma once
#include <SFML/Graphics.hpp>

class Player {
    public:
        Player();

        void moveLeft();
        void moveRight();
        void update(float dt);
        void draw(sf::RenderWindow &window);
        sf::FloatRect getBounds() const;

    private:
        sf::Sprite sprite;
        sf::Texture texture;

        int laneIndex;
        float currentX;
        float targetX;
        float yPos;

}