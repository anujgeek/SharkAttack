#include "gamedata.h"
#include "frameFactory.h"
#include "twowaymultisprite.h"

TwoWayMultiSprite::TwoWayMultiSprite(const std::string& name, const std::string& name1, const std::string& name2)
  :Drawable(name, Vector2f(Gamedata::getInstance().getXmlInt(name + "/startLoc/x"), Gamedata::getInstance().getXmlInt(name + "/startLoc/y")), Vector2f(Gamedata::getInstance().getXmlInt(name + "/speedX"), Gamedata::getInstance().getXmlInt(name + "/speedY"))),
  frames(),
  frames1(FrameFactory::getInstance().getFrames(name1)),
  frames2(FrameFactory::getInstance().getFrames(name2)),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(0),
  numberOfFrames(Gamedata::getInstance().getXmlInt(name + "/frames")),
  frameInterval(Gamedata::getInstance().getXmlInt(name + "/frameInterval")),
  timeSinceLastFrame(0),
  frameWidth(frames1[0]->getWidth()),
  frameHeight(frames1[0]->getHeight())
{
  frames = frames1;
}

TwoWayMultiSprite::TwoWayMultiSprite(const TwoWayMultiSprite& s)
  :Drawable(s),
  frames(s.frames),
  frames1(s.frames1),
  frames2(s.frames2),
  worldWidth(s.worldWidth),
  worldHeight(s.worldHeight),
  currentFrame(s.currentFrame),
  numberOfFrames(s.numberOfFrames),
  frameInterval(s.frameInterval),
  timeSinceLastFrame(s.timeSinceLastFrame),
  frameWidth(s.frameWidth),
  frameHeight(s.frameHeight)
{

}

void TwoWayMultiSprite::draw() const
{
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  getframesBasedOnConditions()[currentFrame]->draw(x, y);
}

void TwoWayMultiSprite::update(Uint32 ticks)
{
  advanceFrame(ticks);

  setPosition(getPosition() + getVelocity() * static_cast<float>(ticks)* 0.001);
  if (X() < 0)
  {
    velocityX(abs(velocityX()));
  }
  if (X() > worldWidth - frameWidth)
  {
    velocityX(-abs(velocityX()));
  }
  if (Y() < 0)
  {
    velocityY(abs(velocityY()));
  }
  if (Y() > worldHeight - frameHeight)
  {
    velocityY(-abs(velocityY()));
  }
}

void TwoWayMultiSprite::advanceFrame(Uint32 ticks)
{
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval)
  {
    currentFrame = (currentFrame + 1) % numberOfFrames;
    timeSinceLastFrame = 0;
  }
}

const Frame* TwoWayMultiSprite::getFrame() const
{
  return getframesBasedOnConditions()[currentFrame];
}

const std::vector<Frame *> TwoWayMultiSprite::getframesBasedOnConditions() const
{
  if (velocityX() > 0)
    return frames1;
  else if (velocityX() < 0)
    return frames2;
  else
    return frames;
}