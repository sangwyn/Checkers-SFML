#include "MediumBot.h"

std::pair<sf::Vector2i, sf::Vector2i> MediumBot::CalculateMove(GameManager &game_manager) {
    dis = std::uniform_int_distribution<>(0, 4);
    if (!dis(gen)) {
        return CalculateRandomMove(game_manager);
    } else {
        return CalculateBestMove(game_manager, white_, 0, kMaxDepth - 1);
    }
}