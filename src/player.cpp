#include "player.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "bulletPool.h"
#include "explodingSprite.h"
#include "collisionStrategy.h"

Player& Player::getInstance()
{
  static Player player = Player("player", "playerRight", "playerLeft", "playerRightUp", "playerLeftUp");
  return player;
}

Player::Player(const std::string& name, const std::string& name1, const std::string& name2, const std::string& name3, const std::string& name4)
  : TwoWayMultiSprite(name, name1, name2),
  HasWon(false),
  GodMode(false),
  MovingRight(true),
  frames3(FrameFactory::getInstance().getFrames(name3)),
  frames4(FrameFactory::getInstance().getFrames(name4)),
  isStopped(true),
  isJumping(false),
  countJumpFrames(0),
  explosion(NULL),
  strategies(),
  strategy(NULL)
{
  strategies.push_back(new MidPointCollisionStrategy);
  strategies.push_back(new RectangularCollisionStrategy);
  strategies.push_back(new PerPixelCollisionStrategy);
  strategy = strategies[2];

  stop();
}

Player::Player(const Player& s)
  : TwoWayMultiSprite(s),
  HasWon(s.HasWon),
  GodMode(s.GodMode),
  MovingRight(s.MovingRight),
  frames3(s.frames3),
  frames4(s.frames4),
  isStopped(s.isStopped),
  isJumping(s.isJumping),
  countJumpFrames(s.countJumpFrames),
  explosion(s.explosion),
  strategies(s.strategies),
  strategy(s.strategy)
{
  stop();
}

Player::~Player()
{
  for (unsigned i = 0; i < strategies.size(); ++i)
  {
    delete strategies[i];
  }
}

void Player::draw() const
{
  if (explosion)
  {
    explosion->draw();
    return;
  }

  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  getframesBasedOnConditions()[currentFrame]->draw(x, y);
}

void Player::update(Uint32 ticks)
{
  if (explosion)
  {
    explosion->update(ticks);
    if (explosion->chunkCount() == 0)
    {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

  advanceFrame(ticks);

  setPosition(getPosition() + getVelocity() * static_cast<float>(ticks)* 0.001);
  if (X() < 0)
  {
    velocityX(abs(velocityX()));
    MovingRight = true;
  }
  if (X() > worldWidth - frameWidth)
  {
    velocityX(-abs(velocityX()));
    MovingRight = false;
  }
  if (Y() < 0)
  {
    velocityY(abs(velocityY()));
    MovingRight = true;
  }
  if (Y() > worldHeight - frameHeight)
  {
    velocityY(-abs(velocityY()));
    MovingRight = false;
  }
}

void Player::advanceFrame(Uint32 ticks)
{
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval)
  {
    if (!isJumping)
    {
      currentFrame = (currentFrame + 1) % numberOfFrames;
    }
    else
    {
      if (countJumpFrames == numberOfFrames)
      {
        currentFrame = 0;
        isJumping = false;
      }
      else
      {
        currentFrame = countJumpFrames;
        countJumpFrames++;
      }
    }
    timeSinceLastFrame = 0;
  }
}

const Frame* Player::getFrame() const
{
  return getframesBasedOnConditions()[currentFrame];
}

const std::vector<Frame *> Player::getframesBasedOnConditions() const
{
  if (velocityX() > 0 && !isJumping)
    return frames1;
  else if (velocityX() < 0 && !isJumping)
    return frames2;
  else if (velocityX() > 0 && isJumping)
    return frames3;
  else if (velocityX() < 0 && isJumping)
    return frames4;
  else
    return frames;
}

void Player::stop()
{
  isJumping = false;
  isStopped = true;
  if (velocityX() > 0)
  {
    frames = frames1;
  }
  else if (velocityX() < 0)
  {
    frames = frames2;
  }
  currentFrame = 0;
  velocityX(0);
  GodMode = false;
}
void Player::right()
{
  isStopped = false;
  if (X() < worldWidth - frameWidth)
  {
    velocityX(velocityXWhenMoving());
    MovingRight = true;
  }
}
void Player::left()
{
  isStopped = false;
  if (X() > 0)
  {
    velocityX(-velocityXWhenMoving());
    MovingRight = false;
  }
}
void Player::up()
{
  if (!isJumping)
  {
    isJumping = true;
    countJumpFrames = 0;
    currentFrame = 0;
    if (velocityX() > 0)
      frames = frames3;
    else if (velocityX() < 0)
      frames = frames4;
  }
}
void Player::down()
{
  stop();
}
void Player::fire()
{
  BulletPool::getInstance().fire();
}

void Player::reset()
{
  if (explosion)
  {
    delete explosion;
    explosion = NULL;
  }
  isJumping = false;
  isStopped = true;
  MovingRight = true;
  frames = frames1;
  currentFrame = 0;
  velocityX(0);
  setPosition(Vector2f(Gamedata::getInstance().getXmlInt("player/startLoc/x"), Gamedata::getInstance().getXmlInt("player/startLoc/y")));
  HasWon = false;
}

void Player::explode()
{
  if (explosion)
  {
    return;
  }
  Sprite sprite(getName(), getPosition(), getVelocity(), getFrame());
  explosion = new ExplodingSprite(sprite);
}