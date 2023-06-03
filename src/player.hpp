class Player {
public:

  // permanent
  scene::IMesh* mesh;
  video::ITexture* texture;
  scene::ISceneManager* smgr;
  GameMap *map;

  double maxspeed = 40;
  double acceleration = 60;
  double vertical_friction = .05;
  double parallel_friction = .5;
  double bounce_factor = .4;
  double rspeed = core::PI;

  // change on setup
  scene::ISceneNode* node;
  core::vector3df position;
  double rotation;
  double camera_rotation;
  double velocityX;
  double velocityZ;
  
  Player(scene::ISceneManager* smgr, scene::IMesh* mesh, video::ITexture* texture, GameMap* map);
  void setup();
  void update(double dt);
  void rotate(double phi);
  std::vector<Point2d> getPolygon();
};
