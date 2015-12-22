#include "frameFactory.h"
#include "extractSurface.h"
#include "ioManager.h"
#include "vector2f.h"

FrameFactory::~FrameFactory()
{
  for (std::map<std::string, SDL_Surface*>::iterator it = surfaces.begin(); it != surfaces.end(); ++it)
  {
    SDL_FreeSurface(it->second);
  }
  for (std::map<std::string, std::vector<SDL_Surface*> >::iterator it1 = multiSurfaces.begin(); it1 != multiSurfaces.end(); ++it1)
  {
    for (std::vector<SDL_Surface*>::iterator it2 = (it1->second).begin(); it2 != (it1->second).end(); ++it2)
    {
      SDL_FreeSurface(*it2);
    }
  }
  for (std::map<std::string, Frame*>::iterator it = frames.begin(); it != frames.end(); ++it)
  {
    delete it->second;
  }
  for (std::map<std::string, std::vector<Frame*> >::iterator it1 = multiFrames.begin(); it1 != multiFrames.end(); ++it1)
  {
    for (std::vector<Frame*>::iterator it2 = (it1->second).begin(); it2 != (it1->second).end(); ++it2)
    {
      delete *it2;
    }
  }
}

FrameFactory& FrameFactory::getInstance()
{
  static FrameFactory factory;
  return factory;
}

Frame* FrameFactory::getFrame(const std::string& name)
{
  std::map<std::string, Frame*>::const_iterator pos = frames.find(name);
  if (pos == frames.end())
  {
    SDL_Surface * const surface =
      IOManager::getInstance().loadAndSet(
        gdata.getXmlStr(name + "/file"),
        gdata.getXmlBool(name + "/transparency"));
    surfaces[name] = surface;
    Frame * const frame = new Frame(name, surface);
    frames[name] = frame;
    return frame;
  }
  else
  {
    return pos->second;
  }
}

std::vector<Frame*> FrameFactory::getFrames(const std::string& name)
{
  // First search map to see if we've already made it:
  std::map<std::string, std::vector<Frame*> >::const_iterator
    pos = multiFrames.find(name);
  if (pos != multiFrames.end())
  {
    return pos->second;
  }

  // It wasn't in the map, so we have to make the vector of Frames:
  SDL_Surface* surface = IOManager::
    getInstance().loadAndSet(gdata.getXmlStr(name + "/file"), true);
  unsigned numberOfFrames = gdata.getXmlInt(name + "/frames");
  std::vector<Frame*> frames;
  std::vector<SDL_Surface*> surfaces;
  frames.reserve(numberOfFrames);
  Uint16 srcX = gdata.getXmlInt(name + "/srcX");
  Uint16 srcY = gdata.getXmlInt(name + "/srcY");
  Uint16 width = gdata.getXmlInt(name + "/width");
  Uint16 height = gdata.getXmlInt(name + "/height");

  SDL_Surface* surf;
  for (unsigned i = 0; i < numberOfFrames; ++i)
  {
    unsigned frameX = i * width + srcX;
    surf = ExtractSurface::getInstance().
      get(surface, width, height, frameX, srcY);
    surfaces.push_back(surf);
    frames.push_back(new Frame(name, surf));
  }
  SDL_FreeSurface(surface);
  multiSurfaces[name] = surfaces;
  multiFrames[name] = frames;
  return frames;
}
