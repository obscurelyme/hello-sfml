#include "Game/Enemy.hpp"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <iostream>
#include <random>

#include "Game/PlayerEvents.hpp"
#include "Logger.hpp"
#include "Renderer.hpp"

unsigned int Enemy::_uid = 0;
std::random_device device;
std::default_random_engine engine(device());
std::uniform_real_distribution<double> distribution(0, 360);

Enemy::Enemy() : _collider(nullptr), _enteredScreen(false), _active(false) {
  _priorTicks = SDL_GetTicks();
  _ticks = _priorTicks;
  _id = "Enemy-" + std::to_string(++_uid);
  _collider = std::make_shared<Collider>(Collider::Type::Enemy, _active);
  _collider->clientRect.h = _clientRect.h;
  _collider->clientRect.w = _clientRect.w;
  _collider->OnCollide(std::bind(&Enemy::OnCollision, this, std::placeholders::_1));
}

Enemy::~Enemy() { std::cout << "deleted " << _id << std::endl; }

void Enemy::Init() {}

void Enemy::Render() {
  SDL_RendererFlip flip = SDL_FLIP_NONE;

  float xx = (400 - _clientRect.x);
  float yy = (300 - _clientRect.y);

  float _rotation = glm::degrees(glm::atan(yy, xx));
  SDL_RenderCopyExF(CoffeeMaker::Renderer::Instance(), _texture.Handle(), &_clipRect, &_clientRect, _rotation + 90,
                    NULL, flip);
  // _collider->Render();
}

void Enemy::Pause() {}

void Enemy::Unpause() {}

void Enemy::Update(float deltaTime) {
  if (_active) {
    _clientRect.x += _movement.x * _speed * deltaTime;
    _clientRect.y += _movement.y * _speed * deltaTime;
    _collider->Update(_clientRect);

    if (!_enteredScreen && !IsOffScreen()) {
      _enteredScreen = true;
    }

    if (IsOffScreen() && _enteredScreen) {
      // Enemy went off screen
      Spawn();
    }
  }
}

void Enemy::Spawn() {
  // Pick a random location around the player
  _enteredScreen = false;
  _collider->active = false;

  double randomAngle = glm::radians(distribution(engine));

  _clientRect.x = (float)(300 + 400 * cos(randomAngle));
  _clientRect.y = (float)(400 + 400 * sin(randomAngle));
  _collider->Update(_clientRect);

  CM_LOGGER_INFO("Enemy {} spawned at position ({}, {})", _id, _clientRect.x, _clientRect.y);

  _movement = glm::normalize(glm::vec2(400 - _clientRect.x, 300 - _clientRect.y));

  _active = true;
  _collider->active = true;
}

bool Enemy::IsActive() const { return _active; }

void Enemy::OnCollision(Collider* collider) {
  if (collider->GetType() == Collider::Type::Projectile) {
    incScore->Emit();
    Spawn();
  }
}

bool Enemy::IsOffScreen() const {
  // TODO: screen width and height should be dynamic
  return _clientRect.x + _clientRect.w <= 0 || _clientRect.x >= 800 || _clientRect.y + _clientRect.h <= 0 ||
         _clientRect.y >= 600;
}

SpecialEnemy::SpecialEnemy() {}

SpecialEnemy::~SpecialEnemy() {}

void SpecialEnemy::Init() {
  _transformVector.x = -50;
  _transformVector.y = 650;
  _endVector.x = 850;
  _endVector.y = 650;
}

void SpecialEnemy::Update(float deltaTime) {
  _currentTime += deltaTime;
  float weight = _currentTime / _totalTime;
  CoffeeMaker::Math::Vector2D currentPos =
      CoffeeMaker::Math::CubicBezierCurve(_transformVector, CoffeeMaker::Math::Vector2D(0.0f, 0.0f),
                                          CoffeeMaker::Math::Vector2D(850.0f, 0.0f), _endVector, weight);
  _clientRect.x = currentPos.x;
  _clientRect.y = currentPos.y;
  _collider->Update(_clientRect);

  if (!_enteredScreen && !IsOffScreen()) {
    _enteredScreen = true;
  }

  if (IsOffScreen() && _enteredScreen) {
    // Enemy went off screen
    Spawn();
  }
}

void SpecialEnemy::Render() {
  SDL_RendererFlip flip = SDL_FLIP_NONE;
  SDL_RenderCopyExF(CoffeeMaker::Renderer::Instance(), _texture.Handle(), &_clipRect, &_clientRect, 90, NULL, flip);
}

void SpecialEnemy::Pause() {}

void SpecialEnemy::Unpause() {}

void SpecialEnemy::Spawn() {
  _currentTime = 0.0f;
  _enteredScreen = false;
  _collider->active = false;

  _clientRect.x = _transformVector.x;
  _clientRect.y = _transformVector.y;
  _collider->Update(_clientRect);

  _active = true;
  _collider->active = true;
}
