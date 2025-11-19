#include "VehicleManager.hpp"
#include "Constants.hpp" 
#include <cstdlib>

VehicleManager::VehicleManager() : ptrTextures(nullptr), spawnTimer(0.f) {}

void VehicleManager::init(const std::vector<sf::Texture>& textures) {
    ptrTextures = &textures; 
    vehicles.reserve(20);
}

void VehicleManager::reset() {
    vehicles.clear();
    spawnTimer = 0.f;
}

void VehicleManager::update(float dt, float gameSpeed, int& score) {
    spawnTimer += dt;

    float spawnRate = 2.0f - (gameSpeed * 0.001f); 
    if (spawnRate < 0.6f) spawnRate = 0.6f;

    if (spawnTimer > spawnRate && ptrTextures != nullptr && !ptrTextures->empty()) {
        int lane = rand() % 3;
        float enemySpeed = gameSpeed + 150.f;

        int randomIndex = rand() % ptrTextures->size();
        const sf::Texture& randomTexture = (*ptrTextures)[randomIndex];

        vehicles.emplace_back(randomTexture, lane, enemySpeed);
        spawnTimer = 0.f;
    }

    for (auto it = vehicles.begin(); it != vehicles.end(); ) {
        it->update(dt);

        if (it->isOffScreen()) {
            score += 10;
            it = vehicles.erase(it);
        } else {
            ++it;
        }
    }
}

void VehicleManager::draw(sf::RenderWindow& window) {
    for (auto& v : vehicles) {
        v.draw(window);
    }
}

bool VehicleManager::checkCollision(sf::FloatRect playerBounds) {
    
    // 1. Shrink PLAYER Hitbox
    float playerShrinkX = 20.f;
    float playerShrinkY = 20.f;

    playerBounds.left += playerShrinkX;
    playerBounds.width -= (playerShrinkX * 2);
    playerBounds.top += playerShrinkY;
    playerBounds.height -= (playerShrinkY * 2);

    for (const auto& v : vehicles) {
        sf::FloatRect enemyBounds = v.getBounds();

        // 2. AGGRESSIVE ENEMY HITBOX REDUCTION
        
        // SIDES: Remove 35% from EACH side (Total 70% width reduction)
        // This makes the car very thin collision-wise
        float enemyShrinkX = enemyBounds.width * 0.35f; 
        
        // FRONT/BACK: Remove 20% from Top/Bottom (Total 40% height reduction)
        float enemyShrinkY = enemyBounds.height * 0.20f; 

        enemyBounds.left += enemyShrinkX;
        enemyBounds.width -= (enemyShrinkX * 2);
        enemyBounds.top += enemyShrinkY;
        enemyBounds.height -= (enemyShrinkY * 2);

        // Check if the two smaller boxes touch
        if (enemyBounds.intersects(playerBounds)) {
            return true;
        }
    }
    return false;
}