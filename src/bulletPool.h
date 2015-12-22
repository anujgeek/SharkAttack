#ifndef BULLETPOOL__H
#define BULLETPOOL__H
#include <iostream>
#include <string>
#include <list>
#include "drawable.h"
#include "collisionStrategy.h"
#include "bullet.h"

class BulletPool
{
public:

  static BulletPool& getInstance();

  ~BulletPool();

  Bullet* getBullet();

  int freeListCount() { return freeList.size(); }
  int inuseListCount() { return inuseList.size(); }

  std::list<Bullet*> getBulletsInUse() { return inuseList; }

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  void fire();

  bool collidedWith(const Drawable* obj) const;

private:

  std::string name;

  mutable std::list<Bullet*> freeList;
  mutable std::list<Bullet*> inuseList;

  float frameInterval;
  float timeSinceLastFrame;

  CollisionStrategy* strategy;

  BulletPool();
  BulletPool(const BulletPool&);
  BulletPool& operator=(const BulletPool&);

};
#endif