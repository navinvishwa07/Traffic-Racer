#include "Game.hpp"
#include "Constants.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm> 

Game::Game() 
    // CHANGE 1: Added "sf::Style::Titlebar | sf::Style::Close" to DISABLE resizing.
    // This fixes the "Restart button not working" issue permanently.
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Traffic Racer", sf::Style::Titlebar | sf::Style::Close),
      currentState(MENU), score(0), highScore(0), gameSpeed(300.f), coinSpawnTimer(0.f), lastSpeedLevel(0)
{
    window.setFramerateLimit(60);

    // --- 1. LOAD ASSETS ---
    if (!tPlayer.loadFromFile("assets/player.png")) std::cerr << "Missing player.png\n";
    if (!tRoad.loadFromFile("assets/road.png")) std::cerr << "Missing road.png\n";
    if (!tCoin.loadFromFile("assets/coin.png")) std::cerr << "Missing coin.png\n"; 

    // --- ULTIMATE MAC FONT FIX ---
    bool fontLoaded = false;
    std::vector<std::string> fontPaths = {
        "assets/font.ttf",                              
        "/Library/Fonts/Arial.ttf",                     
        "/System/Library/Fonts/Helvetica.ttc",          
        "/System/Library/Fonts/Menlo.ttc",              
        "/System/Library/Fonts/Geneva.dfont",           
        "/System/Library/Fonts/Supplemental/Arial.ttf"  
    };

    for (const auto& path : fontPaths) {
        if (font.loadFromFile(path)) {
            std::cout << "SUCCESS: Loaded font from " << path << "\n";
            fontLoaded = true;
            break;
        }
    }
    
    if (!fontLoaded) {
        std::cerr << "CRITICAL ERROR: No font found. Text will be invisible.\n";
    }

    if (!tStartBtn.loadFromFile("assets/start.png")) std::cerr << "Missing start.png\n";
    if (!tRestartBtn.loadFromFile("assets/restart.png")) std::cerr << "Missing restart.png\n";

    // Load Sounds
    if (!sbCoin.loadFromFile("assets/coin.wav")) std::cerr << "Missing coin.wav\n";
    if (!sbCrash.loadFromFile("assets/crash.wav")) std::cerr << "Missing crash.wav\n";
    if (!sbBoost.loadFromFile("assets/boost.wav")) std::cerr << "Missing boost.wav\n";

    soundCoin.setBuffer(sbCoin);
    soundCrash.setBuffer(sbCrash);
    soundBoost.setBuffer(sbBoost);
    soundBoost.setVolume(50.f); 

    std::vector<std::string> fileNames = {
        "assets/ambulance.png", "assets/bus.png", "assets/car.png",
        "assets/car1.png", "assets/policecar.png", "assets/truck.png"
    };

    for (const auto& file : fileNames) {
        sf::Texture temp;
        if (temp.loadFromFile(file)) vehicleTextures.push_back(temp);
    }

    // --- 2. INIT OBJECTS ---
    player.init(tPlayer);
    road.init(tRoad);
    vehicleManager.init(vehicleTextures);
    
    loadHighScore();

    // --- 3. UI SETUP ---
    
    // Buttons
    btnStart.setTexture(tStartBtn);
    btnStart.setScale(0.25f, 0.25f); 
    sf::FloatRect sBounds = btnStart.getLocalBounds();
    btnStart.setOrigin(sBounds.width/2, sBounds.height/2);
    btnStart.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

    btnRestart.setTexture(tRestartBtn);
    btnRestart.setScale(0.25f, 0.25f); 
    sf::FloatRect rBounds = btnRestart.getLocalBounds();
    btnRestart.setOrigin(rBounds.width/2, rBounds.height/2);
    btnRestart.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 50); 

    // SCORE TEXT 
    textScore.setFont(font);
    textScore.setString("Score: 0"); 
    textScore.setCharacterSize(24);
    textScore.setFillColor(sf::Color::White);
    textScore.setPosition(20, 15); 

    // HIGH SCORE TEXT
    textHighScore.setFont(font);
    textHighScore.setString("Best: " + std::to_string(highScore)); 
    textHighScore.setCharacterSize(24);
    textHighScore.setFillColor(sf::Color::Yellow);
    textHighScore.setPosition(20, 45); 

    // CHANGE 2: Speedometer Position Fix
    // Moved text further left (WINDOW_WIDTH - 200) so "KM/H" fits easily.
    textSpeed.setFont(font);
    textSpeed.setString("0 KM/H");
    textSpeed.setCharacterSize(24);
    textSpeed.setFillColor(sf::Color::Cyan);
    textSpeed.setPosition(WINDOW_WIDTH - 200, 15); // Was -150, moved left

    // Game Over Stuff
    gameOverOverlay.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    gameOverOverlay.setFillColor(sf::Color(50, 50, 50, 240)); 
    gameOverOverlay.setPosition(0, 0);

    textGameOver.setFont(font);
    textGameOver.setString("CRASHED!");
    textGameOver.setCharacterSize(50);
    textGameOver.setFillColor(sf::Color::Red);
    sf::FloatRect gBounds = textGameOver.getLocalBounds();
    textGameOver.setOrigin(gBounds.width/2, gBounds.height/2);
    textGameOver.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 100); 
}

void Game::loadHighScore() {
    std::ifstream inFile("../highscore.txt");
    if (inFile.is_open()) {
        inFile >> highScore;
        inFile.close();
    }
}

void Game::saveHighScore() {
    std::ofstream outFile("../highscore.txt");
    if (outFile.is_open()) {
        outFile << highScore;
        outFile.close();
    }
}

void Game::resetGame() {
    currentState = PLAYING;
    score = 0;
    gameSpeed = 300.f;
    lastSpeedLevel = 0;
    vehicleManager.reset();
    coins.clear();
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

void Game::spawnCoin() {
    sf::Sprite c;
    c.setTexture(tCoin);
    c.setScale(0.15f, 0.15f);
    sf::FloatRect bounds = c.getLocalBounds();
    c.setOrigin(bounds.width/2, bounds.height/2);

    int lane = rand() % 3;
    float xPos = LANE_MIDDLE;
    if (lane == 0) xPos = LANE_LEFT;
    if (lane == 2) xPos = LANE_RIGHT;

    c.setPosition(xPos, -50.f);
    coins.push_back(c);
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            saveHighScore(); 
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                // CHANGE 3: Robust Mouse Coordinate Mapping
                // Even if something weird happens with window size, this ensures accuracy.
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF = window.mapPixelToCoords(mousePos);

                if (currentState == MENU) {
                    if (btnStart.getGlobalBounds().contains(mousePosF)) currentState = PLAYING;
                }
                else if (currentState == GAME_OVER) {
                    if (btnRestart.getGlobalBounds().contains(mousePosF)) resetGame();
                }
            }
        }

        if (event.type == sf::Event::KeyPressed) {
            if (currentState == MENU && event.key.code == sf::Keyboard::Enter) currentState = PLAYING;
            if (currentState == PLAYING) {
                if (event.key.code == sf::Keyboard::Left) player.moveLeft();
                if (event.key.code == sf::Keyboard::Right) player.moveRight();
            }
            if (currentState == GAME_OVER && event.key.code == sf::Keyboard::R) resetGame();
        }
    }
}

void Game::update(float dt) {
    if (currentState != PLAYING) return;

    gameSpeed += 40.0f * dt; 
    
    int currentLevel = static_cast<int>(gameSpeed) / 500;
    if (currentLevel > lastSpeedLevel) {
        soundBoost.play(); 
        lastSpeedLevel = currentLevel;
    }

    road.update(dt, gameSpeed);
    player.update(dt);
    int dummyScore = 0; 
    vehicleManager.update(dt, gameSpeed, dummyScore);

    float coinRate = std::max(0.4f, 2.0f - (gameSpeed * 0.002f));
    coinSpawnTimer += dt;
    if (coinSpawnTimer > coinRate) {
        spawnCoin();
        coinSpawnTimer = 0.f;
    }

    sf::FloatRect coinHitbox = player.getBounds();
    float shrink = coinHitbox.width * 0.10f; 
    coinHitbox.left += shrink;
    coinHitbox.width -= (shrink * 2);

    for (auto it = coins.begin(); it != coins.end(); ) {
        it->move(0, gameSpeed * dt); 
        
        if (it->getGlobalBounds().intersects(coinHitbox)) {
            score += 1; 
            soundCoin.play(); 
            
            if (score > highScore) {
                highScore = score;
                saveHighScore(); 
            }
            it = coins.erase(it);
        }
        else if (it->getPosition().y > WINDOW_HEIGHT + 50) {
            it = coins.erase(it);
        } 
        else {
            ++it;
        }
    }

    if (vehicleManager.checkCollision(player.getBounds())) {
        soundCrash.play(); 
        saveHighScore();
        currentState = GAME_OVER;
    }

    textScore.setString("Score: " + std::to_string(score));
    textHighScore.setString("Best: " + std::to_string(highScore));
    
    int displaySpeed = static_cast<int>(gameSpeed / 10.0f); 
    textSpeed.setString(std::to_string(displaySpeed) + " KM/H");
}

void Game::render() {
    window.clear();
    
    road.draw(window);
    
    if (currentState != MENU) {
        vehicleManager.draw(window);
        for (auto& c : coins) window.draw(c);
        player.draw(window);
        
        // --- DRAW UI ---
        
        // CHANGE 4: Speed Box Fix
        // Made the black box wider (200.f) and moved it left to match the text
        sf::RectangleShape speedBox(sf::Vector2f(200.f, 40.f));
        speedBox.setFillColor(sf::Color(0, 0, 0, 150));
        speedBox.setPosition(WINDOW_WIDTH - 210.f, 10.f);
        window.draw(speedBox);

        window.draw(textScore);
        window.draw(textHighScore);
        window.draw(textSpeed);
    }

    if (currentState == MENU) {
        window.draw(btnStart);
    }
    else if (currentState == GAME_OVER) {
        window.draw(gameOverOverlay);
        window.draw(textGameOver);
        window.draw(btnRestart);
        
        textScore.setPosition(WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT/2 - 40);
        window.draw(textScore);
        textScore.setPosition(20, 15); 
    }

    window.display();
}