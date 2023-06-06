#define GAME_QUIT_BUTTON_ID 1
#define GAME_RESUME_BUTTON_ID 2
#define GAME_SCORE_BUTTON_ID 3

class Game : public Activity {
public:
  Game(Application* app);
  
  virtual void setup();
  virtual void update();
  virtual void onevent(const SEvent &event);
  virtual void drop();

  void updateCamera();

  bool successfully_initialized = true;

  scene::ICameraSceneNode* camera;

  Ball* ball;
  Player* player;
  GameMap* map;
  StoreGuy* guy;

  int lastT = 0;
  double timeAfterDeath = 0;

  // GUI objects
  gui::IGUIStaticText* score_widget;
  gui::IGUIButton* pause_widget;
  wchar_t score_string[50];

  gui::IGUIImage* ball_state;
  video::ITexture* ball_held;
  video::ITexture* ball_thrown;
  
  // stats
  int score;
  bool gameOver;
  bool ninjaStood;
  bool lightChanged;
  bool gamePaused;

  Scoreboard* scoreboard;
}; 
