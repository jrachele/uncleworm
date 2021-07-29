//
// Created by Julian Rachele on 6/26/21.
//

#ifndef UNCLEWORM__APPLES_H_
#define UNCLEWORM__APPLES_H_
#include <SFML/Graphics.hpp>
#include <thread>
#include <random>
#include "global.h"

class Apple : public sf::Drawable {
 public:
  Apple(sf::Vector2f pos = {0, 0}, int size = 1,  int pixel_size = 8)
  : pixel_size_(pixel_size), size_(size),  pos_(pos) {
    shape_ = sf::CircleShape((float(getTrueSize())));
//    sf::RectangleShape shape({float(getTrueSize()), float(getTrueSize())});
    shape_.setPosition(pos_);
    shape_.setOrigin(getTrueSize() / 2, getTrueSize() / 2);
    shape_.setFillColor(sf::Color::Black);
  }

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    // don't actually draw the shape, draw a collection of boxes
    int size = size_ * 2;
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        if (j == 0 || j == size - 1) {
          if (i == 0 || i == size - 1) {
            continue;
          }
        }
        sf::RectangleShape r;
        auto [x, y] = pos_;
        x = x - (getTrueSize() / 2);
        y = y - (getTrueSize() / 2);
        r.setPosition({x + (pixel_size_ * j), y + (pixel_size_ * i)});
        float s = pixel_size_;
        r.setSize({s, s});
        r.setFillColor(sf::Color::Black);
        target.draw(r);
      }
    }


//    target.draw(shape_);
  }

  sf::Vector2f getPosition() const {
    return this->pos_;
  }

  int getSize() const {
    return this->size_;
  }

  int getTrueSize() const {
    return this->size_*this->pixel_size_;
  }

  sf::FloatRect getGlobalBounds() const {
    return shape_.getGlobalBounds();
  }

  static std::vector<Apple> from_vec(const std::vector<std::vector<int>>& matrix,
                                     float width = 1280, float height = 960, int pixel_size = 8) {
    size_t rows, cols;
    rows = matrix.size();
    if (rows == 0) return std::vector<Apple>();
    cols = matrix[0].size();
    float horiz = width / cols;
    float vert = height / rows;
    auto ret = std::vector<Apple>();

    for (size_t i = 0; i < rows; i++) {
      for (size_t j = 0; j < cols; j++) {
        int size = matrix[i][j];
        if (size > 0) {
          Apple a({ horiz*i + (horiz/2), vert*j + (vert/2)}, size, pixel_size);
          ret.push_back(a);
        }
      }
    }
    return ret;
  }

  // Generates list of Apples based on given total score desired, number of apples, and wall size
  static std::vector<Apple> from_target(int num_apples, int wall_size) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<> dist(3, 8);

    std::vector<Apple> ret;
    for (int i=0; i<num_apples; ++i) {
      int size = dist(mt);
      std::uniform_real_distribution<> dist_x(wall_size*2 + size, WIDTH - wall_size*2 - size);
      std::uniform_real_distribution<> dist_y(wall_size*2 + size, HEIGHT - wall_size*2 - size);
      float x = dist_x(mt);
      float y = dist_y(mt);
      std::cout << size << " " << x << " " << y << std::endl;
      Apple a({x, y}, size);
      bool bad = false;
      for (auto &o : ret) {
        if (a.getGlobalBounds().intersects(o.getGlobalBounds())) {
          bad = true;
          break;
        }
      }
      if (bad) {
        i--;
        continue;
      }
      ret.emplace_back(Apple({x, y}, size));
    }
    return ret;
  }


 private:
  sf::CircleShape shape_;
  sf::Vector2f pos_;
  int pixel_size_;
  int size_;
};

#endif //UNCLEWORM__APPLES_H_
