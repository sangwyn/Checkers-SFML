#include "HardBot.h"

#include <iostream>

std::pair<sf::Vector2i, sf::Vector2i> HardBot::CalculateMove(GameManager &game_manager) {
    std::vector<sf::Vector2i> can_move;
    std::vector<std::vector<sf::Vector2i>> moves;
    std::vector<sf::Vector2i> can_eat;
    std::vector<std::vector<sf::Vector2i>> eats;

    FillVectors(can_move, moves, can_eat, eats, game_manager);

    if (!can_eat.empty()) {
        sf::Vector2i from, to;
        int max = -100000;

        for (int i = 0; i < can_eat.size(); ++i) {
            for (auto e : eats[i]) {
                int r = GetProfit(e.x, e.y, game_manager);
//                std::cout << r << '\n';
                if (r > max) {
                    max = r;
                    from = can_eat[i];
                    to = e;
                }
            }
        }

        return {from, to};
    } else if (!can_move.empty()) {
        sf::Vector2i from, to;
        int max = -100000;

        for (int i = 0; i < can_move.size(); ++i) {
            for (auto e : moves[i]) {
                int r = GetProfit(e.x, e.y, game_manager);
                if (r > max) {
                    max = r;
                    from = can_move[i];
                    to = e;
                }
            }
        }

        return {from, to};
    } else {
        return {{0, 0}, {0, 0}};
    }
}
