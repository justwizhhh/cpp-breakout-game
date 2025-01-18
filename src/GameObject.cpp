#include "Vector2.h"
#include "GameObject.h"

#include <iostream>

GameObject::GameObject() :
  position(0, 0)
{
  setVisibility(false);
}

GameObject::~GameObject()
{

}

const sf::Sprite& GameObject::getSprite() const
{
  return sprite;
}

/*
 * @brief   Updates the object's current sprite
 * @details Load in a texture for the current object's sprite
 * @return  void
 */

bool GameObject::initialiseSprite(const std::string& file_path)
{
  if (!texture.loadFromFile(file_path))
  {
    std::cout << " Texture for " << file_path << " unable to be loaded in!" << std::endl;
    return false;
  }
  else
  {
    sprite.setTexture(texture);
    return true;
  }
}

const Vector2& GameObject::getPosition() const
{
  return position;
}

/*
 * @brief   Updates the object's current position
 * @details Takes the Vector2 'position' value and assigns it to the actual sprite
 * @return  void
 */
void GameObject::updatePos()
{
  sprite.setPosition(position.x, position.y);
}


/*
 * @brief   Gets the object's mid-point
 * @details Returns the centre position of the object's current sprite
 * @return  Vector2
 */
Vector2 GameObject::getMidPoint() const
{
  float mid_point_x = position.x + (sprite.getGlobalBounds().width / 2);
  float mid_point_y = position.y + (sprite.getGlobalBounds().height / 2);

  Vector2 new_vector(mid_point_x, mid_point_y);

  return new_vector;
}


/*
 * @brief   Checks if there is a collision happening
 * @details Checks for if the current object's sprite is colliding another sprite
 * @return  Vector2
 */
Vector2 GameObject::collision(const GameObject& other_object) const
{
  // Basic AABB collision detection
  Vector2 bounds(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
  Vector2 other_bounds(other_object.sprite.getGlobalBounds().width, other_object.sprite.getGlobalBounds().height);

  // Potential horizontal overlap
  bool collision_x = (position.x + bounds.x) > other_object.position.x
                     and (other_object.position.x + other_bounds.x) > position.x;

  // Potential vertical overlap
  bool collision_y = (position.y + bounds.y) > other_object.position.y
                     and (other_object.position.y + other_bounds.y) > position.y;

  // If there definitely is an overlap, find out on which side the collision occurred
  if (collision_x && collision_y)
  {
    Vector2 side(0, 0);
    float x = (position.x + (bounds.x / 2));
    float y = (position.y + (bounds.y / 2));
    float other_x = (other_object.position.x + (other_bounds.x / 2));
    float other_y = (other_object.position.y + (other_bounds.y / 2));

    // If the ball's y-position is inside the brick somewhere, then it's probably a horizontal collision
    if (y >= other_object.position.y && y <= other_object.position.y + other_bounds.y)
    {
      if (x <= other_x)
        side.x = -1;
      else if (x > other_x)
        side.x = 1;
    }
    if (x >= other_object.position.x && x <= other_object.position.x + other_bounds.x)
    {
      if (y <= other_y)
        side.y = -1;
      else if (y > other_y)
        side.y = 1;
    }

    return Vector2(side.x, side.y);
  }
  else
    return Vector2(0, 0);
}

/*
 * @brief   Checks if there is an overlap happening
 * @details Returns values for how much an object is overlapping another
 * @return  Vector2
 */
Vector2 GameObject::overlap(const GameObject& other_object) const
{
  // Basic AABB collision detection again
  Vector2 bounds(
    sprite.getGlobalBounds().width,
    sprite.getGlobalBounds().height);
  Vector2 other_bounds(
    other_object.sprite.getGlobalBounds().width,
    other_object.sprite.getGlobalBounds().height);

  // Potential horizontal overlap
  bool collision_x = (position.x + bounds.x) > other_object.position.x and
                     (other_object.position.x + other_bounds.x) > position.x;

  // Potential vertical overlap
  bool collision_y = (position.y + bounds.y) > other_object.position.y and
                     (other_object.position.y + other_bounds.y) > position.y;

  float x = 0;
  float y = 0;

  if (collision_x)
    x = (position.x + (bounds.x / 2)) - (other_object.position.x + (bounds.x / 2));
  if (collision_y)
    y = (position.y + (bounds.y / 2)) - (other_object.position.y + (bounds.y / 2));

  return Vector2(x, y);
}

void GameObject::setVisibility(bool toggle)
{
  visibility = toggle;
}
