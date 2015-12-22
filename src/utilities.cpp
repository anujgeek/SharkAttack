#include <iostream>
#include <sstream>
#include <string>
#include "utilities.h"

Utilities& Utilities::getInstance()
{
  static Utilities utilities;
  return utilities;
}

Utilities::Utilities() :
  mainHUD("mainHUD"),
  poolHUD("poolHUD")
{

}

std::string Utilities::NumberToString(int Number)
{
  std::ostringstream ss;
  ss << Number;
  return ss.str();
}