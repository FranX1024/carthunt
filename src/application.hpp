/* Activity refers to a subprogram which takes over
 * the entire game screen for a period of time. It has
 * a setup method, update method and an event listener method
 */

enum ActivityID {
  TITLE,
  GAME,
  SCOREBOARD,
  ACTIVITY_COUNT
};

class Application : public IEventReceiver {
public:
  int status;
  
  Activity* activity[ACTIVITY_COUNT];
  ActivityID current_activity;

  bool keydown[KEY_KEY_CODES_COUNT];
  
  IrrlichtDevice* device;
  video::IVideoDriver* driver;
  scene::ISceneManager* smgr;
  gui::IGUIEnvironment* guienv;

  Application();
  void setActivity(ActivityID new_activity);
  void mainloop();
  void drop();

  virtual bool OnEvent(const SEvent& event);
};
