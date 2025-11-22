#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    Player();

    // Added init to receive the texture from the Game class
    void init(const sf::Texture& texture); 

    void moveLeft();
    void moveRight();
    
    // dt is good for frame-rate independent movement
    void update(float dt); 
    
    void draw(sf::RenderWindow &window);
    sf::FloatRect getBounds() const;

private:
    sf::Sprite sprite;
    // REMOVED: sf::Texture texture; (Store this in Game.hpp instead)

    int laneIndex;
    float currentX;
    float targetX;
    float yPos;
}; // <--- DON'T FORGET THIS SEMICOLON