#ifndef ENEMY__H
#define ENEMY__H
#include <string>
#include <vector>
#include "drawable.h"
#include "sprite.h"
#include "collisionStrategy.h"

class ExplodingSprite;

class Enemy : public Sprite
{
public:

  Enemy();
  Enemy(const Enemy&);
  virtual ~Enemy();

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

  bool isDead;

  bool isExploding() { return explosion != NULL; }
  virtual void explode();

protected:

  ExplodingSprite* explosion;

  Enemy& operator=(const Enemy&);

private:

  std::vector<CollisionStrategy*> strategies;
  CollisionStrategy* strategy;
};
#endif
