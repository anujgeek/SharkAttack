#include "gamedata.h"
#include "player.h"
#include "bulletPool.h"
#include "bullet.h"

Bullet::Bullet()
  : Sprite("bullet", Vector2f(Gamedata::getInstance().getXmlInt("bullet/startLoc/x"), Gamedata::getInstance().getXmlInt("bullet/startLoc/y")), Vector2f(Gamedata::getInstance().getXmlInt("bullet/speedX"), Gamedata::getInstance().getXmlInt("bullet/speedY"))),
  distance(0),
  distanceDie(Gamedata::getInstance().getXmlInt("bullet/distanceDie")),
  isTooFar(false)
{

}

Bullet::Bullet(const Bullet& b)
  : Sprite(b),
  distance(b.distance),
  distanceDie(b.distanceDie),
  isTooFar(b.isTooFar)
{

}

Bullet::~Bullet()
{
}

bool Bullet::collidedWith(const Drawable* d) const
{
  return Sprite::collidedWith(d);
}

void Bullet::draw() const
{
  Sprite::draw();
}

void Bullet::update(Uint32 ticks)
{
  Vector2f tPosition = getPosition();
  setPosition(getPosition() + getVelocity() * static_cast<float>(ticks) * 0.001);
  distance += (hypot(X() - tPosition[0], Y() - tPosition[1]));

  if (distance > distanceDie)
  {
    isTooFar = true;
  }
}

void Bullet::reset()
{
  isTooFar = false;
  distance = 0;
}