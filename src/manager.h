#include <list>
#include <vector>
#include <SDL.h>
#include "sound.h"
#include "clock.h"
#include "ioManager.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "bulletPool.h"
#include "player.h"
#include "enemy.h"
#include "enemySmart.h"

class Manager
{
public:
  Manager();
  ~Manager();
  void play();

private:

  const bool env;
  const IOManager& io;
  Clock& clock;
  BulletPool& bulletPool;

  SDL_Surface * const screen;

  World bgSky;
  World bgClouds;
  World bgHillBack;
  World bgHillFront;
  World bgWater;
  Viewport& viewport;

  std::list<Drawable*> sprites;
  std::list<Drawable*> spritesBird;
  std::list<Drawable*> spritesBirdSmall;
  std::vector<Enemy*> enemies;
  std::vector<EnemySmart*> enemiesSmart;
  Sprite* santa;

  std::list<Drawable*>::iterator currentSprite;

  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;

  HUD mainHUD;

  void draw() const;
  void update();

  void restart();

  void checkForCollisions();

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
