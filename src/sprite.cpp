#include <cmath>
#include "frameFactory.h"
#include "sprite.h"
#include "gamedata.h"

Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel) :
  Drawable(n, pos, vel),
  frame(FrameFactory::getInstance().getFrame(n)),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{
}

Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel, const Frame* frm) :
  Drawable(n, pos, vel),
  frame(frm),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{
}

Sprite::Sprite(const Sprite& s) :
  Drawable(s),
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{
}

Sprite& Sprite::operator=(const Sprite& rhs)
{
  Drawable::operator=(rhs);
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

void Sprite::draw() const
{
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y);
}

int Sprite::getDistance(const Sprite *obj) const
{
  return hypot(X() - obj->X(), Y() - obj->Y());
}

void Sprite::update(Uint32 ticks)
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
  if (Y() < 0)
  {
    velocityY(abs(velocityY()));
  }
  if (Y() > worldHeight - frameHeight)
  {
    velocityY(-abs(velocityY()));
  }
}
