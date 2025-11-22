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
            // Note: We do not add to score here anymore, only coins count.
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

// --- SUPER SMALL HITBOX FIX ---
bool VehicleManager::checkCollision(sf::FloatRect playerBounds) {
    
    // 1. Player Hitbox: Remove 30% from each side
    // This allows you to "tilt" without the corners snagging
    float playerShrinkX = playerBounds.width * 0.30f;
    float playerShrinkY = playerBounds.height * 0.20f;

    playerBounds.left += playerShrinkX;
    playerBounds.width -= (playerShrinkX * 2);
    playerBounds.top += playerShrinkY;
    playerBounds.height -= (playerShrinkY * 2);

    for (const auto& v : vehicles) {
        sf::FloatRect enemyBounds = v.getBounds();

        // 2. Enemy Hitbox: Remove 40% from EACH side
        // This leaves only a thin 20% strip in the middle as "deadly"
        float enemyShrinkX = enemyBounds.width * 0.40f; 
        
        // Remove 20% from top/bottom to avoid bumper crashes
        float enemyShrinkY = enemyBounds.height * 0.20f; 

        enemyBounds.left += enemyShrinkX;
        enemyBounds.width -= (enemyShrinkX * 2);
        enemyBounds.top += enemyShrinkY;
        enemyBounds.height -= (enemyShrinkY * 2);

        if (enemyBounds.intersects(playerBounds)) {
            return true;
        }
    }
    return false;
}