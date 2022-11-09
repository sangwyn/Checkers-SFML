#ifndef GAME_WITH_AI__BOT_H_
#define GAME_WITH_AI__BOT_H_

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

  int cur_profit_;
  virtual std::pair<sf::Vector2i, sf::Vector2i> CalculateBestMove(GameManager &game_manager,
                                                                  bool color,
                                                                  int depth,
                                                                  int max_depth);
  virtual std::pair<sf::Vector2i, sf::Vector2i> CalculateRandomMove(GameManager &game_manager);

  std::mt19937 gen = std::mt19937(time(nullptr));
  std::uniform_int_distribution<> dis;
  virtual int GetMinEats(int res, int i, int j, int depth = 0);
  virtual int GetMaxEats(int res, int i, int j, int depth = 0);

  virtual int MatrixSum(std::vector<std::vector<int>> v);
  virtual std::vector<std::vector<int>> GetProfit(bool color);

  bool CanBeEaten(int i, int j, bool color);

  virtual std::vector<std::pair<sf::Vector2i, sf::Vector2i>> GetAllMoves(bool color);

  bool white_ = false;

  std::vector<std::vector<int>> im_field_;
  sf::Vector2i im_eat_again_ = {-1, -1};
  virtual void ImMove(std::pair<sf::Vector2i, sf::Vector2i> pos);
  virtual void ImMove(sf::Vector2i from, sf::Vector2i to);

  std::vector<sf::Vector2i> GetImEats(sf::Vector2i pos);
  std::vector<sf::Vector2i> GetImEats(int i, int j);
  std::vector<sf::Vector2i> GetImMoves(sf::Vector2i pos);
  std::vector<sf::Vector2i> GetImMoves(int i, int j);
};

#endif
