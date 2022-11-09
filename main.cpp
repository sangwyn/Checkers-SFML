#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "GameManager.h"
#include "Button.h"
#include "Bot.h"
#include "EasyBot.h"
#include "MediumBot.h"
#include "HardBot.h"

int main() {
    sf::RenderWindow
        window({kResolution.x, kResolution.y}, "Game", sf::Style::Close);

    GameManager game_manager;

    sf::Font font;
    font.loadFromFile("Assets/Hack-Regular.ttf");

    sf::Texture restart_selected, restart_unselected;
    restart_selected.loadFromFile("Assets/restart_selected.png");
    restart_unselected.loadFromFile("Assets/restart_unselected.png");
    Button restart_btn({10, 10}, restart_selected, restart_unselected);

    Button pl_btn({10, 210}, font, "vs. Player");
    Button ebot_btn({10, 260}, font, "Easy");
    Button mbot_btn({10, 310}, font, "Medium");
    Button hbot_btn({10, 360}, font, "Hard");
    pl_btn.SetTextSelection(true);

    Button reverse_btn({1300, 10}, font, "Reverse sides");

    std::vector<Bot *> bots(3);
    bots[0] = new EasyBot;
    bots[1] = new MediumBot;
    bots[2] = new HardBot;
    int cur_bot = -1;

    sf::Text cur_bot_text;
    cur_bot_text.setFont(font);
    cur_bot_text.setFillColor(sf::Color::Green);
    cur_bot_text.setString("vs. Player");
    cur_bot_text.setPosition(1400, 10);

    sf::Text gameover_text;
    gameover_text.setFont(font);
    gameover_text.setFillColor(kSelectedColor);
    gameover_text.setCharacterSize(100);
    gameover_text.setOutlineThickness(2);
    gameover_text.setOutlineColor(sf::Color::White);

    bool player_white = true;

    while (window.isOpen()) {
        bool game_over = game_manager.GameOver();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonReleased) {
                if (restart_btn.Selected()) {
                    game_manager.NewGame();
                } else if (reverse_btn.Selected()) {
                    player_white = !player_white;
                    for (int i = 0; i < 3; ++i)
                        bots[i]->ChangeColor();
                    game_manager.NewGame();
                } else if (pl_btn.Selected() && cur_bot != -1) {
                    ebot_btn.SetTextSelection(false);
                    mbot_btn.SetTextSelection(false);
                    hbot_btn.SetTextSelection(false);
                    pl_btn.SetTextSelection(true);
                    game_manager.NewGame();
                    cur_bot = -1;
                    cur_bot_text.setString("vs. Player");
                } else if (ebot_btn.Selected() && cur_bot != 0) {
                    ebot_btn.SetTextSelection(true);
                    mbot_btn.SetTextSelection(false);
                    hbot_btn.SetTextSelection(false);
                    pl_btn.SetTextSelection(false);
                    game_manager.NewGame();
                    cur_bot = 0;
                    cur_bot_text.setString("Easy");
                } else if (mbot_btn.Selected() && cur_bot != 1) {
                    ebot_btn.SetTextSelection(false);
                    mbot_btn.SetTextSelection(true);
                    hbot_btn.SetTextSelection(false);
                    pl_btn.SetTextSelection(false);
                    game_manager.NewGame();
                    cur_bot = 1;
                    cur_bot_text.setString("Medium");
                } else if (hbot_btn.Selected() && cur_bot != 2) {
                    ebot_btn.SetTextSelection(false);
                    mbot_btn.SetTextSelection(false);
                    hbot_btn.SetTextSelection(true);
                    pl_btn.SetTextSelection(false);
                    game_manager.NewGame();
                    cur_bot = 2;
                    cur_bot_text.setString("Hard");
                } else if (!game_over
                    && (cur_bot == -1 || player_white == game_manager.WhoseTurn())) {
                    game_manager.HandleClick((sf::Vector2f) sf::Mouse::getPosition(
                        window));
                }
            }
        }

        restart_btn.SetSelection(restart_btn.MouseOver((sf::Vector2f) sf::Mouse::getPosition(window)));
        pl_btn.SetSelection(pl_btn.MouseOver((sf::Vector2f) sf::Mouse::getPosition(window)));
        ebot_btn.SetSelection(ebot_btn.MouseOver((sf::Vector2f) sf::Mouse::getPosition(window)));
        mbot_btn.SetSelection(mbot_btn.MouseOver((sf::Vector2f) sf::Mouse::getPosition(window)));
        hbot_btn.SetSelection(hbot_btn.MouseOver((sf::Vector2f) sf::Mouse::getPosition(window)));
        if (cur_bot != -1)
            reverse_btn.SetSelection(reverse_btn.MouseOver((sf::Vector2f) sf::Mouse::getPosition(
                window)));

        window.clear(kBackgroundColor);

        game_manager.DrawField(window);

        restart_btn.Draw(window);
        pl_btn.Draw(window);
        ebot_btn.Draw(window);
        mbot_btn.Draw(window);
        hbot_btn.Draw(window);
        if (cur_bot != -1)
            reverse_btn.Draw(window);

//        window.draw(cur_bot_text);

        if (game_over) {
            if (game_manager.WhiteWin()) {
                gameover_text.setString("WHITE WIN!");
            } else {
                gameover_text.setString("BLACK WIN!");
            }
            gameover_text.setOrigin(gameover_text.getGlobalBounds().width / 2,
                                    gameover_text.getGlobalBounds().height / 2);
            gameover_text.setPosition(800, 450);
            window.draw(gameover_text);
        }

        window.display();

//        if (player_white == game_manager.WhoseTurn() && cur_bot != -1) {
//            player_white = !player_white;
//            bots[cur_bot]->ChangeColor();
//        }
        if (!game_over && cur_bot != -1 && player_white != game_manager.WhoseTurn()) {
            sf::sleep(sf::seconds(0.3f));
            game_manager.Move(bots[cur_bot]->CalculateMove(game_manager));
        }
    }

    return 0;
}