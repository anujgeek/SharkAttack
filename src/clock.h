#include <SDL.h>
#include <string>
#include <deque>
#include "vector2f.h"

class Manager;

class Clock
{
public:
  static Clock& getInstance();  // This class is a Singleton

  unsigned int get_ticks() const;
  void start();
  void stop();
  void pause();
  void unpause();
  bool isStarted() const { return started; }
  bool isPaused() const { return paused; }
  void toggleSloMo();
  unsigned int getFPS() const;
  unsigned int getSeconds() const { return get_ticks() / 1000; }
  void draw() const;

private:

  unsigned int frameCount;
  unsigned int ticksAtLastFrame;
  unsigned int ticksSinceLastFrame;
  unsigned int startTicks;
  unsigned int pausedTicks;
  bool started;
  bool paused;
  bool isinSloMo;
  bool capFPS;
  unsigned int maxFPS;

  Vector2f fpsPosition;
  Vector2f secondsPosition;

  Clock();
  Clock(const Clock&);
  Clock& operator=(const Clock&);
  friend class Manager;
  void update();
  unsigned int getTicksSinceLastFrame() const { return (!isinSloMo) ? ticksSinceLastFrame : (ticksSinceLastFrame / 5); };
};