//
// Created by Julian Rachele on 6/26/21.
//

#ifndef UNCLEWORM__APPLES_H_
#define UNCLEWORM__APPLES_H_
#include <SFML/Graphics.hpp>
#include <thread>

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
    target.draw(shape_);
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

 private:
  sf::CircleShape shape_;
  sf::Vector2f pos_;
  int pixel_size_;
  int size_;
};

#endif //UNCLEWORM__APPLES_H_
