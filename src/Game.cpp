
#include "Game.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window),
  middle_of_screen(0, 0),
  window_size(0, 0),
  ball_direction(0, 0)
{
  srand(time(nullptr));

  current_game_state = game_states::TITLE;

  score = 0;
  lives = 3;

  middle_of_screen.set((float)window.getSize().x / 2, (float)window.getSize().y / 2);

  window_size.set(window.getView().getSize().x, window.getView().getSize().y);
}

Game::~Game()
{

}

bool Game::init()
{
  // Gameplay toggle values
  paddle_max_speed = 565;
  paddle_bounce_limit = 0.55;

  ball_max_speed = 465;

  brick_area_width = 8;
  brick_area_height = 5;
  brick_area_offset = 180;

  gem_min_speed = 100;
  gem_max_speed = 600;
  gem_spawn_range = 235;
  gem_spawn_timer_min = 0.8;
  gem_spawn_timer_max = 10;

  // Initialising gameplay sprites
  paddle.initialiseSprite("Data/Images/paddleRed.png");
  ball.initialiseSprite("Data/Images/ballGrey.png");

  for (auto & gem : gems)
  {
    gem.initialiseSprite("Data/Images/element_red_diamond_glossy.png");
    gem.setVisibility(false);
  }

  // Positioning all bricks
  sf::Texture brick_texture;
  brick_texture.loadFromFile("Data/Images/element_blue_rectangle.png");
  Vector2 start_position(
    middle_of_screen.x - (((float)brick_area_width / 2) * (float)brick_texture.getSize().x),
    middle_of_screen.y - (((float)brick_area_height / 2) * (float)brick_texture.getSize().y));
  for (int i = 0; i < brick_area_width; i++)
  {
    for (int j = 0; j < brick_area_height; j++)
    {
      int colour = std::rand() % 6;
      std::cout << colour << std::endl;

      switch (colour)
      {
        default:
        case 0:
          bricks[i][j].initialiseSprite("Data/Images/element_blue_rectangle.png");
          break;
        case 1:
          bricks[i][j].initialiseSprite("Data/Images/element_green_rectangle.png");
          break;
        case 2:
          bricks[i][j].initialiseSprite("Data/Images/element_grey_rectangle.png");
          break;
        case 3:
          bricks[i][j].initialiseSprite("Data/Images/element_purple_rectangle.png");
          break;
        case 4:
          bricks[i][j].initialiseSprite("Data/Images/element_red_rectangle.png");
          break;
        case 5:
          bricks[i][j].initialiseSprite("Data/Images/element_yellow_rectangle.png");
          break;
      }

      bricks[i][j].position.set(
        start_position.x + ((float)i * bricks[i][j].sprite.getGlobalBounds().width),
        start_position.y + ((float)j * bricks[i][j].sprite.getGlobalBounds().height) - (float)brick_area_offset);

      bricks[i][j].updatePos();
      bricks[i][j].setVisibility(true);
    }
  }

  // Positioning all gameplay objects
  spawnBricks();
  gemTimer();

  startGame();

  // Initialising UI elements
  background.setFillColor(sf::Color(1, 23, 92, 255));
  background.setSize(sf::Vector2f(window_size.x, window_size.y));

  if (!main_font.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout << "Font not loaded correctly! \n";
    return false;
  }
  else
  {
    start_logo_text.setFont(main_font);
    start_logo_text.setScale(3, 3);
    start_logo_text.setPosition(310, 240);
    start_logo_text.setFillColor(sf::Color(255, 255, 255, 255));
    start_logo_text.setString("Breakout");

    start_button_text.setFont(main_font);
    start_button_text.setScale(1.4, 1.4);
    start_button_text.setPosition(230, 430);
    start_button_text.setFillColor(sf::Color(255, 255, 255, 255));
    start_button_text.setString("Press enter to start the game");

    score_header_text.setFont(main_font);
    score_header_text.setScale(1.2, 1.2);
    score_header_text.setPosition(40, 25);
    score_header_text.setFillColor(sf::Color(255, 255, 255, 255));
    score_header_text.setString("Score");

    score_text.setFont(main_font);
    score_text.setScale(1.6, 1.6);
    score_text.setPosition(40, 65);
    score_text.setFillColor(sf::Color(255, 255, 255, 255));
    score_text.setString("0");

    lives_header_text.setFont(main_font);
    lives_header_text.setScale(1.2, 1.2);
    lives_header_text.setPosition((float)window_size.x - 120, 25);
    lives_header_text.setFillColor(sf::Color(255, 255, 255, 255));
    lives_header_text.setString("Lives");

    lives_text.setFont(main_font);
    lives_text.setScale(1.6, 1.6);
    lives_text.setPosition((float)window_size.x - 55, 65);
    lives_text.setFillColor(sf::Color(255, 255, 255, 255));
    lives_text.setString("3");

    win_logo_text.setFont(main_font);
    win_logo_text.setScale(3, 3);
    win_logo_text.setPosition(140, 240);
    win_logo_text.setFillColor(sf::Color(255, 255, 255, 255));
    win_logo_text.setString("Congratulations!");

    gameover_logo_text.setFont(main_font);
    gameover_logo_text.setScale(3, 3);
    gameover_logo_text.setPosition(250, 240);
    gameover_logo_text.setFillColor(sf::Color(255, 255, 255, 255));
    gameover_logo_text.setString("Game Over!");

    restart_button_text.setFont(main_font);
    restart_button_text.setScale(1.4, 1.4);
    restart_button_text.setPosition(190, 430);
    restart_button_text.setFillColor(sf::Color(255, 255, 255, 255));
    restart_button_text.setString("Press enter to restart the game");
  }

  return true;
}

void Game::mouseClicked(sf::Event event)
{
  //get the click position
  //sf::Vector2i click = sf::Mouse::getPosition(window);
}

void Game::keyPressed(sf::Event event)
{
  switch(current_game_state)
  {
    case TITLE:
      // Start menu
      if (event.key.code == sf::Keyboard::Enter)
      {
        changeState(GAMEPLAY);
      }

      break;

    case GAMEPLAY:
      // Main gameplay logic
      if (event.key.code == sf::Keyboard::A)
      {
        left_input = true;
      }
      if (event.key.code == sf::Keyboard::D)
      {
        right_input = true;
      }

      break;

    case WIN:
    case GAME_OVER:
      // Game restart
      if (event.key.code == sf::Keyboard::Enter)
        restartGame();

      break;
  }
}

void Game::keyReleased(sf::Event event)
{
  // Movement inputs are only processed during the 'GAMEPLAY' state, no need for switch statement
  if (current_game_state == GAMEPLAY)
  {
    // Main gameplay logic
    if (event.key.code == sf::Keyboard::A)
    {
      left_input = false;
    }
    if (event.key.code == sf::Keyboard::D)
    {
      right_input = false;
    }
  }
}

void Game::startGame()
{
  paddle.position.set(
    middle_of_screen.x - (paddle.sprite.getGlobalBounds().width / 2),
    middle_of_screen.y + 300);

  ball.position.set(
    middle_of_screen.x - (ball.sprite.getGlobalBounds().width / 2),
    middle_of_screen.y);
  ball_direction = Vector2(0, 1);

  // Ball should not unfairly hit a pre-existing gem when respawning
  for (auto & gem : gems)
  {
    if (gem.visibility)
    {
      gem.setVisibility(false);
    }
  }
}

void Game::spawnBricks()
{
  // Positioning all bricks
  for (int i = 0; i < brick_area_width; i++)
  {
    for (int j = 0; j < brick_area_height; j++)
    {
      if (!bricks[i][j].visibility)
      {
        bricks[i][j].setVisibility(true);
      }
    }
  }
}

void Game::restartGame()
{
  left_input = false;
  right_input = false;

  score = 0;
  score_text.setString(std::to_string(score));
  lives = 3;
  lives_text.setString(std::to_string(lives));

  destroyed_bricks = 0;

  for (auto & gem : gems)
  {
    if (gem.visibility)
    {
      gem.setVisibility(false);
    }
  }

  startGame();
  spawnBricks();
  changeState(GAMEPLAY);
}

void Game::changeState(Game::game_states new_state)
{
  current_game_state = new_state;
}

void Game::movePaddle(float dt)
{
  paddle_direction = left_input && right_input ? 0 : left_input ? -1 : right_input ? 1 : 0;

  float left_side = 0;
  float right_side = (float)window_size.x - paddle.sprite.getGlobalBounds().width;

  paddle.position.x += paddle_max_speed * (float)paddle_direction * dt;
  paddle.position.x = std::max(left_side, std::min(paddle.position.x, right_side));
}

void Game::moveBall(float dt)
{
  ball.position.x += ball_direction.x * ball_max_speed * dt;
  ball.position.y += ball_direction.y * ball_max_speed * dt;
}

void Game::bounceBall(Vector2& bounce_direction, float dt)
{
  if (bounce_direction.x != 0)
  {
    ball.position.x += (ball_max_speed * ball_direction.x * dt) * -bounce_direction.x;

    if (ball_direction.x == 0)
      ball_direction.x = bounce_direction.x;
    else if (bounce_direction.x >= 1)
      ball_direction.x = std::abs(ball_direction.x);
    else if (bounce_direction.x <= -1)
      ball_direction.x = -std::abs(ball_direction.x);
  }

  if (bounce_direction.y != 0)
  {
    ball.position.y += (ball_max_speed * ball_direction.y * dt) * -bounce_direction.y;

    if (ball_direction.y == 0)
      ball_direction.y = bounce_direction.y;
    else if (bounce_direction.y >= 1)
      ball_direction.y = std::abs(ball_direction.y);
    else if (bounce_direction.y <= -1)
      ball_direction.y = -std::abs(ball_direction.y);
  }
}

void Game::paddleBounceBall()
{
  // Change the ball's direction depending on the angle between that and the colliding paddle
  Vector2 direction_scale((ball.getMidPoint().x - paddle.getMidPoint().x) * paddle_bounce_limit,
                          ball.getMidPoint().y - paddle.getMidPoint().y);

  direction_scale.normalise();
  ball_direction = direction_scale;
}

void Game::moveGem(int gemNum, float dt)
{
  GameObject& gem = gems[gemNum];

  gem.position.y += (float)gem_speed[gemNum] * dt;

  if (gem.position.y >= (float)window_size.y)
  {
    gem.setVisibility(false);
  }
}

void Game::gemSpawn()
{
  for (int i = 0; i < (sizeof(gems) / sizeof(gems[0])); i++)
  {
    GameObject& gem = gems[i];
    if (!gem.visibility)
    {
      float random_float = (float)std::rand() / RAND_MAX;
      float x_pos = (middle_of_screen.x - gem_spawn_range) + random_float * gem_spawn_range;

      gem.position.x = x_pos;
      gem.position.y = -gem.sprite.getGlobalBounds().height;
      gem.updatePos();

      gem_speed[i] = gem_min_speed + std::rand() % (gem_max_speed - gem_min_speed + 1);

      gem.setVisibility(true);
      break;
    }
  }
}

void Game::gemTimer()
{
  float random_float = (float)std::rand() / RAND_MAX;
  gem_spawn_timer = gem_spawn_timer_min + random_float * (gem_spawn_timer_max - gem_spawn_timer_min);
  clock.restart();
}

void Game::update(float dt)
{
  if (current_game_state == GAMEPLAY)
  {
    movePaddle(dt);
    paddle.updatePos();

    moveBall(dt);
    ball.updatePos();

    // Ball bouncing off of horizontal walls
    if (ball.position.x < 0)
    {
      ball_direction.x *= -1;
      ball.position.x = 0;
    }
    if (ball.position.x > ((float)window_size.x - ball.sprite.getGlobalBounds().width))
    {
      ball_direction.x *= -1;
      ball.position.x = ((float)window_size.x - ball.sprite.getGlobalBounds().width);
    }

    // Ball bouncing off of vertical walls
    if (ball.position.y < 0)
    {
      ball_direction.y *= -1;
      ball.position.y = 0;
    }
    if (ball.position.y > (float)window_size.y - ball.sprite.getGlobalBounds().height)
    {
      // Player loses a life if the ball goes below the paddle
      if (lives > 1)
      {
        lives--;
        lives_text.setString(std::to_string(lives));

        startGame();
      }
      else
      {
        changeState(GAME_OVER);
      }
    }

    // Ball bouncing off of the paddle
    Vector2 paddle_col = ball.collision(paddle);
    if (!(paddle_col.x == 0 && paddle_col.y == 0))
    {
      paddleBounceBall();
    }

    // Ball bouncing off of bricks
    bool loop_break;
    for (int i = 0; i < brick_area_width; i++)
    {
      loop_break = false;
      for (int j = 0; j < brick_area_height; j++)
      {
        GameObject& current_brick = bricks[i][j];
        if (current_brick.visibility)
        {
          Vector2 brick_col = ball.collision(current_brick);
          if (!(brick_col.x == 0 && brick_col.y == 0))
          {
            loop_break = true;

            current_brick.setVisibility(false);
            destroyed_bricks++;

            score += 10;
            score_text.setString(std::to_string(score));

            bounceBall(brick_col, dt);
          }
        }

        if (loop_break)
          break;
      }

      if (loop_break)
        break;
    }

    // Ball bouncing off of gems
    for (auto & gem : gems)
    {
      if (gem.visibility)
      {
        Vector2 gem_col = ball.collision(gem);
        if (!(gem_col.x == 0 && gem_col.y == 0))
        {
          ball_direction = ball_direction * -1;

          gem.setVisibility(false);
        }
      }
    }

    // Gem movement
    for (int i = 0; i < (sizeof(gems) / sizeof(gems[0])); i++)
    {
      GameObject& gem = gems[i];
      if (gem.visibility)
      {
        moveGem(i, dt);
        gem.updatePos();

        // Collision with the paddle
        Vector2 paddleCol = gem.collision(paddle);
        if (!(paddleCol.x == 0 && paddleCol.y == 0))
        {
          score += 200;
          score_text.setString(std::to_string(score));
          gem.setVisibility(false);
        }
      }
    }

    // Gem spawning
    if (clock.getElapsedTime().asSeconds() >= gem_spawn_timer)
    {
      gemTimer();
      gemSpawn();
    }

    // Win condition (all bricks have been cleared)
    if (destroyed_bricks >= (brick_area_width * brick_area_height))
    {
      changeState(WIN);
    }
  }
}

void Game::render()
{
  // Rendering all UI assets
  window.draw(background);
  switch(current_game_state)
  {
    case TITLE:
      window.draw(start_logo_text);
      window.draw(start_button_text);

      break;

    case GAMEPLAY:
      // Game elements
      window.draw(paddle.sprite);
      window.draw(ball.sprite);

      for (int i = 0; i < brick_area_width; i++)
      {
        for (int j = 0; j < brick_area_height; j++)
        {
          if (bricks[i][j].visibility)
            window.draw(bricks[i][j].sprite);
        }
      }

      for (auto & gem : gems)
      {
        if (gem.visibility)
        {
          window.draw(gem.sprite);
        }
      }

      // UI elements
      window.draw(score_header_text);
      window.draw(score_text);

      window.draw(lives_header_text);
      window.draw(lives_text);

      break;

    case WIN:
      window.draw(win_logo_text);
      window.draw(restart_button_text);
      break;

    case GAME_OVER:
      window.draw(gameover_logo_text);
      window.draw(restart_button_text);
      break;
  }
}
