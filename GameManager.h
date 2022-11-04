#ifndef GAME_WITH_AI__GAMEMANAGER_H_
#define GAME_WITH_AI__GAMEMANAGER_H_

#include <SFML/Graphics.hpp>
#include <utility>
#include "Globals.h"
//#include "Bot.h"
//#include "EasyBot.h"
//#include "MediumBot.h"
//#include "HardBot.h"

class GameManager {
 public:
  GameManager();
  ~GameManager();

  bool WhoseTurn() { return white_turn_; }
  bool GameOver() { return game_over_; }
  bool WhiteWin();
  bool CanEat(int i, int j) {return can_eat_[i][j]; }
  std::vector<std::vector<int>> GetField() { return field_; }
  sf::Vector2i EatAgainPos() { return (eat_again_ ? eat_again_pos_ : sf::Vector2i {-1, -1}); }

  // -1 = player, 0 = easy, 1 = medium, 2 = hard
//  void SetDifficulty(int difficulty) { cur_bot_ = difficulty; };

  void NewGame();

//  void Reverse();

  void HandleClick(sf::Vector2f mouse_pos);

  std::vector<sf::Vector2i> GetPossibleMoves(sf::Vector2i pos);
  std::vector<sf::Vector2i> GetPossibleMoves(int i, int j);

  std::vector<sf::Vector2i> GetPossibleEats(sf::Vector2i pos, bool all = false);
  std::vector<sf::Vector2i> GetPossibleEats(int i, int j, bool all = false);

  void Move(int from_i, int from_j, int to_i, int to_j);
  void Move(sf::Vector2i from, sf::Vector2i to);
  void Move(std::pair<sf::Vector2i, sf::Vector2i> move);

  void DrawField(sf::RenderWindow &window);
 private:
  bool white_turn_ = true;

  void Reset();

  std::vector<std::vector<int>> field_; // 1 - white, -1 - black, 2 and -2 - king

  std::vector<std::vector<bool>> selected_;
  std::vector<std::vector<bool>> can_eat_;
  std::vector<std::vector<sf::RectangleShape>> field_sprite_;
  sf::CircleShape checker_sprite_;
  std::vector<sf::Vector2f> crown_pos_;
  sf::Texture crown_texture_;
  sf::Sprite crown_sprite_;
  sf::CircleShape selected_sprite_;

//  std::vector<Bot*> bots_;
//  int cur_bot_;

  sf::Text cur_turn_text_;
  sf::Font font_;

  bool game_over_ = false;
  bool eat_again_ = false;
  sf::Vector2i eat_again_pos_;

//  int coeff = 1;
};

#endif
