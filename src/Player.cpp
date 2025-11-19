#include "Player.hpp"
#include <iostream>
using namespace std;

const LANE_LEFT = 0.00f; /*temporary values*/
const LANE_RIGHT = 0.00f; /*temporary values*/
const LANE_MIDDLE = 0.00f; /*temporary values*/

const float LANE_POSITIONS[3] = { LANE_LEFT, LANE_MIDDLE, LANE_RIGHT };
const float LERP_SPEED = 0.15f;

Player :: Player()
{
    if(!texture.loadFromFile("/assets/player.png")) /* loading the player image and checks if its available */
    {
        std::cer << " Could access Player file ";
    }
    sprite.setTexture(texture);
    sprite.setScale(0.5f, 0.5f); /* adjusting car size to fit the road*/

    /* sets origin as the centre of the sprite - easier later*/
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f); 

    
    /*players starting state*/
    laneIndex = 1; // the middle lane 
    currentX = LANE_POSITIONS[laneIndex];
    targetX = currentX;

    sprite.setPosition(currentX, yPos);

}

/* 
lanes divided into 0 - left lane, 1 - middle lane, 2 - right lane
check current lane then update the cars position by how the car is moved
*/
void Player :: moveLeft()
{
    if(laneIndex > 0)
    {
        laneIndex--;
        targetX = LANE_POSITIONS[laneIndex];
    }
}

void Player :: moveRight()
{
    if(laneIndex < 2) 
    {
        laneIndex++;
        targetX = LANE_POSITIONS[laneIndex];
    }
}

void Player::update(float dt) 
{
    // It moves currentX *towards* targetX by a small amount each frame
    currentX += (targetX - currentX) * LERP_SPEED;

    // Update the sprite's actual on-screen position
    sprite.setPosition(currentX, yPos);
}

void Player::draw(sf::RenderWindow &window) 
{
    window.draw(sprite);
}

// Used for collision detection 
sf::FloatRect Player::getBounds() const 
{
    return sprite.getGlobalBounds(); // global bound - used when we need to the bound to move with the object .
}
