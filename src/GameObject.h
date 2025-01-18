
#ifndef BREAKOUT_GAMEOBJECT_H
#define BREAKOUT_GAMEOBJECT_H

#include "Vector2.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

class GameObject
{
 public:
  GameObject();
  ~GameObject();

  Vector2 position;
  const Vector2& getPosition() const;

  sf::Sprite sprite;
  const sf::Sprite& getSprite() const;
  sf::Texture texture;

  bool initialiseSprite(const std::string& file_path);
  void updatePos();
  Vector2 collision(const GameObject& other_object) const;
  Vector2 overlap(const GameObject& other_object) const;

  Vector2 getMidPoint() const;

  bool visibility = false;
  void setVisibility(bool toggle);
};


#endif // BREAKOUT_GAMEOBJECT_H
