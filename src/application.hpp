enum ActivityID {
  TITLE,
  GAME,
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
