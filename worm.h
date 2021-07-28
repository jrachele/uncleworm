//
// Created by Julian Rachele on 6/25/21.
//

#ifndef UNCLEWORM__WORM_H_
#define UNCLEWORM__WORM_H_
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <list>
#include <thread>
#include "apples.h"

#define RAD 0.0174533

class Worm : public sf::Drawable {
 public:
  // segment size will also represent pixel size
  explicit Worm(float segment_size = 8, float width = 1280, float height = 960) : segment_size_(segment_size),
    width_(width), height_(height)
  {
    // the last member of the list is effectively the position of the worm
    segments_.emplace_back(sf::Vector2f {width / 2, height-segment_size});
  }

  void update() {
//    std::cout << dt.asMicroseconds() << std::endl;
    if (growing_ != 0) {
      // don't do anything to current segments in the list
      growing_--;
    } else {
      segments_.pop_front();
    }
    auto [x, y] = segments_.back();
    segments_.emplace_back(calculate_position({x, y}, direction_, segment_size_));
//    std::this_thread::sleep_for(std::chrono::milliseconds(30));
  }

  void turn(int offset = 1) {
    direction_ += offset * 8;
    if (direction_ < 0) direction_ += 360;
    direction_ = direction_ % 360;
  }

  void grow(int growth) {
    growing_ += growth;
  }

  bool collided(const sf::FloatRect& other) {
    auto head = generate_shape(segments_.back(), segment_size_);
    return head.getGlobalBounds().intersects(other);
  }

  bool check_died() {
    auto head = generate_shape(segments_.back(), segment_size_);
    size_t seg_num = segments_.size();
    // - 2 so that we don't get the true head nor the segment immediately touching the head
    auto it = segments_.begin();
    for (int i = 0; i < seg_num - 2; i++, it++) {
      auto& s = *it;
      auto seg = generate_shape(s, segment_size_);
      if (head.getGlobalBounds().intersects(seg.getGlobalBounds())) {
        return true;
      }
    }
    return false;
  }

  sf::FloatRect getGlobalBounds() const {
    return generate_shape(segments_.back(), segment_size_).getGlobalBounds();
  }

  static sf::RectangleShape generate_shape(const sf::Vector2f& position, float segment_size) {
    auto [x, y] = position;
    sf::RectangleShape segment({segment_size, segment_size});
//      sf::CircleShape segment({segment_size_});
//      segment.setPosition(x, y);
//      segment.setOrigin({segment_size_ / 2, segment_size_ / 2});
    segment.setPosition(int(x/segment_size) * segment_size, int(y/segment_size)*segment_size);
    segment.setFillColor(sf::Color::Black);
    return segment;
  }

  static sf::Vector2f calculate_position(const sf::Vector2f& prev_position, int direction, float segment_size) {
    auto [old_x, old_y] = prev_position;
    float x_offset = cos(float(direction)*RAD) * segment_size;
    float y_offset = sin(float(direction)*RAD) * segment_size;
    return sf::Vector2f {old_x + x_offset, old_y + y_offset};
  }

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    for (auto [x, y] : segments_) {
      auto segment = generate_shape({x, y}, segment_size_);
      target.draw(segment);
    }
  }

 private:
  std::list<sf::Vector2f> segments_;
  float segment_size_;
  float width_;
  float height_;
  int growing_ = 20;
  int direction_ = 270;
};

#endif //UNCLEWORM__WORM_H_
