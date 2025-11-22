#include "Game.hpp"
#include <cstdlib>
#include <ctime>

int main() {
    // Seed the random number generator for random car spawns
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    Game game;
    game.run();

    return 0;
}