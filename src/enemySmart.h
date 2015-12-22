#ifndef ENEMYSMART__H
#define ENEMYSMART__H
#include <string>
#include <vector>
#include "drawable.h"
#include "sprite.h"
#include "collisionStrategy.h"

class ExplodingSprite;

class EnemySmart : public Sprite
{
public:

  EnemySmart();
  EnemySmart(const EnemySmart&);
  virtual ~EnemySmart();

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  void reset();

  bool collidedWith(const Drawable* d) const
  {
    if (isDead == true)
    {
      return false;
    }
    return strategy->execute(*this, *d);
  }

  bool isExploding() { return explosion != NULL; }
  virtual void explode();

  bool isDead;

  int yLow;
  int yHigh;

protected:

  ExplodingSprite* explosion;

  EnemySmart& operator=(const EnemySmart&);

private:

  std::vector<CollisionStrategy*> strategies;
  CollisionStrategy* strategy;

  int minimumXDistanceToTriggerAttack;
};
#endif
