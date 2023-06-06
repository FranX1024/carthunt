Scoreboard::Scoreboard(Application* app) : Activity(app) {
  this->background = this->app->driver->getTexture("./res/img/scoreboard.png");
  
  FILE* sbfile = fopen("./scores", "rb");
  if(sbfile == NULL) {
    printf("ERROR: Will not be able to save scores!\n");
    return;
  }
  fread((char*) this->scores, 1, sizeof(this->scores), sbfile);
  fclose(sbfile);

  this->new_score = -1;
}

void Scoreboard::setup() {
  this->app->guienv->addImage(this->background, core::position2d<int>(0, 0));
  
  // return to title screen button
  this->app->guienv->addButton(core::rect<s32>(10, 10, 40, 40), 0, 1, L"<<");
  
  // if new score, make room
  if(this->new_score != -1) {
    this->new_rank = 9;
    for(int i = 0; i < 10; i++) {
      if(this->scores[i].score <= this->new_score) {
	this->new_rank = i;
	break;
      }
    }
    this->kicked_out = this->scores[9];
    for(int i = 9; i > this->new_rank; i--) {
      this->scores[i] = this->scores[i-1];
    }
    std::time_t real_time = std::time(0);
    std::tm* local_time = std::localtime(&real_time);
    
    swprintf(this->scores[this->new_rank].date, 11, L"%02d/%02d/%d",
	     local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900);
    
    swprintf(this->scores[this->new_rank].name, MAX_NAME_LENGTH, L"");
    
    this->scores[this->new_rank].score = this->new_score;
  }
  
  // write scoreboard entries
  for(int i = 0; i < 10; i++) {
    int x = 100;
    int y = 80 + 35 * i;
    gui::IGUIStaticText* rank;
    gui::IGUIStaticText* date;
    gui::IGUIStaticText* name;
    gui::IGUIStaticText* score;
    static wchar_t scorestring[5];
    static wchar_t rankstring[5];
    swprintf(scorestring, 5, L"%d", this->scores[i].score);
    swprintf(rankstring, 5, L"#%d", i+1);
    rank = this->app->guienv->addStaticText(rankstring, core::rect<s32>(x, y, x + 50, y + 30), false, true, 0);
    date = this->app->guienv->addStaticText(this->scores[i].date, core::rect<s32>(x + 55, y, x + 205, y + 30), false, true, 0);
    name = this->app->guienv->addStaticText(this->scores[i].name, core::rect<s32>(x + 210, y, x + 510, y + 30), false, true, 0);
    score = this->app->guienv->addStaticText(scorestring, core::rect<s32>(x + 515, y, x + 595, y + 30), false, true, 0);
    rank->setOverrideColor(video::SColor(255, 255, 0, 0));
    date->setOverrideColor(video::SColor(255, 255, 0, 0));
    name->setOverrideColor(video::SColor(255, 255, 0, 0));
    score->setOverrideColor(video::SColor(255, 255, 0, 0));
  }

  // if new score, add an entry box and a confirm button
  if(this->new_score != -1) {
    int x = 100;
    int y = 80 + 35 * this->new_rank;
    this->name_box = this->app->guienv->addEditBox(L"", core::rect<s32>(x + 210, y, x + 510, y + 30));
    gui::IGUIButton* btn = this->app->guienv->addButton(core::rect<s32>(x + 600, y, x + 680, y + 30), 0, 2, L"SAVE");
  }
}

void Scoreboard::update() {

}

void Scoreboard::onevent(const SEvent &event) {
  if(event.EventType == EET_GUI_EVENT) {
    s32 id = event.GUIEvent.Caller->getID();
    if(event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED) {
      switch(id) {
      case 1: // return button
	
	// restore scoreboard
	if(this->new_score != -1) {
	  this->new_score = -1;
	  for(int i = this->new_rank; i < 9; i++)
	    this->scores[i] = this->scores[i + 1];
	  this->scores[9] = this->kicked_out;
	}
	// ----
	this->app->setActivity(TITLE);
	break;
	
      case 2: // confirm new score button

	// write name written by user to scoreboard
	swprintf(this->scores[this->new_rank].name, MAX_NAME_LENGTH, L"%S", this->name_box->getText());
	
	// save scores to file
	FILE* sbfile = fopen("./scores", "wb");
	if(sbfile == NULL) {
	  printf("ERROR: Couldn't save scores to file!\n");
	}
	fwrite((char*) this->scores, 1, sizeof(this->scores), sbfile);
	fclose(sbfile);
	// title
	this->new_score = -1;
	this->app->setActivity(TITLE);
	break;
      }
    }
  }
}

void Scoreboard::drop() {
  
} 
