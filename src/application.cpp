Application::Application() {
  
  memset(this->keydown, 0, KEY_KEY_CODES_COUNT);
  memset(this->activity, 0, ACTIVITY_COUNT * sizeof(Activity*));
  this->current_activity = TITLE; // createDevice launches an event
  
  this->device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800, 450),
			      16, false, false, false, this);
  this->status = 0;
  if(!this->device) {
    printf("Error! Device did not initialize properly!\n");
    this->status = 1;
    return;
  }
  this->device->setWindowCaption(L"Cart Hunt");
  this->device->setResizable(false);

  this->driver = this->device->getVideoDriver();
  this->smgr = this->device->getSceneManager();
  this->guienv = this->device->getGUIEnvironment();

  // set font
  gui::IGUISkin* gskin = this->guienv->getSkin();
  gui::IGUIFont* font = this->guienv->getFont("./res/img/comicz.png");
  if(font) gskin->setFont(font);
  else {
    printf("WARNING: Missing font, using built-in font instead.\n");
  }

  // make buttons not transparent
  for (s32 i=0; i<irr::gui::EGDC_COUNT ; ++i) {
    video::SColor col = gskin->getColor((gui::EGUI_DEFAULT_COLOR)i);
    col.setAlpha(255);
    gskin->setColor((gui::EGUI_DEFAULT_COLOR)i, col);
  }

  // activites
  this->activity[TITLE] = new TitleActivity(this);
  Scoreboard* scoreboard = new Scoreboard(this);
  this->activity[SCOREBOARD] = scoreboard;
  Game* game_obj = new Game(this);
  game_obj->scoreboard = scoreboard;
  this->activity[GAME] = game_obj;

  // in case game didn't init properly, kill program
  if(!game_obj->successfully_initialized) {
    printf("Error! Game did not initialize properly!\n");
    this->status = 1;
    return;
  }
  
  this->setActivity(TITLE);
}

void Application::drop() {
  for(int i = 0; i < ACTIVITY_COUNT; i++) {
    if(this->activity[i] == NULL) continue;
    this->activity[i]->drop();
    delete this->activity[i];
  }
  this->device->drop();
}

void Application::setActivity(ActivityID new_activity) {
  this->guienv->clear();
  this->smgr->clear();
  
  this->current_activity = new_activity;
  this->activity[new_activity]->setup();
}

void Application::mainloop() {
  while(this->device->run()) {
    this->driver->beginScene(true, true, video::SColor(255, 0, 0, 0));
    this->activity[this->current_activity]->update();
    this->smgr->drawAll();
    this->guienv->drawAll();
    this->driver->endScene();
  }
}

bool Application::OnEvent(const SEvent& event) {
  if(event.EventType == EET_KEY_INPUT_EVENT) {
    this->keydown[event.KeyInput.Key] = event.KeyInput.PressedDown;
  } else {
    if(this->activity[this->current_activity] != NULL)
      this->activity[this->current_activity]->onevent(event);
  }
  return false;
}
