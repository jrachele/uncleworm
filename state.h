//
// Created by Julian Rachele on 7/27/21.
//

#ifndef UNCLEWORM__STATE_H_
#define UNCLEWORM__STATE_H_
#include <iostream>

#include "level.h"
#include "worm.h"
#include "apples.h"
#include "global.h"


 class State : public sf::Drawable {
 public:
  State() {
    worm = new Worm(8);
    current_level = new Intro();
    if (!font.loadFromFile("font.ttf"))
    {
      // error...
      std::cerr << "It don't work lmao" << std::endl;
    }

  };

  ~State() {
    delete worm;
    delete current_level;
  }
  void check_state() {
    bool worm_ate_itself = worm->check_died();
    bool worm_oob = !worm->has_collided(wall->getGlobalBounds());

    // edge case for when crossing gate
    if (gate && lvl_complete && worm->has_collided(gate->getGlobalBounds())) {
      worm_oob = false;
    }

    // check for when reaching the top of the gate
    if (gate && lvl_complete) {
      sf::RectangleShape portal(*gate);
      auto [w, h] = portal.getSize();
      portal.setSize({w, 20});
      if (worm->has_collided(portal.getGlobalBounds())) {
        // change levels
        change_level(fromInt(current_level->number() + 1));
      }
    }

    if (worm_ate_itself || worm_oob) {
      change_level(new GameOver(score, current_level->number()));
    }
  }

  void change_level(Level* l) {
     if (l == nullptr) return;
     if (current_level)
       delete current_level;
     if (apples) {
       delete apples;
     }
     if (worm) {
       delete worm;
     }
     current_level = l;
     lvl_complete = false;
     apples = new std::vector<Apple>(current_level->apples());
     worm = new Worm(8);
     float thickness = l->number() * THICKNESS + 10;
     wall = new sf::RectangleShape({WIDTH - 2*thickness, HEIGHT - 2*thickness});
     wall->setFillColor(screen);
     wall->setOrigin(WIDTH/2 - thickness, HEIGHT/2 - thickness);
     wall->setPosition(WIDTH/2, HEIGHT/2);

     gate = new sf::RectangleShape({120, thickness});
     gate->setFillColor(screen);
     gate->setOrigin(60, 0);
     gate->setPosition(WIDTH/2, 0);

  }


   void update() {
     if (apples && worm) {
       worm->update();
       if (!apples->empty()) {
         auto& apple = apples->at(0);
         if (worm->getGlobalBounds().intersects(apple.getGlobalBounds())) {
           worm->grow(apple.getSize()*apple.getSize());
           score += apple.getSize();
           apples->erase(apples->begin());
         }

       }
       if (apples->empty()) {
         // open the gate
         lvl_complete = true;
       }
       check_state();
     }
   }

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    // special cases for intro and gameover
    int num = current_level->number();
    if (num == INTRO || num == GAME_OVER) {
      sf::Text title;
      title.setFont(font);
      title.setString(current_level->title());
      title.setCharacterSize(150);
      sf::FloatRect textRect = title.getLocalBounds();
      title.setOrigin(textRect.left + textRect.width/2.0f,
                     textRect.top  + textRect.height/2.0f);
      title.setPosition(sf::Vector2f(WIDTH/2.0f,HEIGHT/2.25f));
      title.setFillColor(sf::Color::Black);
      target.draw(title);

      sf::Text subtitle;
      subtitle.setFont(font);
      std::string s = current_level->subtitle();
      unsigned int characterSize = 75;
      subtitle.setString(s);
      subtitle.setCharacterSize(characterSize);
      sf::FloatRect subRect = subtitle.getLocalBounds();
      subtitle.setOrigin(subRect.left + subRect.width/2.0f,
                      subRect.top  + subRect.height/2.0f);
      subtitle.setPosition(sf::Vector2f(WIDTH/2.0f,HEIGHT/1.75f));
      subtitle.setFillColor(sf::Color::Black);
      target.draw(subtitle);
    } else {
      // handles drawing worm and current level apples
      if (wall) {
        sf::RectangleShape bg({WIDTH, HEIGHT});
        bg.setFillColor(sf::Color::Black);
        target.draw(bg);
        target.draw(*wall);

        float thickness = current_level->number() * THICKNESS + 10;

        // draw the gate if it exists
        if (lvl_complete && gate) {
          target.draw(*gate);
        }

        // also draw level text
        sf::Text t;
        t.setFillColor(screen);
        t.setFont(font);
        t.setString("Level: " + std::to_string(current_level->number()));
        t.setCharacterSize(24);
        t.setPosition(24, 0);
        target.draw(t);

        sf::Text s;
        s.setFillColor(screen);
        s.setFont(font);
        s.setString("Score: " + std::to_string(score));
        s.setCharacterSize(24);
        s.setPosition(160, 0);
        target.draw(s);

      }

      if (apples) {
        if (!apples->empty()) {
          target.draw((*apples)[0]);
        }
//        for (auto& apple: *apples) {
//          target.draw(apple);
//        }
      }
      if (worm) {
        target.draw(*worm);
      }


    }
  }



  Worm* get_worm() {
    return worm;
  }

  Level* get_current_level() {
     return current_level;
   }

 private:
  Worm *worm;
  std::vector<Apple>* apples;
  Level *current_level;
  sf::RectangleShape *wall;
  sf::RectangleShape *gate;
  sf::Font font;
  int score = 0;
  bool lvl_complete = false;
};

#endif //UNCLEWORM__STATE_H_
