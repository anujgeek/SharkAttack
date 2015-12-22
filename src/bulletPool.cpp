#include "hud.h"
#include "player.h"
#include "bullet.h"
#include "bulletPool.h"

BulletPool& BulletPool::getInstance()
{
  static BulletPool bulletPool;
  return bulletPool;
}

BulletPool::BulletPool() :
  name("bullet"),
  freeList(),
  inuseList(),
  frameInterval(Gamedata::getInstance().getXmlInt("bullet/frameInterval")),
  timeSinceLastFrame(0),
  strategy(new MidPointCollisionStrategy)
{

}

BulletPool::~BulletPool()
{
  std::list<Bullet*>::iterator it1 = freeList.begin();
  while (it1 != freeList.end())
  {
    delete *it1;
    ++it1;
  }
  std::list<Bullet*>::iterator it2 = inuseList.begin();
  while (it2 != inuseList.end())
  {
    delete *it2;
    ++it2;
  }
}

Bullet* BulletPool::getBullet()
{
  if (freeList.empty())
  {
    Bullet* bullet = new Bullet();
    inuseList.push_back(bullet);
    return bullet;
  }
  else
  {
    Bullet* bullet = freeList.front();
    freeList.pop_front();
    bullet->reset();
    inuseList.push_back(bullet);
    return bullet;
  }
}

void BulletPool::draw() const
{
  std::list<Bullet*>::const_iterator it = inuseList.begin();
  while (it != inuseList.end())
  {
    (*it)->draw();
    ++it;
  }
}

void BulletPool::update(Uint32 ticks)
{
  timeSinceLastFrame += ticks;

  std::list<Bullet*>::iterator it = inuseList.begin();
  while (it != inuseList.end())
  {
    (*it)->update(ticks);
    if ((*it)->goneTooFar())
    {
      freeList.push_back(*it);
      it = inuseList.erase(it);
    }
    else
    {
      it++;
    }
  }
}

void BulletPool::fire()
{
  if (timeSinceLastFrame > frameInterval)
  {
    if (freeList.empty())
    {
      Bullet* b = new Bullet();
      if (Player::getInstance().MovingRight == true)
      {
        b->velocityX(abs(b->velocityXWhenMoving()));
        b->X(Player::getInstance().getPosition()[0] + 120);
      }
      else
      {
        b->velocityX(-abs(b->velocityXWhenMoving()));
        b->X(Player::getInstance().getPosition()[0]);
      }
      inuseList.push_back(b);
    }
    else
    {
      Bullet* b = freeList.front();
      freeList.pop_front();
      b->reset();

      if (Player::getInstance().MovingRight == true)
      {
        b->velocityX(abs(b->velocityXWhenMoving()));
        b->X(Player::getInstance().getPosition()[0] + 120);
      }
      else
      {
        b->velocityX(-abs(b->velocityXWhenMoving()));
        b->X(Player::getInstance().getPosition()[0]);
      }

      inuseList.push_back(b);
    }
    timeSinceLastFrame = 0;
  }
}

bool BulletPool::collidedWith(const Drawable* d) const
{
  std::list<Bullet*>::iterator it = inuseList.begin();
  while (it != inuseList.end())
  {
    if (strategy->execute(*(*it), *d))
    {
      return true;
    }
    ++it;
  }
  return false;
}