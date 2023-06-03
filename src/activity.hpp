class Application;

class Activity {
public:
  Application* app;

  Activity(Application* parent);
  
  virtual void setup();
  virtual void update();
  virtual void onevent(const SEvent &event);
  virtual void drop();
};
