//
// Created by Julian Rachele on 7/27/21.
//

#ifndef UNCLEWORM__LEVEL_H_
#define UNCLEWORM__LEVEL_H_
#include "apples.h"

enum Levels {
  GAME_OVER = -1,
  INTRO = 0,
};


class Level {
 public:
  virtual std::vector<Apple> apples() {
    return std::vector<Apple>();
  };
  virtual int number() = 0;
  virtual std::string title() {return "";};
  virtual std::string subtitle() {return "";};
};

class Gameplay : public Level {
 public:
  Gameplay(int number, int num_apples = 0) : number_(number), num_apples_(num_apples) {
    if (num_apples == 0) {
      num_apples_ = 5 + number;
    }
  }

  std::vector<Apple> apples() override {
    int thickness = number() * 5 + 20;
    return Apple::from_target(num_apples_, thickness);
  }

  int number() override {
    return number_;
  }

 private:
  int number_;
  int num_apples_;
};


class GameOver : public Level {
 public:
  GameOver(int score, int level) : score(score), level(level) {}
  int number() override {
    return GAME_OVER;
  }

  std::string title() override { return "GAME OVER :(";}

  std::string subtitle() override {
    if (score > 0) {
      return "Level: " + std::to_string(level) + " Score: " + std::to_string(score);
    }
    return "Time go ni night";
  }
 private:
  int score;
  int level;
};

class Intro : public Level {
  int number() {
    return INTRO;
  }

  std::string title() { return "UNCLE WORM";}

  std::string subtitle() { return "Press Space";}
};

static Level* fromInt(int lvl) {
  switch (lvl) {
    case GAME_OVER:
      return new GameOver(0, 1);
    case INTRO:
      return new Intro();
    default:
      return new Gameplay(lvl);
  }
}
#endif //UNCLEWORM__LEVEL_H_
