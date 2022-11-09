#include "Bot.h"
#include <iostream>

int Bot::GetMinEats(int res, int i, int j, int depth) {
    auto eats = GetImEats(i, j);
    if (eats.empty())
        return res;
    for (auto e: eats) {
        res = std::min(res, GetMinEats(res + 1, e.x, e.y, depth + 1));
    }
    return res;
}

int Bot::GetMaxEats(int res, int i, int j, int depth) {
    auto eats = GetImEats(i, j);
    if (eats.empty())
        return res;
    for (auto e: eats) {
        res = std::max(res, GetMaxEats(res + 1, e.x, e.y, depth + 1));
    }
    return res;
}

int Bot::MatrixSum(std::vector<std::vector<int>> v) {
    int res = 0;

    for (int i = 0; i < v.size(); ++i) {
        for (int j = 0; j < v[i].size(); ++j) {
            res += v[i][j];
        }
    }

    return res;
}

std::vector<std::vector<int>> Bot::GetProfit(bool color) {
    std::vector<std::vector<int>> profit_matrix(8, std::vector<int>(8, 0));

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (!im_field_[i][j])
                continue;
            int res = -5;

            if (im_field_[i][j] < 0 && i == 0 || im_field_[i][j] > 0 && i == 7)
                res -= 1;
            if (abs(im_field_[i][j]) == 2)
                res -= 10;

//            if (CanBeEaten(i, j, color)) {
//                res += 20;
//                if (abs(im_field_[i][j]) == 2)
//                    res += 20;
//            }
//            res -= 2 * GetImEats(i, j).size();

            if (i > 0 && j < 7 && (im_field_[i - 1][j + 1] > 0) == color)
                res -= 2;
            if (i > 0 && j > 0 && (im_field_[i - 1][j + 1] > 0) == color)
                res -= 2;
            if (i < 7 && j < 7 && (im_field_[i + 1][j + 1] > 0) == color)
                res -= 2;
            if (i < 7 && j > 0 && (im_field_[i + 1][j - 1] > 0) == color)
                res -= 2;

            if (i > 0 && j < 7 && (im_field_[i - 1][j + 1] > 0) != color)
                res += 2;
            if (i > 0 && j > 0 && (im_field_[i - 1][j + 1] > 0) != color)
                res += 2;
            if (i < 7 && j < 7 && (im_field_[i + 1][j + 1] > 0) != color)
                res += 2;
            if (i < 7 && j > 0 && (im_field_[i + 1][j - 1] > 0) != color)
                res += 2;

            profit_matrix[i][j] = res * (color == im_field_[i][j] > 0 ? 1 : -1);
        }
    }

    return profit_matrix;
}

std::vector<std::pair<sf::Vector2i, sf::Vector2i>> Bot::GetAllMoves(bool color) {
    std::vector<sf::Vector2i> can_eat, can_move;
    std::vector<std::vector<sf::Vector2i>> eats, moves;

    if (im_eat_again_.x == -1) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (color == (im_field_[i][j] > 0)) {
                    if (!GetImEats(i, j).empty()) {
                        eats.push_back(GetImEats(i, j));
                        can_eat.push_back({i, j});
                    } else if (!GetImMoves(i, j).empty()) {
                        moves.push_back(GetImMoves(i, j));
                        can_move.push_back({i, j});
                    };
                }
            }
        }
    } else {
        can_eat.push_back(im_eat_again_);
        eats.push_back(GetImEats(im_eat_again_));
    }

    std::vector<std::pair<sf::Vector2i, sf::Vector2i>> res;
    if (!can_eat.empty()) {
        for (int i = 0; i < can_eat.size(); ++i) {
            for (auto e: eats[i]) {
                res.push_back({can_eat[i], e});
            }
        }
    } else {
        for (int i = 0; i < can_move.size(); ++i) {
            for (auto e: moves[i]) {
                res.push_back({can_move[i], e});
            }
        }
    }
    return res;
}

std::pair<sf::Vector2i, sf::Vector2i> Bot::CalculateBestMove(GameManager &game_manager,
                                                             bool color,
                                                             int depth,
                                                             int max_depth) {
//    if (!depth)
//        std::cout << '\n';
    if (depth >= max_depth || game_manager.GameOver()) {
//        std::cout << "BRUH\n";
        return {{0, 0}, {0, 0}};
    }
//    if (color)
//        std::cout << "";

    sf::Vector2i best_from = {-1, -1}, best_to = {-1, -1};
    if (!color)
        best_from = best_to = {-2, -2};
    bool found = false;

    auto base_field = im_field_;
    if (!depth)
        base_field = game_manager.GetField();
    im_field_ = base_field;

    auto moves = GetAllMoves(color);
    if (moves.empty()) {
//        std::cout << (color ? "W":"B") << "\n";
        return {best_from, best_to};
    }

    int base_profit = cur_profit_;
    if (!depth)
        base_profit = 0;
    cur_profit_ = base_profit;
    int max = base_profit - 1;

    sf::Vector2i base_again = im_eat_again_;
    if (!depth)
        base_again = game_manager.EatAgainPos();
    im_eat_again_ = base_again;

    auto bst = im_field_;

    for (auto m: moves) {
        sf::Vector2i from = m.first, to = m.second;
        if (from == to) {
//            std::cout << "WTF\n";
        }

        cur_profit_ = base_profit;
        im_field_ = base_field;
        im_eat_again_ = base_again;

        ImMove(from, to);

        std::vector<std::vector<int>> profit_matrix;
//        profit_matrix = GetProfit(color);
//        cur_profit_ += MatrixSum(profit_matrix);

        while (im_eat_again_.x != -1) {
            cur_profit_ -= 40;
            auto next = GetImEats(im_eat_again_.x, im_eat_again_.y)[0];
            ImMove(im_eat_again_, next);
        }

        auto best_move = CalculateBestMove(game_manager, !color, depth + 1, max_depth);
        if (best_move.first.x == -1 && color || best_move.first.x == -2 && !color) {
            cur_profit_ = 1e9;
            best_from = from;
            best_to = to;
            break;
        }
        if (best_move.first.x == -2 && color || best_move.first.x == -1 && !color) {
            cur_profit_ = -1e9;
        } else if (best_move.first != best_move.second){
            ImMove(best_move);
            profit_matrix = GetProfit(color);
            cur_profit_ += MatrixSum(profit_matrix);
        }

        if (cur_profit_ > max && from != to || !found) {
            found = true;
            bst = im_field_;

            max = cur_profit_;
            best_from = from;
            best_to = to;
        }

    }

//    std::cout << (color ? "WHITE":"BLACK") << '\n';
//    std::cout << best_from.x << ' ' << best_from.y << " --> " << best_to.x << ' ' << best_to.y
//              << '\n';

//    std::cout << max << '\n';
//    for (int i = 0; i < 8; ++i) {
//        for (int j = 0; j < 8; ++j) {
//            std::cout << bst[i][j] << ' ';
//        }
//        std::cout << '\n';
//    }
//    std::cout << '\n';
//}

    cur_profit_ = base_profit;
    im_field_ = base_field;

    return {best_from, best_to};
}

std::pair<sf::Vector2i, sf::Vector2i> Bot::CalculateRandomMove(GameManager &game_manager) {
    if (game_manager.GameOver()) {
        std::cout << "NIGGER\n";
        return {{0, 0}, {0, 0}};
    }

    im_field_ = game_manager.GetField();
    auto moves = GetAllMoves(white_);

    dis = std::uniform_int_distribution<>(0, moves.size() - 1);

    int ind = dis(gen);

//    std::cout << moves[ind].first.x << ' ' << moves[ind].first.y << " -------> "
//              << moves[ind].second.x << ' ' << moves[ind].second.y << '\n';

    return moves[ind];
}

void Bot::ImMove(std::pair<sf::Vector2i, sf::Vector2i> pos) {
    ImMove(pos.first, pos.second);
}
void Bot::ImMove(sf::Vector2i from, sf::Vector2i to) {
    im_eat_again_ = {-1, -1};

    std::swap(im_field_[from.x][from.y], im_field_[to.x][to.y]);

    if (to.x == 0 && im_field_[to.x][to.y] < 0 || to.x == 7 && im_field_[to.x][to.y] > 0)
        im_field_[to.x][to.y] = (im_field_[to.x][to.y] > 0 ? 2 : -2);

    if (abs(to.x - from.x) > 1 || abs(to.y - from.y) > 1) {
        std::vector<int> eat_i, eat_j;
        if (to.x > from.x) {
            for (int x = from.x + 1; x < to.x; ++x)
                eat_i.push_back(x);
        } else {
            for (int x = from.x - 1; x > to.x; --x)
                eat_i.push_back(x);
        }
        if (to.y > from.y) {
            for (int x = from.y + 1; x < to.y; ++x)
                eat_j.push_back(x);
        } else {
            for (int x = from.y - 1; x > to.y; --x)
                eat_j.push_back(x);
        }

        int i = 0;
        bool eaten = false;
        while (i < eat_i.size() && i < eat_j.size()) {
            if (im_field_[eat_i[i]][eat_j[i]]) {
                im_field_[eat_i[i]][eat_j[i]] = 0;
                eaten = true;
                break;
            }
            ++i;
        }

        auto eats = GetImEats(to);
        if (!eats.empty() && eaten) {
            im_eat_again_ = to;
        }
    }
}

std::vector<sf::Vector2i> Bot::GetImEats(sf::Vector2i pos) {
    return GetImEats(pos.x, pos.y);
}
std::vector<sf::Vector2i> Bot::GetImEats(int i, int j) {
    std::vector<sf::Vector2i> moves;
    if (!im_field_[i][j])
        return moves;
    bool white = im_field_[i][j] > 0;

    if (abs(im_field_[i][j]) == 1) {
        if (i > 1 && j > 1
            && (white && im_field_[i - 1][j - 1] < 0 || !white && im_field_[i - 1][j - 1] > 0)
            && !im_field_[i - 2][j - 2]) {
            moves.push_back({i - 2, j - 2});

        }
        if (i > 1 && j < 6
            && (white && im_field_[i - 1][j + 1] < 0 || !white && im_field_[i - 1][j + 1] > 0)
            && !im_field_[i - 2][j + 2]) {
            moves.push_back({i - 2, j + 2});
        }

        if (i < 6 && j > 1
            && (white && im_field_[i + 1][j - 1] < 0 || !white && im_field_[i + 1][j - 1] > 0)
            && !im_field_[i + 2][j - 2]) {
            moves.push_back({i + 2, j - 2});

        }
        if (i < 6 && j < 6
            && (white && im_field_[i + 1][j + 1] < 0 || !white && im_field_[i + 1][j + 1] > 0)
            && !im_field_[i + 2][j + 2]) {
            moves.push_back({i + 2, j + 2});
        }
    } else if (abs(im_field_[i][j]) == 2) {

        bool go = true;
        bool eating = false;
        int x = i + 1, y = j + 1;
        while (go && x <= 7 && y <= 7) {
            if (im_field_[x][y] && x < 7 && y < 7) {
                if (eating) {
                    go = false;
                    break;
                }
                if (im_field_[x][y] > 0 && white || im_field_[x][y] < 0 && !white
                    || im_field_[x + 1][y + 1]) {
                    go = false;
                    break;
                }
                moves.push_back({x + 1, y + 1});
                eating = true;
            } else if (!im_field_[x][y] && eating) {
                moves.push_back({x, y});
            }

            ++x;
            ++y;
        }

        go = true;
        eating = false;
        x = i + 1;
        y = j - 1;
        while (go && x <= 7 && y >= 0) {
            if (im_field_[x][y] && x < 7 && y > 0) {
                if (eating) {
                    go = false;
                    break;
                }
                if (im_field_[x][y] > 0 && white || im_field_[x][y] < 0 && !white
                    || im_field_[x + 1][y - 1]) {
                    go = false;
                    break;
                }
                moves.push_back({x + 1, y - 1});
                eating = true;
            } else if (!im_field_[x][y] && eating) {
                moves.push_back({x, y});
            }
            ++x;
            --y;
        }

        go = true;
        eating = false;
        x = i - 1;
        y = j - 1;
        while (go && x >= 0 && y >= 0) {
            if (im_field_[x][y] && x > 0 && y > 0) {
                if (eating) {
                    go = false;
                    break;
                }
                if (im_field_[x][y] > 0 && white || im_field_[x][y] < 0 && !white
                    || im_field_[x - 1][y - 1]) {
                    go = false;
                    break;
                }
                moves.push_back({x - 1, y - 1});
                eating = true;
            } else if (!im_field_[x][y] && eating) {
                moves.push_back({x, y});
            }
            --x;
            --y;
        }

        go = true;
        eating = false;
        x = i - 1;
        y = j + 1;
        while (go && x >= 0 && y <= 7) {
            if (im_field_[x][y] && x > 0 && y < 7) {
                if (eating) {
                    go = false;
                    break;
                }
                if (im_field_[x][y] > 0 && white || im_field_[x][y] < 0 && !white
                    || im_field_[x - 1][y + 1]) {
                    go = false;
                    break;
                }
                moves.push_back({x - 1, y + 1});
                eating = true;
            } else if (!im_field_[x][y] && eating) {
                moves.push_back({x, y});
            }
            --x;
            ++y;
        }

    }
}

std::vector<sf::Vector2i> Bot::GetImMoves(sf::Vector2i pos) {
    return GetImMoves(pos.x, pos.y);
}
std::vector<sf::Vector2i> Bot::GetImMoves(int i, int j) {
    std::vector<sf::Vector2i> moves;
    if (!im_field_[i][j])
        return moves;
    bool white = im_field_[i][j] > 0;

    if (im_field_[i][j] == 1) {

        if (i > 0 && j > 0 && !im_field_[i - 1][j - 1]) {
            moves.push_back({i - 1, j - 1});
        }

        if (i > 0 && j < 7 && !im_field_[i - 1][j + 1]) {
            moves.push_back({i - 1, j + 1});
        }

    } else if (im_field_[i][j] == -1) {

        if (i < 7 && j > 0 && !im_field_[i + 1][j - 1]) {
            moves.push_back({i + 1, j - 1});
        }

        if (i < 7 && j < 7 && !im_field_[i + 1][j + 1]) {
            moves.push_back({i + 1, j + 1});
        }

    } else if (abs(im_field_[i][j]) == 2) {

        bool go = true;
        int x = i + 1, y = j + 1;
        while (go && x < 8 && y < 8) {
            if (!im_field_[x][y]) {
                moves.push_back({x, y});
            } else {
                go = false;
                break;
            }
            ++x;
            ++y;
        }

        go = true;
        x = i + 1;
        y = j - 1;
        while (go && x < 8 && y >= 0) {
            if (!im_field_[x][y]) {
                moves.push_back({x, y});
            } else {
                go = false;
                break;
            }
            ++x;
            --y;
        }

        go = true;
        x = i - 1;
        y = j - 1;
        while (go && x >= 0 && y >= 0) {
            if (!im_field_[x][y]) {
                moves.push_back({x, y});
            } else {
                go = false;
                break;
            }
            --x;
            --y;
        }

        go = true;
        x = i - 1;
        y = j + 1;
        while (go && x >= 0 && y < 8) {
            if (!im_field_[x][y]) {
                moves.push_back({x, y});
            } else {
                go = false;
                break;
            }
            --x;
            ++y;
        }

    }
}

bool Bot::CanBeEaten(int i, int j, bool color) {
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
//            if (color != (im_field_[x][y] > 0)) {
            auto eats = GetImEats(x, y);
            for (auto e: eats)
                if (e.x == i && e.y == j)
                    return true;
//            }
        }
    }
    return false;
}