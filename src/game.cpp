Game::Game(Application* app) : Activity(app) {
  // init map
  this->map = new GameMap(this->app->smgr, this->app->driver);
  
  // init player
  scene::IMesh* player_mesh = this->app->smgr->getMesh("./res/model/barba/barba.obj");
  if(!player_mesh) {
    printf("Error! Could not load barba.obj!");
    this->successfully_initialized = false;
    return;
  }
  video::ITexture* player_texture = this->app->driver->getTexture("./res/model/barba/barba_texture.png");
  this->player = new Player(this->app->smgr, player_mesh, player_texture, this->map);

  // init ball
  scene::IMesh* ball_mesh = this->app->smgr->getMesh("./res/model/ball/ball.obj");
  if(!player_mesh) {
    printf("Error! Could not load barba.obj!");
    this->successfully_initialized = false;
    return;
  }
  video::ITexture* ball_texture = this->app->driver->getTexture("./res/model/ball/texture.png");
  this->ball = new Ball(this->app->smgr, ball_mesh, ball_texture, this->map);

  // init store guy
  scene::IAnimatedMesh* guy_mesh = this->app->smgr->getMesh("./res/model/storeguy/ninja.b3d");
  video::ITexture* guy_texture = this->app->driver->getTexture("./res/model/storeguy/nskinrd.jpg");
  this->guy = new StoreGuy(this->app->smgr, guy_mesh, guy_texture, this->map, this->player);
  
  // load gui images
  this->ball_held = this->app->driver->getTexture("./res/img/ball_held.png");
  this->ball_thrown = this->app->driver->getTexture("./res/img/ball_thrown.png");
}

void Game::setup() {
  this->app->smgr->setAmbientLight(video::SColorf(.6f,.6f,.6f,1.0f));
  
  // setup game objects
  this->player->setup();
  this->map->setup();
  this->ball->setup();
  this->guy->setup();
  
  this->camera = this->app->smgr->addCameraSceneNode();
  this->updateCamera();

  scene::ILightSceneNode * light = this->app->smgr->addLightSceneNode(0, core::vector3df(-60,100,400),
							   video::SColorf(1.0f,1.0f,1.0f,1.0f), 300.0f);

  // GUI ELEMENTS
  
  this->score_widget =
    this->app->guienv->addStaticText(L"Score: 0", core::rect<s32>(10, 10, 150, 40), false, false, 0);
  this->score_widget->setOverrideColor(video::SColor(255, 255, 255, 0));

  this->ball_state = this->app->guienv->addImage(this->ball_held, core::position2d<s32>(750, 10));

  // stats

  this->score = 0;
  this->gameOver = false;
  this->timeAfterDeath = 0;
  this->ninjaStood = false;
  this->lightChanged = false;
  this->gamePaused = false;
}

void Game::update() {
  
  // keep track of time
  int newT = this->app->device->getTimer()->getTime();
  double dt = this->lastT == 0 ? .001 : ((double)(newT - this->lastT) * .001);
  this->lastT = newT;

  
  if(this->gameOver) {
    this->timeAfterDeath += dt;
    // breathing
    if(this->timeAfterDeath > .5 && !this->ninjaStood) {
      this->guy->node->setFrameLoop(183, 204);
      this->ninjaStood = true;
    }
    if(this->timeAfterDeath > .4 && !this->lightChanged) {
      this->app->smgr->setAmbientLight(video::SColorf(.6f,.3f,.3f,1.0f));
      this->lightChanged = true;
    }
    return;
  }

  if(this->gamePaused) return;
  
  // pause
  if(this->app->keydown[27]) {
    this->pause_widget =
      this->app->guienv->addButton(core::rect<s32>(320, 300, 480, 330), 0, GAME_RESUME_BUTTON_ID, L"Continue");
    this->gamePaused = true;
  }

  // precompute
  double player_rotation_sin = sin(this->player->rotation);
  double player_rotation_cos = cos(this->player->rotation);
  
  // player movement
  if(this->app->keydown[38] or this->app->keydown[87]) { // forward
    this->player->velocityX += this->player->acceleration * dt * player_rotation_sin;
    this->player->velocityZ += this->player->acceleration * dt * player_rotation_cos;
    double total_velocity = squareNum(this->player->velocityX) + squareNum(this->player->velocityZ);
    if(total_velocity > squareNum(this->player->maxspeed)) {
      this->player->velocityX = this->player->maxspeed * player_rotation_sin;
      this->player->velocityZ = this->player->maxspeed * player_rotation_cos;
    }
  }
  if(this->app->keydown[40] or this->app->keydown[83]) { // backward
    this->player->velocityX -= this->player->acceleration * dt * player_rotation_sin * .5;
    this->player->velocityZ -= this->player->acceleration * dt * player_rotation_cos * .5;
    double total_velocity = squareNum(this->player->velocityX) + squareNum(this->player->velocityZ);
    if(total_velocity > squareNum(this->player->maxspeed)) {
      this->player->velocityX = -this->player->maxspeed * player_rotation_sin;
      this->player->velocityZ = -this->player->maxspeed * player_rotation_cos;
    }
  }
  if(this->app->keydown[37] or this->app->keydown[65]) { // left
    this->player->rotate(-this->player->rspeed * dt);
  }
  if(this->app->keydown[39] or this->app->keydown[68]) { // right
    this->player->rotate(this->player->rspeed * dt);
  }
  // throw ball
  if(this->app->keydown[32] && !this->ball->active) {
    this->ball->active = true;
    this->ball->position = this->player->position;
    this->ball->velocityZ = this->player->velocityZ + player_rotation_cos * 20;
    this->ball->velocityX = this->player->velocityX + player_rotation_sin * 20;
    this->ball->velocityY = 20;
    this->ball_state->setImage(this->ball_thrown);
  }

  double ball_player_x_dist = this->player->position.X - this->ball->position.X;
  double ball_player_z_dist = this->player->position.Z - this->ball->position.Z;
  if(ball_player_x_dist * ball_player_x_dist + ball_player_z_dist * ball_player_z_dist < 4 &&
     this->ball->position.Y < 2 && this->ball->velocityY < 15) {
    this->ball->pickup();
    this->ball_state->setImage(this->ball_held);
  }

  // pick up drops
  int player_row = floor((this->player->position.X + 80) * .2);
  int player_col = floor((this->player->position.Z + 80) * .2);
  if(this->map->drop_nodes[player_col][player_row] != 0) {
    this->score += 1;
    swprintf(this->score_string, 50, L"Score: %d", this->score);
    this->score_widget->setText(this->score_string);
    this->map->drop_nodes[player_col][player_row]->remove();
    this->map->drop_nodes[player_col][player_row] = 0;
  }
  
  if(this->ball->active) this->ball->update(dt);
  this->player->update(dt);
  this->guy->update(dt);
  this->updateCamera();

  // check if player lost
  double x_dist = this->player->position.X - this->guy->position.X;
  double z_dist = this->player->position.Z - this->guy->position.Z;
  if(x_dist * x_dist + z_dist * z_dist <= 7) {
    this->gameOver = true;
    // set proper rotation (0° = Z axis)
    if(z_dist == 0) {
      if(x_dist > 0) {
	this->guy->rotation = core::PI * .5;
      } else {
	this->guy->rotation = core::PI * 1.5;
      }
    } else {
      this->guy->rotation = atan(x_dist / z_dist);
      if(z_dist < 0) this->guy->rotation += core::PI;
    }
    this->guy->rotation -= core::PI * .25;
    this->guy->node->setRotation(core::vector3df(0, 180/core::PI*this->guy->rotation, 0));
    // jump and stab
    this->guy->node->setFrameLoop(112, 125);
    // gui
    this->app->guienv->addButton(core::rect<s32>(320, 300, 480, 330), 0, GAME_QUIT_BUTTON_ID, L"Title screen");
    // check if score beats someone on scoreboard
    if(this->score >= this->scoreboard->scores[9].score) {
      this->app->guienv->addButton(core::rect<s32>(320, 340, 480, 370), 0, GAME_SCORE_BUTTON_ID, L"Scoreboard!");
    }
  }
}

void Game::updateCamera() {
  core::vector3df camera_pos = core::vector3df(this->player->position.X - 10 * sin(this->player->camera_rotation),
					       this->player->position.Y + 10,
					       this->player->position.Z - 10 * cos(this->player->camera_rotation));
  
  core::vector3df camera_targ = core::vector3df(camera_pos.X + 12 * sin(this->player->camera_rotation),
						this->player->position.Y,
						camera_pos.Z + 12 * cos(this->player->camera_rotation));
    
  this->camera->setPosition(camera_pos);
  this->camera->setTarget(camera_targ);
}

void Game::onevent(const SEvent &event) {
  if(event.EventType == EET_GUI_EVENT) {
    s32 id = event.GUIEvent.Caller->getID();
    if(event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED) {
      switch(id) {
      case GAME_QUIT_BUTTON_ID:
	this->app->setActivity(TITLE);
	break;
      case GAME_RESUME_BUTTON_ID:
	this->gamePaused = false;
	this->pause_widget->remove();
	break;
      case GAME_SCORE_BUTTON_ID:
	this->scoreboard->new_score = this->score;
	this->app->setActivity(SCOREBOARD);
	break;
      }
    }
  }
}

void Game::drop() {
  delete this->guy;
  delete this->player;
  delete this->ball;
  delete this->map;
}
