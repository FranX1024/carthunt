#define MAX_NAME_LENGTH 25

// Scoreboard tracks top 10

struct Scoreboard_row {
  wchar_t date[11];
  wchar_t name[MAX_NAME_LENGTH];
  int score;
};

class Scoreboard : public Activity {
public:
  Scoreboard(Application* app);
  
  virtual void setup();
  virtual void update();
  virtual void onevent(const SEvent &event);
  virtual void drop();

  video::ITexture* background;
  
  int new_score; // -1 if no new score, otherwise equal to the new score
  Scoreboard_row scores[10];

  // preserve old scoreboard
  Scoreboard_row kicked_out;
  int new_rank;

  // new score widgets
  gui::IGUIEditBox* name_box;
};
