#include "GameManager.h"
#include <iostream>

GameManager::GameManager() {
    font_.loadFromFile("Assets/Hack-Regular.ttf");
    cur_turn_text_.setFont(font_);
    cur_turn_text_.setColor(kSelectedColor);
    cur_turn_text_.setPosition({10, 100});

    field_.assign(8, std::vector<int>(8, 0));

    Reset();

    sf::RectangleShape square({kCellSize, kCellSize});
    square.setOutlineThickness(2);
    square.setOutlineColor(kCellOutlineColor);
    field_sprite_.resize(8);
    bool white = true;
    for (int i = 0; i < 8; ++i) {
        field_sprite_[i].resize(8);
        square.setPosition({kFieldPos.x, kFieldPos.y + i * kCellSize});
        for (auto &j: field_sprite_[i]) {
            if (white)
                square.setFillColor(kWhiteCellColor);
            else
                square.setFillColor(kBlackCellColor);

            j = square;

            white = !white;
            square.setPosition({square.getPosition().x + kCellSize,
                                square.getPosition().y});
        }
        white = !white;
    }
    checker_sprite_.setOutlineThickness(2);
    checker_sprite_.setRadius(kCellSize / 2);
    checker_sprite_.setPointCount(5000);

    selected_sprite_.setRadius(kCellSize / 8);
    selected_sprite_.setOutlineColor(kCellOutlineColor);
    selected_sprite_.setOutlineThickness(1);
    selected_sprite_.setFillColor(kSelectedColor);
    selected_sprite_.setOrigin({selected_sprite_.getRadius(),
                                selected_sprite_.getRadius()});
    selected_sprite_.setPointCount(5000);

    crown_texture_.loadFromFile("Assets/crown.png");
    crown_sprite_.setTexture(crown_texture_);
    float scale = (kCellSize / 512) / 2;
    crown_sprite_.setScale(scale, scale);
    crown_sprite_.setOrigin(crown_sprite_.getGlobalBounds().width / 2,
                            crown_sprite_.getGlobalBounds().height / 2);

//    bots_.resize(3);
//    bots_[0] = new EasyBot;
//    bots_[1] = new MediumBot;
//    bots_[2] = new HardBot;
//    cur_bot_ = -1;
}

GameManager::~GameManager() {

}

void GameManager::Reset() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            field_[i][j] = 0;
            if (i < 3 && (i + j) % 2) {
                field_[i][j] = -1;
            } else if (i > 4 && (i + j) % 2) {
                field_[i][j] = 1;
            }
        }
    }
    selected_.assign(8, std::vector<bool>(8, false));
    can_eat_.assign(8, std::vector<bool>(8, false));
    white_turn_ = true;
}

void GameManager::HandleClick(sf::Vector2f mouse_pos) {
    if (mouse_pos.x < kFieldPos.x || mouse_pos.y < kFieldPos.y
        || mouse_pos.x > kFieldPos.x + kCellSize * 8
        || mouse_pos.y > kFieldPos.y + kCellSize * 8) {
        selected_.assign(8, std::vector<bool>(8, false));
        return;
    }

//    for (int x = 0; x < 8; ++x) {
//        for (int y = 0; y < 8; ++y) {
//            if ((white_turn_ && field_[x][y] > 0
//                || !white_turn_ && field_[x][y] < 0)) {
//                auto e = GetPossibleEats(x, y);
//                if (!e.empty()) {
//                    can_eat_[x][y] = true;
//                }
//            }
//        }
//    }

    auto t = sf::Vector2i{2, 2} / 2;
    sf::Vector2i pos = (sf::Vector2i) ((mouse_pos - kFieldPos) / kCellSize);
    int i = pos.y, j = pos.x;
//    std::cout << i << ' ' << j << '\n';
//    for (auto i : selected_) {
//        for (auto j : i)
//            std::cout << (j ? 1 : 0);
//        std::cout << '\n';
//    }
//    std::cout << '\n';
    if (!field_[i][j]) {
        if (!selected_[i][j]) {
            selected_.assign(8, std::vector<bool>(8, false));
            return;
        }
        int start_i = -1, start_j = -1;

        for (int x = 0; x < 8 && start_i == -1; ++x) {
            for (int y = 0; y < 8 && start_i == -1; ++y) {
                if (selected_[x][y] && field_[x][y]) {
                    start_i = x;
                    start_j = y;
                }
            }
        }

        Move(start_i, start_j, i, j);
        selected_.assign(8, std::vector<bool>(8, false));
//        white_turn_ = !white_turn_;

//        can_eat_.assign(8, std::vector<bool>(8, false));
//        for (int x = 0; x < 8; ++x) {
//            for (int y = 0; y < 8; ++y) {
//                if ((white_turn_ && field_[x][y] > 0
//                    || !white_turn_ && field_[x][y] < 0)) {
//                    auto e = GetPossibleEats(x, y);
//                    if (!e.empty()) {
//                        can_eat_[x][y] = true;
//                    }
//                }
//            }
//        }

        return;
    }
    if (selected_[i][j] || white_turn_ && field_[i][j] < 0
        || !white_turn_ && field_[i][j] > 0) {
        selected_.assign(8, std::vector<bool>(8, false));
        return;
    }
    bool go = true, can = true;
    if (!can_eat_[i][j]) {
        for (int x = 0; x < 8 && go; ++x) {
            for (int y = 0; y < 8 && go; ++y) {
                if ((white_turn_ && field_[x][y] > 0
                    || !white_turn_ && field_[x][y] < 0)
                    && !(x == i && y == j)) {
                    if (can_eat_[x][y]) {
                        can = false;
                        go = false;
                        break;
                    }
                }
            }
        }
    }

    selected_.assign(8, std::vector<bool>(8, false));
    if (!can)
        return;

    selected_[i][j] = true;
    auto eats = GetPossibleEats(i, j, eat_again_);
    auto moves = GetPossibleMoves(i, j);
    if (eats.empty()) {
        for (auto m: moves) {
            selected_[m.x][m.y] = true;
        }
    }
    for (auto m: eats) {
        selected_[m.x][m.y] = true;
    }
}

std::vector<sf::Vector2i> GameManager::GetPossibleMoves(sf::Vector2i pos) {
    return GetPossibleMoves(pos.x, pos.y);
}
std::vector<sf::Vector2i> GameManager::GetPossibleMoves(int i, int j) {
    std::vector<sf::Vector2i> moves;
    if (!field_[i][j])
        return moves;
    bool white = field_[i][j] > 0;

    if (field_[i][j] == 1) {

        if (i > 0 && j > 0 && !field_[i - 1][j - 1]) {
            moves.push_back({i - 1, j - 1});
        }

        if (i > 0 && j < 7 && !field_[i - 1][j + 1]) {
            moves.push_back({i - 1, j + 1});
        }

    } else if (field_[i][j] == -1) {

        if (i < 7 && j > 0 && !field_[i + 1][j - 1]) {
            moves.push_back({i + 1, j - 1});
        }

        if (i < 7 && j < 7 && !field_[i + 1][j + 1]) {
            moves.push_back({i + 1, j + 1});
        }

    } else if (abs(field_[i][j]) == 2) {

        bool go = true;
        int x = i + 1, y = j + 1;
        while (go && x < 8 && y < 8) {
            if (!field_[x][y]) {
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
            if (!field_[x][y]) {
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
            if (!field_[x][y]) {
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
            if (!field_[x][y]) {
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

std::vector<sf::Vector2i> GameManager::GetPossibleEats(sf::Vector2i pos, bool all) {
    return GetPossibleEats(pos.x, pos.y, all);
}
std::vector<sf::Vector2i> GameManager::GetPossibleEats(int i, int j, bool all) {
//    for (int i = 0; i < 8; ++i) {
//        for (int j = 0; j < 8; ++j) {
//            std::cout << field_[i][j] << ' ';
//        }
//        std::cout << '\n';
//    }
//    std::cout << "\n\n";
    std::vector<sf::Vector2i> moves;
    if (!field_[i][j])
        return moves;
    bool white = field_[i][j] > 0;

    if (abs(field_[i][j]) == 1) {
//        if (field_[i][j] == 1 || all) {
        if (i > 1 && j > 1
            && (white && field_[i - 1][j - 1] < 0 || !white && field_[i - 1][j - 1] > 0)
            && !field_[i - 2][j - 2]) {
            moves.push_back({i - 2, j - 2});

        }
        if (i > 1 && j < 6
            && (white && field_[i - 1][j + 1] < 0 || !white && field_[i - 1][j + 1] > 0)
            && !field_[i - 2][j + 2]) {
            moves.push_back({i - 2, j + 2});
        }

//        }
//        if (field_[i][j] == -1 || all) {
        if (i < 6 && j > 1
            && (white && field_[i + 1][j - 1] < 0 || !white && field_[i + 1][j - 1] > 0)
            && !field_[i + 2][j - 2]) {
            moves.push_back({i + 2, j - 2});

        }
        if (i < 6 && j < 6
            && (white && field_[i + 1][j + 1] < 0 || !white && field_[i + 1][j + 1] > 0)
            && !field_[i + 2][j + 2]) {
            moves.push_back({i + 2, j + 2});
        }

//        }
    } else if (abs(field_[i][j]) == 2) {

        bool go = true;
        bool eating = false;
        int x = i + 1, y = j + 1;
        while (go && x <= 7 && y <= 7) {
            if (field_[x][y] && x < 7 && y < 7) {
                if (eating) {
                    go = false;
                    break;
                }
                if (field_[x][y] > 0 && white || field_[x][y] < 0 && !white
                    || field_[x + 1][y + 1]) {
                    go = false;
                    break;
                }
                moves.push_back({x + 1, y + 1});
                eating = true;
            } else if (!field_[x][y] && eating) {
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
            if (field_[x][y] && x < 7 && y > 0) {
                if (eating) {
                    go = false;
                    break;
                }
                if (field_[x][y] > 0 && white || field_[x][y] < 0 && !white
                    || field_[x + 1][y - 1]) {
                    go = false;
                    break;
                }
                moves.push_back({x + 1, y - 1});
                eating = true;
            } else if (!field_[x][y] && eating) {
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
            if (field_[x][y] && x > 0 && y > 0) {
                if (eating) {
                    go = false;
                    break;
                }
                if (field_[x][y] > 0 && white || field_[x][y] < 0 && !white
                    || field_[x - 1][y - 1]) {
                    go = false;
                    break;
                }
                moves.push_back({x - 1, y - 1});
                eating = true;
            } else if (!field_[x][y] && eating) {
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
            if (field_[x][y] && x > 0 && y < 7) {
                if (eating) {
                    go = false;
                    break;
                }
                if (field_[x][y] > 0 && white || field_[x][y] < 0 && !white
                    || field_[x - 1][y + 1]) {
                    go = false;
                    break;
                }
                moves.push_back({x - 1, y + 1});
                eating = true;
            } else if (!field_[x][y] && eating) {
                moves.push_back({x, y});
            }
            --x;
            ++y;
        }

    }
}

void GameManager::Move(std::pair<sf::Vector2i, sf::Vector2i> move) {
    Move(move.first, move.second);
}
void GameManager::Move(sf::Vector2i from, sf::Vector2i to) {
    Move(from.x, from.y, to.x, to.y);
}
void GameManager::Move(int from_i, int from_j, int to_i, int to_j) {
    eat_again_ = false;
    eat_again_pos_ = {-1, -1};
    white_turn_ = !white_turn_;

    if (from_i == to_i && from_j == to_j)
        return;
//    std::cout << from_i << ' ' << from_j << ' ' << to_i << ' ' << to_j << '\n';

    std::swap(field_[from_i][from_j], field_[to_i][to_j]);
    if (to_i == 0 && field_[to_i][to_j] > 0 || to_i == 7 && field_[to_i][to_j] < 0)
        field_[to_i][to_j] = (field_[to_i][to_j] > 0 ? 2 : -2);

    if (abs(to_i - from_i) > 1 || abs(to_j - from_j) > 1) {
//        std::cout << from_i << ' ' << from_j << ' ' << to_i << ' ' << to_j << '\n';

        std::vector<int> eat_i, eat_j;
        if (to_i > from_i) {
            for (int x = from_i + 1; x < to_i; ++x)
                eat_i.push_back(x);
        } else {
            for (int x = from_i - 1; x > to_i; --x)
                eat_i.push_back(x);
        }
        if (to_j > from_j) {
            for (int x = from_j + 1; x < to_j; ++x)
                eat_j.push_back(x);
        } else {
            for (int x = from_j - 1; x > to_j; --x)
                eat_j.push_back(x);
        }

        int i = 0;
        bool eaten = false;
        while (i < eat_i.size() && i < eat_j.size()) {
//            std::cout << eat_i[i] << ' ' << ea
            if (field_[eat_i[i]][eat_j[i]]) {
                field_[eat_i[i]][eat_j[i]] = 0;
                eaten = true;
                break;
            }
            ++i;
        }

//        for (int i = 0; i < 8; ++i) {
//            for (int j = 0; j < 8; ++j) {
//                std::cout << field_[i][j] << ' ';
//            }
//            std::cout << '\n';
//        }
//        std::cout << "\n\n";
        auto eats = GetPossibleEats(to_i, to_j, true);
        if (!eats.empty() && eaten) {
//            Move(to_i, to_j, eats_[0].x, eats_[0].y);
            white_turn_ = !white_turn_;

            can_eat_.assign(8, std::vector<bool>(8, false));
            can_eat_[to_i][to_j] = true;

            eat_again_ = true;
            eat_again_pos_ = {to_i, to_j};
        }
    }
}

void GameManager::DrawField(sf::RenderWindow &window) {
    int blacks = 0, whites = 0, white_move = 0, black_move = 0;
    if (!eat_again_)
        can_eat_.assign(8, std::vector<bool>(8, false));
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (field_[x][y] > 0) {
                ++whites;

                if (!GetPossibleMoves(x, y).empty() || !GetPossibleEats(x, y).empty())
                    ++white_move;
            } else if (field_[x][y] < 0) {
                ++blacks;

                if (!GetPossibleMoves(x, y).empty() || !GetPossibleEats(x, y).empty())
                    ++black_move;
            }

            if (!eat_again_ && (white_turn_ && field_[x][y] > 0
                || !white_turn_ && field_[x][y] < 0)) {
                auto e = GetPossibleEats(x, y);
                if (!e.empty()) {
                    can_eat_[x][y] = true;
                }
            }
        }
    }
    if (!blacks || !whites || !white_move || !black_move) {
        if (!white_move || !black_move)
            white_turn_ = !white_turn_;
        game_over_ = true;
    }

    for (auto &i: field_sprite_) {
        for (auto &j: i) {
            window.draw(j);
        }
    }
    for (int i = 0; i < 8; ++i) {
        checker_sprite_.setPosition({kFieldPos.x, kFieldPos.y + i * kCellSize});
        for (int j = 0; j < 8; ++j) {
            if (can_eat_[i][j] && !game_over_) {
                checker_sprite_.setOutlineColor(kSelectedColor);
            } else {
                checker_sprite_.setOutlineColor(sf::Color::Transparent);
            }
            if (field_[i][j] > 0) {
                checker_sprite_.setFillColor(kWhiteCheckerColor);
                window.draw(checker_sprite_);
            } else if (field_[i][j] < 0) {
                checker_sprite_.setFillColor(kBlackCheckerColor);
                window.draw(checker_sprite_);
            }

            if (abs(field_[i][j]) == 2) {
                crown_sprite_.setPosition({checker_sprite_.getPosition().x
                                               + kCellSize / 4,
                                           checker_sprite_.getPosition().y
                                               + kCellSize / 4});
                window.draw(crown_sprite_);
            }

            if (selected_[i][j] && !game_over_) {
                selected_sprite_.setPosition({checker_sprite_.getPosition().x
                                                  + kCellSize / 2,
                                              checker_sprite_.getPosition().y
                                                  + kCellSize / 2});
                window.draw(selected_sprite_);
            }


//            sf::Text tet;
//            tet.setString(std::to_string(i) + "," + std::to_string(j));
//            tet.setFont(font_);
//            tet.setPosition(checker_sprite_.getPosition());
//            tet.setFillColor(kSelectedColor);
//            window.draw(tet);


            checker_sprite_.setPosition({checker_sprite_.getPosition().x
                                             + kCellSize,
                                         checker_sprite_.getPosition().y});
        }
    }

    cur_turn_text_.setString(white_turn_ ? "White turn" : "Black turn");
    if (!game_over_)
        window.draw(cur_turn_text_);
//    for (int i = 0; i < 8; ++i) {
//        for (int j = 0; j < 8; ++j) {
//            std::cout << field_[i][j] << ' ';
//        }
//        std::cout << '\n';
//    }
//    std::cout << "\n\n";
}

void GameManager::NewGame() {
    game_over_ = false;
    Reset();
}

bool GameManager::WhiteWin() {
    int whites = 0;
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (field_[x][y] > 0)
                ++whites;
        }
    }
    return !whites;
}

//void GameManager::Reverse() {
//    coeff *= -1;
//    NewGame();
//}
