#ifndef GAME_WITH_AI__BOT_H_
#define GAME_WITH_AI__BOT_H_

#include "SearchTree.h"
#include "GameManager.h"
#include <utility>
#include <SFML/Graphics.hpp>
#include <random>

class Bot {
 public:
  void ChangeColor() { white_ = !white_; }
  void SetColor(bool white) { white_ = white; }
  virtual std::pair<sf::Vector2i, sf::Vector2i> CalculateMove(GameManager &game_manager) = 0;
 protected:
  std::mt19937 gen = std::mt19937(time(nullptr));
  std::uniform_int_distribution<> dis;
  virtual int GetMinEats(int res, int i, int j, GameManager game_manager);
  virtual int GetMaxEats(int res, int i, int j, GameManager game_manager);
  virtual int GetProfit(int i, int j, GameManager game_manager);

  virtual void FillVectors(std::vector<sf::Vector2i> &can_move,
                           std::vector<std::vector<sf::Vector2i>> &moves,
                           std::vector<sf::Vector2i> &can_eat,
                           std::vector<std::vector<sf::Vector2i>> &eats,
                           GameManager game_manager);

  bool white_ = false;
  SearchTree search_tree_;
};

#endif
