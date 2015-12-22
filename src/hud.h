#ifndef HUD__H
#define HUD__H
#include <SDL.h>
#include <iostream>
#include <string>
#include "vector2f.h"
#include "ioManager.h"

class HUD
{
public:

  HUD(const std::string& n);
  HUD(const HUD&);
  HUD& operator=(const HUD&);
  ~HUD();

  virtual void draw() const;
  virtual void update();

  const std::string name;

  bool show;

private:

  const IOManager& io;
  SDL_Surface * const screen;

  const int HUD_WIDTH;
  const int HUD_HEIGHT;
  const Vector2f HUD_POSITION;

  unsigned int timeout;
  bool timeoutPassed;

  void draw(SDL_Surface* screen) const;
};
#endif