#include "Game.hpp"
#include "Constants.hpp"
#include <iostream>
#include <vector>
#include <string>

Game::Game() 
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Traffic Racer"),
      currentState(MENU), score(0), coinPoints(0), gameSpeed(300.f), coinSpawnTimer(0.f)
{
    window.setFramerateLimit(60);

    // 1. LOAD ASSETS
    if (!tPlayer.loadFromFile("assets/player.png")) std::cerr << "Missing player.png\n";
    if (!tRoad.loadFromFile("assets/road.png")) std::cerr << "Missing road.png\n";
    if (!tCoin.loadFromFile("assets/coin.png")) std::cerr << "Missing coin.png\n"; 
    if (!font.loadFromFile("assets/font.ttf")) std::cerr << "Missing font.ttf\n";
    if (!tStartBtn.loadFromFile("assets/start.png")) std::cerr << "Missing start.png\n";
    if (!tRestartBtn.loadFromFile("assets/restart.png")) std::cerr << "Missing restart.png\n";

    std::vector<std::string> fileNames = {
        "assets/ambulance.png", "assets/bus.png", "assets/car.png",
        "assets/car1.png", "assets/policecar.png", "assets/truck.png"
    };

    for (const auto& file : fileNames) {
        sf::Texture temp;
        if (temp.loadFromFile(file)) vehicleTextures.push_back(temp);
    }

    // 2. INIT OBJECTS
    player.init(tPlayer);
    road.init(tRoad);
    vehicleManager.init(vehicleTextures);

    // 3. UI SETUP
    btnStart.setTexture(tStartBtn);
    sf::FloatRect sBounds = btnStart.getLocalBounds();
    btnStart.setOrigin(sBounds.width/2, sBounds.height/2);
    btnStart.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

    btnRestart.setTexture(tRestartBtn);
    sf::FloatRect rBounds = btnRestart.getLocalBounds();
    btnRestart.setOrigin(rBounds.width/2, rBounds.height/2);
    btnRestart.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 50); 

    // SCORE TEXT (Top Left)
    textScore.setFont(font);
    textScore.setCharacterSize(24);
    textScore.setFillColor(sf::Color::White);
    textScore.setPosition(20, 20); 

    // COINS TEXT (Top Left, Under Score)
    textCoins.setFont(font);
    textCoins.setCharacterSize(24);
    textCoins.setFillColor(sf::Color::Yellow);
    textCoins.setPosition(20, 50); // 30 pixels below score

    textGameOver.setFont(font);
    textGameOver.setString("GAME OVER");
    textGameOver.setCharacterSize(45);
    textGameOver.setFillColor(sf::Color::Red);
    sf::FloatRect gBounds = textGameOver.getLocalBounds();
    textGameOver.setOrigin(gBounds.width/2, gBounds.height/2);
    textGameOver.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 100); 
}

void Game::resetGame() {
    currentState = PLAYING;
    score = 0;
    coinPoints = 0;
    gameSpeed = 300.f;
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
        if (event.type == sf::Event::Closed) window.close();

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

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

    gameSpeed += 5.0f * dt;
    road.update(dt, gameSpeed);
    player.update(dt);
    vehicleManager.update(dt, gameSpeed, score);

    coinSpawnTimer += dt;
    if (coinSpawnTimer > 2.5f) {
        spawnCoin();
        coinSpawnTimer = 0.f;
    }

    for (auto it = coins.begin(); it != coins.end(); ) {
        it->move(0, gameSpeed * dt); 
        if (it->getGlobalBounds().intersects(player.getBounds())) {
            coinPoints += 50; // 50 points per coin
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
        currentState = GAME_OVER;
    }

    // Update Strings
    textScore.setString("Score: " + std::to_string(score));
    textCoins.setString("Coins: " + std::to_string(coinPoints));
}

void Game::render() {
    window.clear();
    
    road.draw(window);
    
    if (currentState != MENU) {
        vehicleManager.draw(window);
        for (auto& c : coins) window.draw(c);
        player.draw(window);
        
        // Draw UI
        window.draw(textScore);
        window.draw(textCoins);
    }

    if (currentState == MENU) {
        window.draw(btnStart);
    }
    else if (currentState == GAME_OVER) {
        window.draw(textGameOver);
        window.draw(btnRestart);
    }

    window.display();
}