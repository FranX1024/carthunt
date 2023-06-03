GameMap::GameMap(scene::ISceneManager* smgr, video::IVideoDriver* driver) {
  this->mesh = smgr->getMesh("./res/model/map/map.obj");
  this->texture = driver->getTexture("./res/model/map/texture.png");
  this->shelf_mesh = smgr->getMesh("./res/model/shelf/shelf.obj");
  this->shelf_texture = driver->getTexture("./res/model/shelf/texture.png");
  this->empty_texture = driver->getTexture("./res/model/shelf/empty_texture.png");
  this->drop_mesh = smgr->getMesh("./res/model/drop/drop.obj");
  this->drop_texture = driver->getTexture("./res/model/drop/texture.png");
  this->smgr = smgr;
}

void GameMap::setup() {
  this->node = this->smgr->addMeshSceneNode(this->mesh);
  this->node->setPosition(core::vector3df(0, 0, 0));
  this->node->setMaterialTexture(0, this->texture);

  // reset tilemap
  for(int i = 0; i < this->rows; i++) {
    for(int j = 0; j < this->cols; j++) {
      this->tiles[i][j] = TILEMAP[i][j];
    }
  }
  
  // setup tilemap
  for(int i = 0; i < this->cols; i++) {
    for(int j = 0; j < this->rows; j++) {
      if(!this->tiles[i][j]) continue;
      scene::ISceneNode* node = this->smgr->addMeshSceneNode(this->shelf_mesh);
      node->setMaterialTexture(0, this->shelf_texture);
      node->setPosition(core::vector3df(2.4 + 5 * j - 80, 0, 2.45 + 5 * i - 80));
      this->tile_nodes[i][j] = node;
    }
  }

  // reset drops
  for(int i = 0; i < this->cols; i++) {
    for(int j = 0; j < this->rows; j++) {
      this->drop_nodes[i][j] = 0;
    }
  }
}

bool GameMap::collides(std::vector<Point2d> polygon) {
      
  // find top and bottom row polygon is contained in
  double miny = polygon[0].y;
  double maxy = polygon[0].y;
  for(int i = 0; i < polygon.size(); i++) {
    miny = std::min(miny, polygon[i].y);
    maxy = std::max(maxy, polygon[i].y);
  }
  int bottom_row = floor(maxy);
  int top_row = floor(miny);
  int rows = bottom_row - top_row + 1;
    
  // arrays to store leftmost and rightmost column for each row
  int* left_border = new int[rows];
  int* right_border = new int[rows];
    
  for(int i = 0; i < rows; i++) {
    left_border[i] = 1e9;
    right_border[i] = -1e9;
  }
    
  for(int i = 0; i < polygon.size(); i++) {
        
    // get the next point to form a line
    int j = i + 1;
    if(j == polygon.size()) j = 0;
        
    // rows through which the line spans
    double top_y = std::min(polygon[i].y, polygon[j].y);
    double bottom_y = std::max(polygon[i].y, polygon[j].y);
    int line_top = floor(top_y);
    int line_bottom = floor(bottom_y);
        
    // special case if the line is contained within a single row
    if(line_top == line_bottom) {
      left_border[line_top - top_row] = std::min(left_border[line_top - top_row], (int)floor(std::min(polygon[i].x, polygon[j].x)));
      right_border[line_top - top_row] = std::max(right_border[line_top - top_row], (int)floor(std::max(polygon[i].x, polygon[j].x)));
      continue;
    }
        
    double y2x = (polygon[i].x - polygon[j].x) / (polygon[i].y - polygon[j].y);
        
    for(int row = line_top; row <= line_bottom; row++) {
      double minx, maxx;
      minx = polygon[i].x + y2x * (std::max((double) row, top_y) - polygon[i].y);
      maxx = polygon[i].x + y2x * (std::min((double) row + 1, bottom_y) - polygon[i].y);
      if(minx > maxx) std::swap(minx, maxx);
            
      left_border[row - top_row] = std::min(left_border[row - top_row], (int)floor(minx));
      right_border[row - top_row] = std::max(right_border[row - top_row], (int)floor(maxx));
    }
  }
    
  // check all covered cells for collison
  bool collides = false;
  for(int row = top_row; row <= bottom_row; row++) {
    for(int col = left_border[row - top_row]; col <= right_border[row - top_row]; col++) {
      if(row < 0 or row >= this->rows or col < 0 or col >= this->cols or this->tiles[col][row] != 0) {
	collides = true;
	break;
      }
    }
  }
  
  delete left_border;
  delete right_border;

  return collides;
}

void GameMap::ballHit(int row, int col) {
  if(row >= 0 && col >= 0 && row < this->rows && col < this->cols && this->tiles[col][row] == 1) {
    this->tile_nodes[col][row]->setMaterialTexture(0, this->empty_texture);
    this->tiles[col][row] = 2; // 1 = full, 2 = empty

    // create drops
    std::pair<int,int> sides[4] = {
      {col + 1, row}, {col - 1, row},
      {col, row + 1}, {col, row - 1}
    };
    for(int i = 0; i < 4; i++) {
      int col = sides[i].first, row = sides[i].second;
      if(
	 col < 0 or
	 row < 0 or
	 col >= this->cols or
	 row >= this->rows or
	 drop_nodes[col][row] != 0 or
	 tiles[col][row] != 0)
	continue;
      scene::ISceneNode* node = this->smgr->addMeshSceneNode(this->drop_mesh);
      node->setMaterialTexture(0, this->drop_texture);
      node->setPosition(core::vector3df(2.4 + 5 * row - 80, 0, 2.45 + 5 * col - 80));
      this->drop_nodes[col][row] = node;
    }
  }
}
