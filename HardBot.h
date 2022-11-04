#ifndef GAME_WITH_AI__HARDBOT_H_
#define GAME_WITH_AI__HARDBOT_H_

class Bot;
#include "Bot.h"

class HardBot : public Bot {
 public:
  std::pair<sf::Vector2i, sf::Vector2i> CalculateMove(GameManager &game_manager) override;
};

#endif
