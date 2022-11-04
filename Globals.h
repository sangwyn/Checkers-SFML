#ifndef GAME_WITH_AI__GLOBALS_H_
#define GAME_WITH_AI__GLOBALS_H_

#include "SFML/Graphics.hpp"

const sf::Vector2u kResolution = {1600, 900};

const float kCellSize = 100;

const sf::Vector2f kFieldPos = {400, 50};

const sf::Color kCellOutlineColor = sf::Color::Black;
const sf::Color kWhiteCellColor = sf::Color(100, 100, 100);
const sf::Color kBlackCellColor = sf::Color(57, 57, 57);

const sf::Color kWhiteCheckerColor = sf::Color::White;
const sf::Color kBlackCheckerColor = sf::Color::Black;

const sf::Color kBackgroundColor = sf::Color(57, 57, 57);
const sf::Color kUIColor = sf::Color::White;
const sf::Color kUnselectedColor = sf::Color(100, 100, 100);
const sf::Color kSelectedColor = sf::Color::Cyan;

const sf::Color kTextColor = sf::Color::Black;
const sf::Color kUIBackgroundColor = sf::Color(240, 240, 240);

#endif
