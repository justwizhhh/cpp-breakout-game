
#ifndef BREAKOUT_GAME_H
#define BREAKOUT_GAME_H

#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Vector2.h"

class Game
{
 public:
  Game(sf::RenderWindow& window);
  ~Game();

  // Gameplay toggles
  float paddle_max_speed = 0;
  float paddle_bounce_limit = 0;

  float ball_max_speed = 0;

  int gem_min_speed = 0;
  int gem_max_speed = 0;
  float gem_spawn_range = 0;
  float gem_spawn_timer_min = 0;
  float gem_spawn_timer_max = 0;

  int brick_area_width = 0;
  int brick_area_height = 0;
  int brick_area_offset = 0;

  bool init();
  void update(float dt, sf::Clock clock1);
  void render();
  void mouseClicked(sf::Event event);
  void keyPressed(sf::Event event);
  void keyReleased(sf::Event event);

  Vector2 middle_of_screen;
  Vector2 window_size;
  sf::Clock clock;

  void startGame();
  void spawnBricks();
  void restartGame();

  // Gameplay objects
  GameObject paddle;
  bool left_input = false;
  bool right_input = false;
  int paddle_direction = 0;
  void movePaddle(float dt);

  GameObject ball;
  Vector2 ball_direction;
  void moveBall(float dt);
  void bounceBall(Vector2& bounce_direction, float dt);
  void paddleBounceBall();

  GameObject bricks[16][16];
  int destroyed_bricks = 0;

  GameObject gems[4];
  int gem_speed[4] = {0, 0, 0, 0};
  float gem_spawn_timer = 0;
  void moveGem(int gemNum, float dt);
  void gemSpawn();
  void gemTimer();

  enum game_states
  {
    TITLE,
    GAMEPLAY,
    WIN,
    GAME_OVER
  };
  game_states current_game_state;
  void changeState(game_states new_state);

  void update(float d);

 private:
  sf::RenderWindow& window;

  // Game elements
  int score;
  int lives;

  // UI elements
  sf::RectangleShape background;

  sf::Font main_font;

  sf::Text start_logo_text;
  sf::Text start_button_text;

  sf::Text score_header_text;
  sf::Text score_text;
  sf::Text lives_header_text;
  sf::Text lives_text;

  sf::Text win_logo_text;
  sf::Text gameover_logo_text;
  sf::Text restart_button_text;
};

#endif // BREAKOUT_GAME_H
