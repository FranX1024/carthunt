TitleActivity::TitleActivity(Application* app) : Activity(app) {
  this->background = this->app->driver->getTexture("./res/img/title.png");
}

void TitleActivity::setup() {
  this->app->guienv->addImage(this->background, core::position2d<int>(0, 0));
  this->app->guienv->addButton(core::rect<s32>(300, 310, 500, 340), 0, 1, L"PLAY THE GAME");
  this->app->guienv->addButton(core::rect<s32>(300, 350, 500, 380), 0, 2, L"SCOREBOARD");
}

void TitleActivity::update() {
}

void TitleActivity::onevent(const SEvent &event) {
  if(event.EventType == EET_GUI_EVENT) {
    s32 id = event.GUIEvent.Caller->getID();
    if(event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED) {
      switch(id) {
      case 1:
	this->app->setActivity(GAME);
	break;
      case 2:
	this->app->setActivity(SCOREBOARD);
	break;
      }
    }
  }
}

void TitleActivity::drop() {
}
