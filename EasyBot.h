#ifndef GAME_WITH_AI__EASYBOT_H_
#define GAME_WITH_AI__EASYBOT_H_

class Bot;
#include "Bot.h"
#include <random>
#include <ctime>

class EasyBot : public Bot {
 public:
  std::pair<sf::Vector2i, sf::Vector2i> CalculateMove(GameManager &game_manager) override;
};

#endif
