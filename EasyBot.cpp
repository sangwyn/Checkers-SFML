#include "EasyBot.h"

std::pair<sf::Vector2i, sf::Vector2i> EasyBot::CalculateMove(GameManager &game_manager) {
    std::vector<sf::Vector2i> can_move;
    std::vector<std::vector<sf::Vector2i>> moves;
    std::vector<sf::Vector2i> can_eat;
    std::vector<std::vector<sf::Vector2i>> eats;

    FillVectors(can_move, moves, can_eat, eats, game_manager);

    if (!can_eat.empty()) {
        dis = std::uniform_int_distribution<>(0, can_eat.size() - 1);
        int ind = dis(gen);
        sf::Vector2i from = can_eat[ind];
        dis = std::uniform_int_distribution<>(0, eats[ind].size() - 1);
        sf::Vector2i to = eats[ind][dis(gen)];

        return {from, to};
    } else if (!can_move.empty()) {
        dis = std::uniform_int_distribution<>(0, can_move.size() - 1);
        int ind = dis(gen);
        sf::Vector2i from = can_move[ind];
        dis = std::uniform_int_distribution<>(0, moves[ind].size() - 1);
        sf::Vector2i to = moves[ind][dis(gen)];

        return {from, to};
    } else {
        return {{0, 0,}, {0, 0}};
    }
}