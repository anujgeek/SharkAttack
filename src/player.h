#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include "drawable.h"
#include "twowaymultisprite.h"
#include "collisionStrategy.h"

class ExplodingSprite;

class Player : public TwoWayMultiSprite
{
public:

  static Player& getInstance();
  virtual ~Player();

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Frame* getFrame() const;
  virtual const std::vector<Frame *> getframesBasedOnConditions() const;

  void up();
  void down();
  void left();
  void right();
  void stop();
  void fire();
  void reset();

  bool getIsStopped() { return isStopped; }

  bool collidedWith(const Drawable* d) const
  {
    return strategy->execute(Player::getInstance(), *d);
  }

  bool isExploding() { return explosion != NULL; }
  virtual void explode();

  bool HasWon;
  bool GodMode;

  bool MovingRight;

protected:

  const std::vector<Frame *> frames3;
  const std::vector<Frame *> frames4;

  bool isStopped;

  bool isJumping;
  unsigned int countJumpFrames;

  ExplodingSprite* explosion;

  Player& operator=(const Player&);
  virtual void advanceFrame(Uint32 ticks);

private:

  Player(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
  Player(const Player&);

  std::vector<CollisionStrategy*> strategies;
  CollisionStrategy* strategy;
};
#endif