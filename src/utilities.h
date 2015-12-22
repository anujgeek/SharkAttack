#ifndef UTILITIES__H
#define UTILITIES__H
#include <iostream>
#include <string>

class Utilities
{
public:
  static Utilities& getInstance();

  const  std::string mainHUD;
  const  std::string poolHUD;

  std::string NumberToString(int Number);

private:

  Utilities();
  Utilities(const Utilities&);
  Utilities& operator=(const Utilities&);
};
#endif