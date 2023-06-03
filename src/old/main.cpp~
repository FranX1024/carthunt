#include <irrlicht.h>

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#include <math.h>
#include <stdio.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

enum GameState {
  TITLESCREEN, // GAMESCREEN
} gameState;

#include "irrobjects.hpp"
#include "title.hpp"
#include "game.hpp"
#include "event_receiver.hpp"

#include "title.cpp"
#include "game.cpp"
#include "event_receiver.cpp"


int main() {
  irrDevice = createDevice( video::EDT_OPENGL, dimension2d<u32>(800, 600), 16,
		  false, false, false, &event_receiver);
  if (!irrDevice) return 1;
  irrDevice->setWindowCaption(L"Barbapapa game");

  // get devices
  irrDriver = irrDevice->getVideoDriver();
  irrSmgr = irrDevice->getSceneManager();
  irrGuienv = irrDevice->getGUIEnvironment();
  
  // set font
  IGUISkin* gskin = irrGuienv->getSkin();
  IGUIFont* font = irrGuienv->getFont("./res/img/fontlucida.png");
  if(font) gskin->setFont(font);
  else {
    printf("ERROR: Missing font, using built in font instead.\n");
    gskin->setFont(irrGuienv->getBuiltInFont(), EGDF_TOOLTIP);
  }

  // init
  init_title();
  
  // create camera
  irrCamera = irrSmgr->addCameraSceneNode(0, vector3df(0, 0, 0), vector3df(0, 0, 1));
  
  while(irrDevice->run()) {
    irrDriver->beginScene(true, true, SColor(100, 100, 100, 100));
    switch(gameState) {
    case TITLESCREEN: update_title(); break;
      //case GAMESCREEN: update_game(); break;
    }
    // draw
    irrSmgr->drawAll();
    irrGuienv->drawAll();
    irrDriver->endScene();
  }
  // free all
  irrDevice->drop();
  return 0;
}
