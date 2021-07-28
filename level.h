//
// Created by Julian Rachele on 7/27/21.
//

#ifndef UNCLEWORM__LEVEL_H_
#define UNCLEWORM__LEVEL_H_
#include "apples.h"

enum Levels {
  GAME_OVER,
  INTRO,
  LEVEL1,
  LEVEL2,
  LEVEL3,
  LEVEL4,
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


class L1 : public Level {
 public:
  std::vector<Apple> apples() override {
    return Apple::from_vec(
        {
            {0, 10, 0, 5},
            {10, 0, 0, 5},
            {0, 0, 10, 0},
            {0, 10, 0, 0}
        }
    );
  }
   int number() {
     return LEVEL1;
   };
};

class L2 : public Level {
 public:
  std::vector<Apple> apples() override {
    return Apple::from_vec(
        {
            {5, 5, 0, 5, 2, 0, 0, 0},
            {10, 0, 0, 10, 0, 0, 0, 0},
            {0, 0, 10, 0, 5, 0, 5, 0},
            {0, 10, 0, 0, 0, 0, 0, 0},
            {10, 0, 0, 10, 0, 0, 0, 0},
            {0, 0, 10, 0, 5, 0, 5, 0},
            {0, 0, 10, 0, 5, 0, 5, 0},
            {0, 10, 0, 0, 0, 0, 0, 0}
        }
    );
  }
  int number() {
    return LEVEL1;
  };
};

class GameOver : public Level {
  int number() {
    return GAME_OVER;
  }

  std::string title() { return "GAME OVER :(";}

  std::string subtitle() { return "Time go ni night";}
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
      return new GameOver();
    case INTRO:
      return new Intro();
    case LEVEL1:
      return new L1();
    case LEVEL2:
      return new L2();
    default:
      return new Intro();
  }
}
#endif //UNCLEWORM__LEVEL_H_
