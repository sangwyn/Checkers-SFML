#ifndef GAME_WITH_AI__MEDIUMBOT_H_
#define GAME_WITH_AI__MEDIUMBOT_H_

class Bot;
#include "Bot.h"

class MediumBot : public Bot {
 public:
  std::pair<sf::Vector2i, sf::Vector2i> CalculateMove(GameManager &game_manager) override;
};

#endif
