#ifndef BULLET__H
#define BULLET__H
#include <string>
#include <vector>
#include "sprite.h"

class Bullet : public Sprite
{
public:

  Bullet();
  Bullet(const Bullet&);
  virtual ~Bullet();

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  void reset();

  bool goneTooFar() const { return isTooFar; }

  bool collidedWith(const Drawable* d) const;

protected:

  Bullet& operator=(const Bullet&);

private:

  float distance;
  float distanceDie;
  bool isTooFar;
};
#endif