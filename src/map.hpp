class Point2d {
public:
  double x, y;
  Point2d(int x, int y) {
    this->x = x;
    this->y = y;
  }
};

class GameMap {
public:

  // permanent
  scene::IMesh* mesh;
  video::ITexture* texture;
  scene::ISceneManager* smgr;

  scene::IMesh* shelf_mesh;
  video::ITexture* shelf_texture;
  video::ITexture* empty_texture;

  scene::IMesh* drop_mesh;
  video::ITexture* drop_texture;
  
  int rows = 32;
  int cols = 32;

  // change on setup
  scene::ISceneNode* node;
  
  int tiles[32][32];
  scene::ISceneNode* tile_nodes[32][32];
  scene::ISceneNode* drop_nodes[32][32];
  
  GameMap(scene::ISceneManager* smgr, video::IVideoDriver* driver);
  void setup();
  void ballHit(int row, int col);
  bool collides(std::vector<Point2d> polygon);
};
