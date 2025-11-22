#pragma once  // FIX 1: Removed "#include <...>" 
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
    
    // NOTE: Keeping this here is risky (see warning below), 
    // but it matches your current .cpp file.
    sf::Texture texture; 

    int laneIndex;
    float currentX;
    float targetX;
    float yPos;

}; // FIX 2: Added the missing semicolon here!