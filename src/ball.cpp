Ball::Ball(scene::ISceneManager* smgr, scene::IMesh* mesh, video::ITexture* texture, GameMap* map) {
  this->mesh = mesh;
  this->texture = texture;
  this->smgr = smgr;
  this->map = map;
}

void Ball::setup() {
  this->position = core::vector3df(0, 200, 200);
  
  this->node = this->smgr->addMeshSceneNode(this->mesh);
  this->node->setMaterialTexture(0, this->texture);
  this->node->setPosition(this->position);
  
  this->velocityX = 0;
  this->velocityZ = 0;
  this->velocityY = 0;

  this->active = false;
}

std::vector<Point2d> Ball::getPolygon() {
  std::vector<Point2d> pts;

  double step_angle = core::PI * .25;
  for(int i = 0; i < 8; i++) {
    pts.push_back(Point2d((this->radius * cos(step_angle * i) + this->position.Z + 80) * .2,
			  (this->radius * sin(step_angle * i) + this->position.X + 80) * .2));
  }
  
  return pts;
}

void Ball::pickup() {
  this->position = core::vector3df(0, 200, 200);
  this->active = false;
  this->node->setPosition(this->position);
}

void Ball::update(double dt) {

  bool hit_wall = false;
  std::pair<int, int> hit_position;
  
  this->position.X += this->velocityX * dt;

  // handle collisin (X axis)
  if(this->map->collides(this->getPolygon()) || this->position.Z + 80 < this->radius || this->position.X + 80 < this->radius) {
    
    this->position.X -= this->velocityX * dt;

    hit_wall = true;
    hit_position = this->velocityX > 0 ?
      std::pair<int,int>( (int)floor((this->position.X+80)*.2)+1, (int)floor((this->position.Z+80)*.2) ):
      std::pair<int,int>( (int)floor((this->position.X+80)*.2)-1, (int)floor((this->position.Z+80)*.2) );
    
    double hi = this->velocityX * dt;
    double lo = -hi / 1023;

    for(int it = 0; it < 10; it++) {
      double mid = (lo + hi) * .5;
      this->position.X += mid;

      if(this->map->collides(this->getPolygon()) || this->position.Z + 80 < this->radius || this->position.X + 80 < this->radius) {
	hi = mid;
      } else {
	lo = mid;
      }

      this->position.X -= mid;
    }

    this->position.X += lo;
    this->velocityX *= -this->bounce_factor;
  }
  
  this->position.Z += this->velocityZ * dt;

  // handle collisin (Z axis)
  if(this->map->collides(this->getPolygon()) || this->position.Z + 80 < this->radius || this->position.X + 80 < this->radius) {
    
    this->position.Z -= this->velocityZ * dt;

    hit_wall = true;
    hit_position = this->velocityZ > 0 ?
      std::pair<int,int>( (int)floor((this->position.X+80)*.2), (int)floor((this->position.Z+80)*.2)+1 ):
      std::pair<int,int>( (int)floor((this->position.X+80)*.2), (int)floor((this->position.Z+80)*.2)-1 );
    
    double hi = this->velocityZ * dt;
    double lo = -hi / 1023;

    for(int it = 0; it < 10; it++) {
      double mid = (lo + hi) * .5;
      this->position.Z += mid;

      if(this->map->collides(this->getPolygon()) || this->position.Z + 80 < this->radius || this->position.X + 80 < this->radius) {
	hi = mid;
      } else {
	lo = mid;
      }

      this->position.Z -= mid;
    }
    
    this->position.Z += lo;
    this->velocityZ *= -this->bounce_factor;
  }

  if(hit_wall) {
    this->map->ballHit(hit_position.first, hit_position.second);
  }
  
  // y (up-down)
  this->position.Y += this->velocityY * dt;
  if(this->position.Y < this->radius) {
    this->velocityY *= -this->bounce_factor;
    this->position.Y = this->radius;
    this->velocityX *= this->friction;
    this->velocityZ *= this->friction;
  }
  this->velocityY -= this->gravity * dt;

  // update position of scene node
  this->node->setPosition(this->position);
}
