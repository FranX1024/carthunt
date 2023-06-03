class StoreGuy {
public:

  // permanent
  scene::IAnimatedMesh* mesh;
  video::ITexture* texture;
  scene::ISceneManager* smgr;
  GameMap* map;
  Player* player;

   // (5 / speed), 5 is width/height of single cell
  double step_time = .2;
  double inverse_step_time;

  // change on setup
  scene::IAnimatedMeshSceneNode* node;
  core::vector3df position;
  double rotation;

  int target_row;
  int target_col;
  double target_rotation;

  double velocityX;
  double velocityZ;
  double velocityR;
  double movement_duration;
  
  StoreGuy(scene::ISceneManager* smgr, scene::IAnimatedMesh* mesh,
	   video::ITexture* texture, GameMap* map, Player* player);
  void setup();
  void update(double dt);
  void setToTarget();
  std::pair<int,int> pickWhereNext();
};
