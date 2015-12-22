#include "hud.h"
#include <cmath>
#include <iostream>
#include <string>
#include "clock.h"
#include "gamedata.h"
#include "ioManager.h"
#include "aaline.h"
#include "utilities.h"
#include "bulletPool.h"
#include "player.h"

HUD::HUD(const std::string& n) :
  name(n),
  show(true),
  io(IOManager::getInstance()),
  screen(io.getScreen()),
  HUD_WIDTH(Gamedata::getInstance().getXmlInt(n + "/width")),
  HUD_HEIGHT(Gamedata::getInstance().getXmlInt(n + "/height")),
  HUD_POSITION(Vector2f(Gamedata::getInstance().getXmlInt(n + "/loc/x"), Gamedata::getInstance().getXmlInt(n + "/loc/y"))),
  timeout(Gamedata::getInstance().getXmlInt(n + "/timeout")),
  timeoutPassed(false)
{
}

HUD::HUD(const HUD& hud) :
  name(hud.name),
  show(hud.show),
  io(hud.io),
  screen(hud.screen),
  HUD_WIDTH(hud.HUD_WIDTH),
  HUD_HEIGHT(hud.HUD_HEIGHT),
  HUD_POSITION(Vector2f(hud.HUD_POSITION[0], hud.HUD_POSITION[1])),
  timeout(hud.timeout),
  timeoutPassed(hud.timeoutPassed)
{
}

HUD::~HUD()
{

}

void HUD::draw() const
{
  if (show)
  {
    draw(screen);
  }

  if (name == Utilities::getInstance().mainHUD)
  {
    io.printMessageAt("Shark Attack", HUD_POSITION[0] + 5, HUD_POSITION[1] + 5);
    io.printMessageAt("By Anuj Agrawal", HUD_POSITION[0] + 5, HUD_POSITION[1] + 20);
  }

  if (Player::getInstance().HasWon == true)
  {
    io.printMessageCenteredAt("You WON. Press 'R' to restart.", 130);
  }
}

void HUD::update()
{
  if (timeoutPassed == false && Clock::getInstance().getSeconds() > timeout)
  {
    timeoutPassed = true;
    show = false;
  }
}


void HUD::draw(SDL_Surface* screen) const
{
  const Uint32 RED = SDL_MapRGB(screen->format, 0xff, 0, 0);
  Draw_AALine(screen, HUD_POSITION[0], HUD_POSITION[1] + HUD_HEIGHT / 2, HUD_POSITION[0] + HUD_WIDTH, HUD_POSITION[1] + HUD_HEIGHT / 2, HUD_HEIGHT, 0xff, 0xff, 0xff, 0xff / 2);
  Draw_AALine(screen, HUD_POSITION[0], HUD_POSITION[1], HUD_POSITION[0] + HUD_WIDTH, HUD_POSITION[1], RED);
  Draw_AALine(screen, HUD_POSITION[0], HUD_POSITION[1], HUD_POSITION[0], HUD_POSITION[1] + HUD_HEIGHT, RED);
  Draw_AALine(screen, HUD_POSITION[0] + HUD_WIDTH, HUD_POSITION[1] + HUD_HEIGHT, HUD_POSITION[0] + HUD_WIDTH, HUD_POSITION[1], RED);
  Draw_AALine(screen, HUD_POSITION[0] + HUD_WIDTH, HUD_POSITION[1] + HUD_HEIGHT, HUD_POSITION[0], HUD_POSITION[1] + HUD_HEIGHT, RED);

  if (name == Utilities::getInstance().mainHUD)
  {
    io.printMessageAt("Press a - Left", HUD_POSITION[0] + 5, HUD_POSITION[1] + 75);
    io.printMessageAt("Press d - Right", HUD_POSITION[0] + 5, HUD_POSITION[1] + 90);
    io.printMessageAt("Press s - Stop", HUD_POSITION[0] + 5, HUD_POSITION[1] + 105);
    io.printMessageAt("Press w or Space - Jump", HUD_POSITION[0] + 5, HUD_POSITION[1] + 120);
    io.printMessageAt("Press f - Fire", HUD_POSITION[0] + 5, HUD_POSITION[1] + 135);
    io.printMessageAt("Press F1 - Toggle HUD", HUD_POSITION[0] + 5, HUD_POSITION[1] + 170);
    io.printMessageAt("Press p - Toggle Pause", HUD_POSITION[0] + 5, HUD_POSITION[1] + 190);
    io.printMessageAt("Press o - Toggle SloMo", HUD_POSITION[0] + 5, HUD_POSITION[1] + 210);
    if (Player::getInstance().GodMode == true)
    {
      io.printMessageAt("Press g - GodMode - ON", HUD_POSITION[0] + 5, HUD_POSITION[1] + 230);
    }
    else
    {
      io.printMessageAt("Press g - GodMode - OFF", HUD_POSITION[0] + 5, HUD_POSITION[1] + 230);
    }
    io.printMessageAt("Press r - Restart", HUD_POSITION[0] + 5, HUD_POSITION[1] + 250);

    io.printMessageAt("Bullets", HUD_POSITION[0] + 5, HUD_POSITION[1] + 280);
    io.printMessageAt("In Use: " + Utilities::getInstance().NumberToString(BulletPool::getInstance().inuseListCount()), HUD_POSITION[0] + 15, HUD_POSITION[1] + 300);
    io.printMessageAt("Free: " + Utilities::getInstance().NumberToString(BulletPool::getInstance().freeListCount()), HUD_POSITION[0] + 15, HUD_POSITION[1] + 315);
  }
  else if (name == Utilities::getInstance().poolHUD)
  {
    io.printMessageAt("Press F2 - Pool HUD", HUD_POSITION[0] + 5, HUD_POSITION[1] + 5);
    io.printMessageAt("Bullets", HUD_POSITION[0] + 5, HUD_POSITION[1] + 30);
    io.printMessageAt("In Use: " + Utilities::getInstance().NumberToString(BulletPool::getInstance().inuseListCount()), HUD_POSITION[0] + 15, HUD_POSITION[1] + 50);
    io.printMessageAt("Free: " + Utilities::getInstance().NumberToString(BulletPool::getInstance().freeListCount()), HUD_POSITION[0] + 15, HUD_POSITION[1] + 65);
  }
}