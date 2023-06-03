class Ball {
public:

  // permanent
  scene::IMesh* mesh;
  video::ITexture* texture;
  scene::ISceneManager* smgr;
  GameMap *map;

  double radius = .3;
  double friction = .8;
  double bounce_factor = .8;
  double gravity = 40;
  
  // change on setup
  scene::ISceneNode* node;
  core::vector3df position;
  double velocityX;
  double velocityZ;
  double velocityY;

  bool active;
  
  Ball(scene::ISceneManager* smgr, scene::IMesh* mesh, video::ITexture* texture, GameMap* map);
  void setup();
  void pickup();
  void update(double dt);
  std::vector<Point2d> getPolygon();
};
