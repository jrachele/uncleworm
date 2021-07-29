#include "worm.h"
#include "apples.h"
#include "state.h"

#include <iostream>
#include <SFML/Graphics.hpp>

#include "global.h"

int main() {

  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Uncle Worm");
  int pixelSize = WIDTH / 320;
  sf::RectangleShape background({WIDTH, HEIGHT});
  background.setFillColor(screen);


  State state;
  sf::Clock clock;
  while (window.isOpen()) {

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      state.get_worm()->turn(1);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      state.get_worm()->turn(-1);
    }

    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
        {
          window.close();
          break;
        }
        case sf::Event::KeyPressed:
        {
          if (event.key.code == sf::Keyboard::Space) {
            if (state.get_current_level()->number() == INTRO) {
              Level *l1 = new Gameplay(1, 5);
              state.change_level(l1);
            }
            if (state.get_current_level()->number() == GAME_OVER) {
              exit(0);
            }

          }
        }
        default:
          break;
      }
    }
    int lvl = state.get_current_level()->number();
    if (lvl != INTRO && lvl != GAME_OVER) {
      state.update();
    }

    window.clear();
    window.draw(background);
    window.draw(state);

    window.display();

    sf::Time elapsed = clock.restart();
    while (elapsed.asMilliseconds() < 33) {
      elapsed = clock.getElapsedTime();
    }
  }


  return 0;
}
