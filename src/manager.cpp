#include <iostream>
#include <string>
#include <iomanip>
#include "extractSurface.h"
#include "vector2f.h"
#include "drawable.h"
#include "sprite.h"
#include "multisprite.h"
#include "twowaymultisprite.h"
#include "explodingSprite.h"
#include "player.h"
#include "enemy.h"
#include "enemySmart.h"
#include "gamedata.h"
#include "hud.h"
#include "bulletPool.h"
#include "manager.h"

Manager::~Manager()
{
  std::list<Drawable*>::const_iterator ptr1 = sprites.begin();
  while (ptr1 != sprites.end())
  {
    delete (*ptr1);
    ++ptr1;
  }
  std::list<Drawable*>::const_iterator ptr2 = spritesBird.begin();
  while (ptr2 != spritesBird.end())
  {
    delete (*ptr2);
    ++ptr2;
  }
  std::list<Drawable*>::const_iterator ptr3 = spritesBirdSmall.begin();
  while (ptr3 != spritesBirdSmall.end())
  {
    delete (*ptr3);
    ++ptr3;
  }
  for (unsigned i = 0; i < enemies.size(); ++i)
  {
    delete enemies[i];
  }
  for (unsigned i = 0; i < enemiesSmart.size(); ++i)
  {
    delete enemiesSmart[i];
  }
  delete santa;
}

Manager::Manager() :
  env(SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center"))),
  io(IOManager::getInstance()),
  clock(Clock::getInstance()),
  bulletPool(BulletPool::getInstance()),
  screen(io.getScreen()),
  bgSky("bgSky", Gamedata::getInstance().getXmlInt("bgSky/factor")),
  bgClouds("bgClouds", Gamedata::getInstance().getXmlInt("bgClouds/factor")),
  bgHillBack("bgHillBack", Gamedata::getInstance().getXmlInt("bgHillBack/factor")),
  bgHillFront("bgHillFront", Gamedata::getInstance().getXmlInt("bgHillFront/factor")),
  bgWater("bgWater", Gamedata::getInstance().getXmlInt("bgWater/factor")),
  viewport(Viewport::getInstance()),
  sprites(),
  spritesBird(),
  spritesBirdSmall(),
  enemies(),
  enemiesSmart(),
  santa(),
  currentSprite(),
  makeVideo(false),
  frameCount(0),
  username(Gamedata::getInstance().getXmlStr("username")),
  title(Gamedata::getInstance().getXmlStr("screenTitle")),
  frameMax(Gamedata::getInstance().getXmlInt("frameMax")),
  mainHUD(HUD("mainHUD"))
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);

  for (unsigned i = 0; i < 10; ++i)
  {
    TwoWayMultiSprite* bird = new TwoWayMultiSprite("BirdBlue", "BirdBlueRight", "BirdBlueLeft");
    bird->X(i * 100);
    bird->Y(bird->Y() + (i % 2 == 0 ? 0 : 25));
    spritesBird.push_back(bird);
  }

  for (unsigned i = 0; i < 10; ++i)
  {
    TwoWayMultiSprite* birdSmall = new TwoWayMultiSprite("BirdGray", "BirdGrayRight", "BirdGrayLeft");
    birdSmall->X(i * 250);
    birdSmall->Y(birdSmall->Y() + (i % 2 == 0 ? 0 : 10));
    spritesBirdSmall.push_back(birdSmall);
  }

  for (unsigned i = 0; i < 4; ++i)
  {
    Enemy* enemy = new Enemy();
    enemy->X(750 + i * 750);
    enemies.push_back(enemy);
  }

  for (unsigned i = 0; i < 3; ++i)
  {
    EnemySmart* enemy = new EnemySmart();
    enemy->X(1000 + i * 800);
    enemy->reset();
    enemiesSmart.push_back(enemy);
  }

  santa = new Sprite("santa", Vector2f(Gamedata::getInstance().getXmlInt("santa/startLoc/x"), Gamedata::getInstance().getXmlInt("santa/startLoc/y")), Vector2f(Gamedata::getInstance().getXmlInt("santa/speedX"), Gamedata::getInstance().getXmlInt("santa/speedY")));

  viewport.setObjectToTrack(&Player::getInstance());
}

void Manager::draw() const
{
  bgSky.draw();
  std::list<Drawable*>::const_iterator ptr3 = spritesBirdSmall.begin();
  while (ptr3 != spritesBirdSmall.end())
  {
    (*ptr3)->draw();
    ++ptr3;
  }
  bgClouds.draw();
  std::list<Drawable*>::const_iterator ptr2 = spritesBird.begin();
  while (ptr2 != spritesBird.end())
  {
    (*ptr2)->draw();
    ++ptr2;
  }
  bgHillBack.draw();
  bgHillFront.draw();
  bgWater.draw();
  santa->draw();
  std::list<Drawable*>::const_iterator ptr1 = sprites.begin();
  while (ptr1 != sprites.end())
  {
    (*ptr1)->draw();
    ++ptr1;
  }
  bulletPool.draw();
  for (unsigned i = 0; i < enemies.size(); ++i)
  {
    enemies[i]->draw();
  }
  for (unsigned i = 0; i < enemiesSmart.size(); ++i)
  {
    enemiesSmart[i]->draw();
  }
  Player::getInstance().draw();
  mainHUD.draw();
  clock.draw();

  viewport.draw();

  SDL_Flip(screen);
}

void Manager::makeFrame()
{
  std::stringstream strm;
  strm << "frames/" << username << '.'
    << std::setfill('0') << std::setw(4)
    << frameCount++ << ".bmp";
  std::string filename(strm.str());
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::update()
{
  clock.update();
  Uint32 ticks = clock.getTicksSinceLastFrame();

  mainHUD.update();

  santa->update(ticks);
  std::list<Drawable*>::const_iterator ptr1 = sprites.begin();
  while (ptr1 != sprites.end())
  {
    (*ptr1)->update(ticks);
    ++ptr1;
  }
  std::list<Drawable*>::const_iterator ptr2 = spritesBird.begin();
  while (ptr2 != spritesBird.end())
  {
    (*ptr2)->update(ticks);
    ++ptr2;
  }
  std::list<Drawable*>::const_iterator ptr3 = spritesBirdSmall.begin();
  while (ptr3 != spritesBirdSmall.end())
  {
    (*ptr3)->update(ticks);
    ++ptr3;
  }
  for (unsigned i = 0; i < enemies.size(); ++i)
  {
    enemies[i]->update(ticks);
  }
  for (unsigned i = 0; i < enemiesSmart.size(); ++i)
  {
    enemiesSmart[i]->update(ticks);
  }
  bulletPool.update(ticks);
  Player::getInstance().update(ticks);
  checkForCollisions();

  if (makeVideo && frameCount < frameMax)
  {
    makeFrame();
  }

  bgSky.update();
  bgClouds.update();
  bgHillBack.update();
  bgHillFront.update();
  bgWater.update();

  viewport.update();
}

void Manager::checkForCollisions()
{
  static SDLSound sound;

  if (Player::getInstance().HasWon == false && Player::getInstance().collidedWith(santa))
  {
    sound[5];
    Player::getInstance().HasWon = true;
  }

  for (unsigned i = 0; i < enemies.size(); ++i)
  {
    if (Player::getInstance().GodMode == false && Player::getInstance().HasWon == false && enemies[i]->isDead == false && Player::getInstance().isExploding() == false && enemies[i]->isExploding() == false && Player::getInstance().collidedWith(enemies[i]))
    {
      sound[4];
      Player::getInstance().explode();
    }

    if (enemies[i]->isDead == false && enemies[i]->isExploding() == false && bulletPool.collidedWith(enemies[i]))
    {
      sound[3];
      enemies[i]->isDead = true;
      enemies[i]->explode();
    }
  }
  if (Player::getInstance().GodMode == false && Player::getInstance().HasWon == false)
  {
    for (unsigned i = 0; i < enemiesSmart.size(); ++i)
    {
      if (enemiesSmart[i]->isDead == false && Player::getInstance().isExploding() == false && Player::getInstance().collidedWith(enemiesSmart[i]))
      {
        sound[4];
        Player::getInstance().explode();
      }
    }
  }
}

void Manager::restart()
{
  Player::getInstance().reset();

  for (unsigned i = 0; i < enemies.size(); ++i)
  {
    enemies[i]->reset();
    enemies[i]->X(750 + i * 750);
  }
  for (unsigned i = 0; i < enemiesSmart.size(); ++i)
  {
    enemiesSmart[i]->reset();
    enemiesSmart[i]->X(1000 + i * 800);
  }
}

void Manager::play()
{
  static SDLSound sound;
  SDL_Event event;
  bool done = false;

  float lastTime = 0.0f;
  float timeBehind = 0.0f;
  float targetTimeStep = 1000.0f / clock.maxFPS;

  clock.start();

  while (!done)
  {
    while (SDL_PollEvent(&event))
    {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type == SDL_QUIT) { done = true; break; }
      if (event.type == SDL_KEYDOWN)
      {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q])
        {
          done = true;
          break;
        }
        if (keystate[SDLK_p])
        {
          sound[0];
          clock.isPaused() ? clock.unpause() : clock.pause();
        }
        if (keystate[SDLK_o])
        {
          clock.toggleSloMo();
        }
        if (keystate[SDLK_F4] && !makeVideo)
        {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
        if (keystate[SDLK_F1])
        {
          mainHUD.show = !mainHUD.show;
        }

        if (keystate[SDLK_a] && keystate[SDLK_d])
        {
          Player::getInstance().stop();
        }
        else if (keystate[SDLK_a])
        {
          Player::getInstance().left();
        }
        else if (keystate[SDLK_s])
        {
          Player::getInstance().down();
        }
        else if (keystate[SDLK_d])
        {
          Player::getInstance().right();
        }
        else if (keystate[SDLK_w] || keystate[SDLK_SPACE])
        {
          if (Player::getInstance().isExploding() == false && Player::getInstance().getIsStopped() == false)
          {
            sound[1];
            Player::getInstance().up();
          }
        }

        if (keystate[SDLK_f])
        {
          if (Player::getInstance().isExploding() == false)
          {
            sound[2];
            Player::getInstance().fire();
          }
        }

        if (keystate[SDLK_g])
        {
          sound[0];
          Player::getInstance().GodMode = !Player::getInstance().GodMode;
        }

        if (keystate[SDLK_r])
        {
          sound[0];
          restart();
        }
      }
    }

    draw();
    if (clock.capFPS)
    {
      timeBehind += clock.get_ticks() - lastTime;
      lastTime = clock.get_ticks();
      while (timeBehind >= targetTimeStep)
      {
        update();
        timeBehind -= targetTimeStep;
      }
    }
    else
    {
      update();
    }
  }
}
