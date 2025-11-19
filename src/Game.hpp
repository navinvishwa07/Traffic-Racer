#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.hpp"
#include "Road.hpp"
#include "VehicleManager.hpp"

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void resetGame();
    void spawnCoin();

    sf::RenderWindow window;
    
    Player player;
    Road road;
    VehicleManager vehicleManager;

    sf::Texture tPlayer, tRoad, tCoin; 
    sf::Texture tStartBtn, tRestartBtn; 

    std::vector<sf::Texture> vehicleTextures; 

    sf::Sprite btnStart;
    sf::Sprite btnRestart;

    std::vector<sf::Sprite> coins;
    float coinSpawnTimer;

    sf::Font font;
    
    // --- UI TEXT ---
    sf::Text textScore; // Will stay Top Left
    sf::Text textCoins; // Will move to Top Right
    sf::Text textGameOver; 

    GameState currentState; 
    float gameSpeed;
    int score;
    int coinPoints;
};