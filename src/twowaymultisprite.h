#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include <string>
#include <vector>
#include "drawable.h"

class TwoWayMultiSprite : public Drawable
{
public:

  TwoWayMultiSprite(const std::string&, const std::string&, const std::string&);
  TwoWayMultiSprite(const TwoWayMultiSprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Frame* getFrame() const;
  virtual const std::vector<Frame *> getframesBasedOnConditions() const;

  virtual ~TwoWayMultiSprite() {}

protected:

  std::vector<Frame *> frames;
  const std::vector<Frame *> frames1;
  const std::vector<Frame *> frames2;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;

  virtual void advanceFrame(Uint32 ticks);
};
#endif