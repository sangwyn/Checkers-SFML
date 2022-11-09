#include "HardBot.h"

#include <iostream>

std::pair<sf::Vector2i, sf::Vector2i> HardBot::CalculateMove(GameManager &game_manager) {
    return CalculateBestMove(game_manager, white_, 0, kMaxDepth);
}
