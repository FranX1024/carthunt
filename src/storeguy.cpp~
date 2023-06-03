/*
  row = x axis
  column = z axis
*/

StoreGuy::StoreGuy(scene::ISceneManager* smgr, scene::IAnimatedMesh* mesh,
		   video::ITexture* texture, GameMap* map, Player* player) {
  this->smgr = smgr;
  this->mesh = mesh;
  this->map = map;
  this->player = player;
  this->texture = texture;

  this->inverse_step_time = 1 / this->step_time;
}

void StoreGuy::setup() {
  this->target_row = 16;
  this->target_col = 16;
  this->target_rotation = 0;
  this->movement_duration = 0;
  
  this->node = this->smgr->addAnimatedMeshSceneNode(this->mesh);
  this->node->setScale(core::vector3df(.9, .9, .9));
  this->node->setMaterialTexture(0, this->texture);
  this->node->setFrameLoop(0, 13);
  this->setToTarget();
}

void StoreGuy::update(double dt) {
  this->movement_duration += dt;
  this->position.X += dt * this->velocityX;
  this->position.Z += dt * this->velocityZ;
  this->rotation += dt * this->velocityR;

  if(this->movement_duration >= this->step_time) {
    this->movement_duration = 0;
    this->setToTarget();
    std::pair<int,int> new_target = this->pickWhereNext();
    this->velocityX = (double) (new_target.first - this->target_row) * 5 * this->inverse_step_time;
    this->velocityZ = (double) (new_target.second - this->target_col) * 5 * this->inverse_step_time;
    // proper orientation
    if(this->velocityZ == 0) {
      if(this->velocityX > 0) {
	this->target_rotation = core::PI * .5;
      } else {
	this->target_rotation = core::PI * 1.5;
      }
    } else {
      if(this->velocityZ > 0) {
	this->target_rotation = 0;
      } else {
	this->target_rotation = core::PI;
      }
    }
    // choose whether to rotate clockwise or counterclockwise
    if(abs(this->target_rotation - this->rotation) > core::PI) {
      if(this->target_rotation > this->rotation)
	this->velocityR = (this->target_rotation - this->rotation - 2 * core::PI) * this->inverse_step_time;
      else
	this->velocityR = (this->target_rotation - this->rotation + 2 * core::PI) * this->inverse_step_time;
    } else
      this->velocityR = (this->target_rotation - this->rotation) * this->inverse_step_time;
    // ...
    this->target_row = new_target.first;
    this->target_col = new_target.second;
  } else {
    this->node->setPosition(this->position);
    this->node->setRotation(core::vector3df(0, 180/core::PI*this->rotation, 0));
  }
}

void StoreGuy::setToTarget() {
  this->position = core::vector3df((double)(this->target_row - 16) * 5 + 2.5, 0,
				   (double)(this->target_col - 16) * 5 + 2.5);
  this->rotation = this->target_rotation;
  this->node->setPosition(this->position);
  this->node->setRotation(core::vector3df(0, 180/core::PI*this->rotation, 0));
}

// implementation of A* algorithm (BFS with distance heuristic)

std::pair<int,int> StoreGuy::pickWhereNext() {
  
  static std::pair<int,int> facings[4] = {
    {0,1},
    {1,0},
    {0,-1},
    {-1,0}
  };

  static int player_row, player_col, map_rows, map_cols;
  
  player_col = (this->player->position.Z + 80) * .2;
  player_row = (this->player->position.X + 80) * .2;
  map_rows = this->map->rows;
  map_cols = this->map->cols;

  struct map_cell {
    int square_distance;
    int row;
    int col;
    int marker;
    map_cell(int rrow, int ccol, int mm) {
      marker = mm;
      row = rrow;
      col = ccol;
      int drow = (player_row - row);
      int dcol = (player_col - col);
      square_distance = drow * drow + dcol * dcol;
    }
  };

  struct map_cell_cmp {
    bool operator () (map_cell a, map_cell b) const {
      return a.square_distance < b.square_distance or a.square_distance == b.square_distance and
	(a.row < b.row or a.row == b.row and a.col < b.col);
    }
  };

  std::set <map_cell, map_cell_cmp> que;
  std::set <map_cell, map_cell_cmp> passed;
  
  for(int i = 0; i < 4; i++) {
    int nrow = this->target_row + facings[i].first;
    int ncol = this->target_col + facings[i].second;
    if(nrow < 0 or ncol < 0 or nrow >= map_rows or ncol >= map_cols or
       this->map->tiles[ncol][nrow] != 0)
      continue;
    que.insert(map_cell(nrow, ncol, i));
  }
  int winning_marker = 0;
  while(!que.empty()) {
    map_cell cell = *que.begin();
    que.erase(cell);
    if(cell.row == player_row and cell.col == player_col) {
      winning_marker = cell.marker;
      break;
    }
    /* marker is completely ignored by the comparator so
       cells can be stored as are regardless of their marker
     */
    for(int i = 0; i < 4; i++) {
      if(cell.row < 0 or cell.col < 0 or cell.row >= map_rows or cell.col >= map_cols or
	 this->map->tiles[cell.col][cell.row] != 0)
	continue;
      map_cell newcell = map_cell(cell.row + facings[i].first, cell.col + facings[i].second, cell.marker);
      if(passed.find(newcell) != passed.end())
	continue;
      passed.insert(newcell);
      que.insert(newcell);
    }
  }
  
  return {this->target_row + facings[winning_marker].first, this->target_col + facings[winning_marker].second};
}
