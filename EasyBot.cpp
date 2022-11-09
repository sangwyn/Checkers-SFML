#include "EasyBot.h"

std::pair<sf::Vector2i, sf::Vector2i> EasyBot::CalculateMove(GameManager &game_manager) {
    return CalculateRandomMove(game_manager);
}