#ifndef GAME_WITH_AI__BUTTON_H_
#define GAME_WITH_AI__BUTTON_H_

#include <SFML/Graphics.hpp>

class Button {
 public:
  Button(sf::Vector2f pos, sf::Font font, sf::String text);
  Button(sf::Vector2f pos, sf::Texture &selected, sf::Texture &unselected);
  bool MouseOver(sf::Vector2f mouse_pos);
  void SetSelection(bool select);
  bool Selected();
  void Draw(sf::RenderWindow &window);
  float GetWidth();
  float GetHeight();
  void SetTextSelection(bool select);
  sf::Vector2f GetPos();
  void SetPos(sf::Vector2f pos);
 private:
  std::string type;
  sf::Font font;
  sf::RectangleShape rect;
  sf::Text text_inside;
  bool is_selected = false;
  bool text_selected = false;
  sf::Vector2f pos;
  sf::Texture selected, unselected;
  sf::Sprite sprite;
};

#endif
