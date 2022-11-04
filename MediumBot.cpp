#include "MediumBot.h"

std::pair<sf::Vector2i, sf::Vector2i> MediumBot::CalculateMove(GameManager &game_manager) {
    std::vector<sf::Vector2i> can_move;
    std::vector<std::vector<sf::Vector2i>> moves;
    std::vector<sf::Vector2i> can_eat;
    std::vector<std::vector<sf::Vector2i>> eats;

    FillVectors(can_move, moves, can_eat, eats, game_manager);

    if (can_eat.empty() && can_move.empty())
        return {{0, 0,}, {0, 0}};

    dis = std::uniform_int_distribution<>(0, 2);
    if (!dis(gen)) {
        if (!can_eat.empty()) {
            dis = std::uniform_int_distribution<>(0, can_eat.size() - 1);
            int ind = dis(gen);
            sf::Vector2i from = can_eat[ind];
            dis = std::uniform_int_distribution<>(0, eats[ind].size() - 1);
            sf::Vector2i to = eats[ind][dis(gen)];

            return {from, to};
        } else {
            dis = std::uniform_int_distribution<>(0, can_move.size() - 1);
            int ind = dis(gen);
            sf::Vector2i from = can_move[ind];
            dis = std::uniform_int_distribution<>(0, moves[ind].size() - 1);
            sf::Vector2i to = moves[ind][dis(gen)];

            return {from, to};
        }
    } else {
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
        } else {
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
        }
    }
}