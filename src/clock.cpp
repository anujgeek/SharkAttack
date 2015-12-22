#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include "clock.h"
#include "gamedata.h"
#include "ioManager.h"

Clock& Clock::getInstance()
{
  if (SDL_WasInit(SDL_INIT_VIDEO) == 0)
  {
    throw std::string("Must init SDL before Clock");
  }
  static Clock clock;
  return clock;
}

Clock::Clock() :
  frameCount(0),
  ticksAtLastFrame(0),
  ticksSinceLastFrame(0),
  startTicks(0),
  pausedTicks(0),
  started(false),
  paused(false),
  isinSloMo(false),
  capFPS(Gamedata::getInstance().getXmlBool("clock/capFPS")),
  maxFPS(Gamedata::getInstance().getXmlInt("clock/maxFPS")),
  fpsPosition(Gamedata::getInstance().getXmlInt("clock/fpsPosition/x"),
    Gamedata::getInstance().getXmlInt("clock/fpsPosition/y")),
  secondsPosition(Gamedata::getInstance().getXmlInt("clock/secondsPosition/x"),
    Gamedata::getInstance().getXmlInt("clock/secondsPosition/y"))
{

}

void Clock::draw() const
{
  IOManager::getInstance().
    printMessageValueAt("Seconds: ", getSeconds(), fpsPosition[0], fpsPosition[1]);
  IOManager::getInstance().
    printMessageValueAt("fps: ", getFPS(), secondsPosition[0], secondsPosition[1]);
}

void Clock::update()
{
  if (paused) return;

  frameCount++;

  unsigned int currentTicks = get_ticks();
  ticksSinceLastFrame = currentTicks - ticksAtLastFrame;
  ticksAtLastFrame = currentTicks;
}

unsigned int Clock::get_ticks() const
{
  if (started == true)
  {
    if (paused == true)
    {
      return pausedTicks;
    }
    else
    {
      return SDL_GetTicks() - startTicks;
    }
  }
  return 0;
}

void Clock::start()
{
  started = true;
  paused = false;
  startTicks = SDL_GetTicks();
}

void Clock::stop()
{
  started = false;
  paused = false;
}

void Clock::pause()
{
  if ((started == true) && (paused == false))
  {
    paused = true;
    pausedTicks = SDL_GetTicks() - startTicks;
  }
}

void Clock::unpause()
{
  if (paused == true)
  {
    paused = false;
    startTicks = SDL_GetTicks() - pausedTicks;
    pausedTicks = 0;
  }
}

void Clock::toggleSloMo()
{
  isinSloMo = !isinSloMo;
}

unsigned int Clock::getFPS() const
{
  if (paused == false && frameCount > 0) return  frameCount / (get_ticks() / 1000.f);
  return 0;
}