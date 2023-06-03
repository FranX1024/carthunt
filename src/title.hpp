class TitleActivity : public Activity {
public:
  TitleActivity(Application* app);
  
  virtual void setup();
  virtual void update();
  virtual void onevent(const SEvent &event);
  virtual void drop();

  video::ITexture* background;
};
