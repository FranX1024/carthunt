#include <irrlicht.h>
#include <vector>
#include <queue>
#include <set>
#include <utility>
#include <algorithm>
#include <string.h>
#include <wchar.h>

#define squareNum(x) x*x

double sign(double x) {
  if(x == 0) return 0;
  else if(x > 0) return 1;
  else return -1;
}

using namespace irr;

#include "tilemap.hpp"

#include "activity.hpp"
#include "title.hpp"
#include "map.hpp"
#include "ball.hpp"
#include "player.hpp"
#include "storeguy.hpp"
#include "game.hpp"
#include "application.hpp"

#include "application.cpp"
#include "storeguy.cpp"
#include "player.cpp"
#include "ball.cpp"
#include "map.cpp"
#include "game.cpp"
#include "title.cpp"
#include "activity.cpp"

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main() {
  Application app = Application();
  if(app.status != 0) {
    return app.status;
  }
  app.mainloop();
  app.drop();
  return 0;
}
