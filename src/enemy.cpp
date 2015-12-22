#include "enemy.h"
#include "player.h"
#include "gamedata.h"
#include "explodingSprite.h"
#include "collisionStrategy.h"

Enemy::Enemy()
  :Sprite("enemy", Vector2f(Gamedata::getInstance().getXmlInt("enemy/startLoc/x"), Gamedata::getInstance().getXmlInt("enemy/startLoc/y")), Vector2f(Gamedata::getInstance().getXmlInt("enemy/speedX"), Gamedata::getInstance().getXmlInt("enemy/speedY"))),
  isDead(false),
  explosion(NULL),
  strategies(),
  strategy(NULL)
{
  strategies.push_back(new MidPointCollisionStrategy);
  strategies.push_back(new RectangularCollisionStrategy);
  strategies.push_back(new PerPixelCollisionStrategy);
  strategy = strategies[2];
}

Enemy::Enemy(const Enemy& e)
  : Sprite(e),
  isDead(e.isDead),
  explosion(e.explosion),
  strategies(e.strategies),
  strategy(e.strategy)
{

}

Enemy::~Enemy()
{
  for (unsigned i = 0; i < strategies.size(); ++i)
  {
    delete strategies[i];
  }
}

void Enemy::draw() const
{
  if (explosion)
  {
    explosion->draw();
    return;
  }
  if (isDead == false)
  {
    Sprite::draw();
  }
}

void Enemy::update(Uint32 ticks)
{
  if (explosion)
  {
    explosion->update(ticks);
    if (explosion->chunkCount() == 0)
    {
      delete explosion;
      explosion = NULL;
    }
    return;
  }
  if (isDead == false)
  {
    Sprite::update(ticks);
  }
}

void Enemy::reset()
{
  isDead = false;
  velocityX(velocityXWhenMoving());
}

void Enemy::explode()
{
  if (explosion)
  {
    return;
  }
  Sprite sprite(getName(), getPosition(), getVelocity(), getFrame());
  explosion = new ExplodingSprite(sprite);
}