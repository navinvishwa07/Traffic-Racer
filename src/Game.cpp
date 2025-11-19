#include "Game.hpp"
#include <iostream>

// --- Game Constructor ---
Game::Game(sf::RenderWindow& window) 
    : window(window), currentState(PLAYING), score(0.0f)
{
    // Load font for HUD/Scoring (Assuming "assets/arial.ttf" exists)
    if (!font.loadFromFile("assets/arial.ttf"))
    {
        std::cerr << "Error loading font!" << std::endl;
        // You should handle this by crashing or using a default font
    }
    
    // Set up score text HUD
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.0f, 10.0f); // Top left corner
}

// --- Main Game Loop (Replaces the loop in main.cpp) ---
void Game::run() 
{
    sf::Clock updateClock;
    
    while (window.isOpen())
    {
        // Calculate Delta Time (dt) for smooth, frame-rate-independent movement
        sf::Time elapsed = updateClock.restart();
        float dt = elapsed.asSeconds();
        
        handleInput();
        update(dt);
        render();
    }
}

// --- Input Handling ---
void Game::handleInput() 
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        
        if (currentState == PLAYING)
        {
            if (event.type == sf::Event::KeyPressed)
            {
                // Handles lane switching based on player input
                if (event.key.code == sf::Keyboard::Left)
                {
                    player.moveLeft();
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    player.moveRight();
                }
            }
        }
        else if (currentState == GAME_OVER)
        {
            // Input to restart the game
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
            {
                resetGame();
            }
        }
    }
}

// --- Game Logic Update ---
void Game::update(float dt) 
{
    if (currentState == PLAYING)
    {
        // 1. Update Player position (smooth lane switching)
        player.update(dt);
        
        // 2. Update Scoring (distance travelled)
        score += dt * 10; // 10 points per second
        scoreText.setString("Score: " + std::to_string(static_cast<int>(score)));
        
        // 3. Update Traffic/Background (future steps)
        // background.update(dt); 
        // vehicleManager.update(dt);
        
        // 4. Check Collisions (future step)
        // checkCollisions();
    }
}

// --- Rendering ---
void Game::render() 
{
    window.clear(sf::Color(70, 70, 70)); // Dark Gray background for road area
    
    // Draw all elements in appropriate order
    
    // 1. Draw Background (future step)
    // background.draw(window);
    
    // 2. Draw Traffic (future step)
    // vehicleManager.draw(window);
    
    // 3. Draw Player
    player.draw(window);
    
    // 4. Draw HUD (Score)
    window.draw(scoreText);
    
    window.display();
}

// --- Placeholders for Future Steps ---
void Game::checkCollisions() 
{
    // Implementation will go here in Step 4
}

void Game::resetGame() 
{
    score = 0.0f;
    // player.reset(); // Need to add a reset method to Player.hpp
    // vehicleManager.clear();
    currentState = PLAYING;
}