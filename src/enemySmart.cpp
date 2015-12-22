#include "enemySmart.h"
#include "player.h"
#include "gamedata.h"
#include "explodingSprite.h"
#include "collisionStrategy.h"

EnemySmart::EnemySmart()
  :Sprite("enemySmart", Vector2f(Gamedata::getInstance().getXmlInt("enemySmart/startLoc/x"), Gamedata::getInstance().getXmlInt("enemySmart/startLoc/y")), Vector2f(Gamedata::getInstance().getXmlInt("enemySmart/speedX"), Gamedata::getInstance().getXmlInt("enemySmart/speedY"))),
  isDead(false),
  yLow(Gamedata::getInstance().getXmlInt("enemySmart/yLow")),
  yHigh(Gamedata::getInstance().getXmlInt("enemySmart/yHigh")),
  explosion(NULL),
  strategies(),
  strategy(NULL),
  minimumXDistanceToTriggerAttack(Gamedata::getInstance().getXmlInt("enemySmart/minimumXDistanceToTriggerAttack"))
{
  strategies.push_back(new MidPointCollisionStrategy);
  strategies.push_back(new RectangularCollisionStrategy);
  strategies.push_back(new PerPixelCollisionStrategy);
  strategy = strategies[2];
}

EnemySmart::EnemySmart(const EnemySmart& e)
  : Sprite(e),
  isDead(e.isDead),
  yLow(e.yLow),
  yHigh(e.yHigh),
  explosion(e.explosion),
  strategies(e.strategies),
  strategy(e.strategy),
  minimumXDistanceToTriggerAttack(e.minimumXDistanceToTriggerAttack)
{

}

EnemySmart::~EnemySmart()
{
  for (unsigned i = 0; i < strategies.size(); ++i)
  {
    delete strategies[i];
  }
}

void EnemySmart::draw() const
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

void EnemySmart::update(Uint32 ticks)
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
    setPosition(getPosition() + getVelocity() * static_cast<float>(ticks) * 0.001);
    if (X() < 0)
    {
      velocityX(abs(velocityX()));
    }
    if (X() > worldWidth - frameWidth)
    {
      velocityX(-abs(velocityX()));
    }

    float xPositionDifference = abs(getPosition()[0] - Player::getInstance().getPosition()[0]);
    if (xPositionDifference < minimumXDistanceToTriggerAttack)
    {
      if (Y() >= yLow)
      {
        velocityY(-abs(velocityYWhenMoving()));
      }
      if (Y() <= yHigh)
      {
        velocityY(abs(velocityYWhenMoving()));
      }
    }
    else
    {
      if (Y() <= yHigh)
      {
        velocityY(0);
        Y(yHigh);
      }
      else
      {
        velocityY(-abs(velocityYWhenMoving()));
      }
    }
  }
}

void EnemySmart::reset()
{
  isDead = false;

  Y(yHigh);
  velocityX(velocityXWhenMoving());
  velocityY(0);
}

void EnemySmart::explode()
{
  if (explosion)
  {
    return;
  }
  Sprite sprite(getName(), getPosition(), getVelocity(), getFrame());
  explosion = new ExplodingSprite(sprite);
}