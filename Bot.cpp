#include "Bot.h"

int Bot::GetMinEats(int res, int i, int j, GameManager game_manager) {
    auto eats = game_manager.GetPossibleEats(i, j);
    if (eats.empty())
        return res;
    for (auto e : eats) {
        res = std::min(res, GetMaxEats(res + 1, e.x, e.y, game_manager));
    }
    return res;
}

int Bot::GetMaxEats(int res, int i, int j, GameManager game_manager) {
    auto eats = game_manager.GetPossibleEats(i, j);
    if (eats.empty())
        return res;
    for (auto e : eats) {
        res = std::max(res, GetMaxEats(res + 1, e.x, e.y, game_manager));
    }
    return res;
}

int Bot::GetProfit(int i, int j, GameManager game_manager) {
    int res = 0;
    int min_eats = GetMinEats(0, i, j, game_manager);
    res -= min_eats;

    auto field = game_manager.GetField();
    if (i == 0 || i == 7)
        res += 1;
    if (white_) {
        if (i > 0 && (j < 7 && field[i - 1][j + 1] < 0 || j > 0 && field[i - 1][j - 1] < 0)) {
            res += 1;
        } else {
            if (i > 0 && j < 7 && field[i - 1][j + 1] > 0)
                res -= 1;
            if (i > 0 && j > 0 && field[i - 1][j - 1] > 0)
                res -= 1;
        }
    } else {
        if (i < 7 && (j < 7 && field[i + 1][j + 1] > 0 || j > 0 && field[i + 1][j - 1] > 0)) {
            res += 1;
        } else {
            if (i < 7 && j < 7 && field[i + 1][j + 1] < 0)
                res -= 1;
            if (i < 7 && j > 0 && field[i + 1][j - 1] < 0)
                res -= 1;
        }
    }
    return res;
}

void Bot::FillVectors(std::vector<sf::Vector2i> &can_move,
                      std::vector<std::vector<sf::Vector2i>> &moves,
                      std::vector<sf::Vector2i> &can_eat,
                      std::vector<std::vector<sf::Vector2i>> &eats,
                      GameManager game_manager) {
    auto field = game_manager.GetField();
    auto eat_again = game_manager.EatAgainPos();
    if (eat_again.x == -1) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (white_ && field[i][j] > 0 || !white_ && field[i][j] < 0) {
                    if (game_manager.CanEat(i, j)) {
                        eats.push_back(game_manager.GetPossibleEats(i, j));
                        can_eat.push_back({i, j});
                    } else {
                        moves.push_back(game_manager.GetPossibleMoves(i, j));
                        if (moves[moves.size() - 1].empty()) {
                            moves.pop_back();
                        } else {
                            can_move.push_back({i, j});
                        }
                    };
                }
            }
        }
    } else {
        can_eat.push_back(eat_again);
        eats.push_back(game_manager.GetPossibleEats(eat_again, true));
    }
}