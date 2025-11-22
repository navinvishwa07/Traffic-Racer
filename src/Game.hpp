#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> // Required for Sound
#include <vector>
#include <fstream> // Required for file saving
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
    
    // High Score Persistence
    void loadHighScore();
    void saveHighScore();

    sf::RenderWindow window;
    
    Player player;
    Road road;
    VehicleManager vehicleManager;

    // Textures
    sf::Texture tPlayer, tRoad, tCoin; 
    sf::Texture tStartBtn, tRestartBtn; 
    std::vector<sf::Texture> vehicleTextures; 

    // Sprites
    sf::Sprite btnStart;
    sf::Sprite btnRestart;
    std::vector<sf::Sprite> coins;

    // Audio
    sf::SoundBuffer sbCoin, sbCrash, sbBoost;
    sf::Sound soundCoin, soundCrash, soundBoost;

    // UI Components
    sf::Font font;
    sf::Text textScore; 
    sf::Text textHighScore;
    sf::Text textSpeed;     // New Speed Display
    sf::Text textGameOver; 
    
    // Grey Overlay for Game Over
    sf::RectangleShape gameOverOverlay;

    // Game Data
    GameState currentState; 
    float gameSpeed;
    float coinSpawnTimer;
    int score;      
    int highScore;
    
    // Track speed for sound effects
    int lastSpeedLevel; 
};