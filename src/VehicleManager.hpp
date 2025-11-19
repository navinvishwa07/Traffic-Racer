#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Vehicle.hpp" // This links the two files

class VehicleManager {
public:
    VehicleManager();

    void init(const std::vector<sf::Texture>& textures);
    
    void update(float dt, float gameSpeed, int& score);
    void draw(sf::RenderWindow& window);
    bool checkCollision(sf::FloatRect playerBounds);
    void reset();

private:
    std::vector<Vehicle> vehicles;
    const std::vector<sf::Texture>* ptrTextures; 
    float spawnTimer;
};