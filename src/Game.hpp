#pragma once
#include <SFML/Graphics.hpp>
#include "Player.hpp"
// Forward declarations for classes we will build later
// class VehicleManager;
// class Background;

class Game {
public:
    // Game States (as defined in your design document)
    enum GameState {
        START_SCREEN,
        PLAYING,
        GAME_OVER
    };

    // Constructor requires the window to be passed from main
    Game(sf::RenderWindow& window); 

    void run();
    void handleInput();
    void update(float dt);
    void render();
    void checkCollisions();
    void resetGame();

private:
    sf::RenderWindow& window;
    Player player;
    // VehicleManager vehicleManager; // Placeholder
    // Background background;       // Placeholder
    
    GameState currentState;
    
    sf::Font font;
    sf::Text scoreText;

    float score;
    sf::Clock clock; // Used for calculating delta time (dt)
}; 