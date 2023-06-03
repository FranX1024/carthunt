Player::Player(scene::ISceneManager* smgr, scene::IMesh* mesh, video::ITexture* texture, GameMap* map) {
  this->mesh = mesh;
  this->texture = texture;
  this->smgr = smgr;
  this->map = map;
}

void Player::setup() {
  this->position = core::vector3df(0, 1.4, -78);
  
  this->node = this->smgr->addMeshSceneNode(this->mesh);
  this->node->setMaterialTexture(0, this->texture);
  this->node->setPosition(this->position);

  this->camera_rotation = this->rotation = 0;
  this->velocityX = 0;
  this->velocityZ = 0;
}

std::vector<Point2d> Player::getPolygon() {
  double rotation_sin = sin(this->rotation);
  double rotation_cos = cos(this->rotation);
  std::vector<Point2d> pts = {
    Point2d(-2, -1.8),
    Point2d(2, -1.8),
    Point2d(2, 1.8),
    Point2d(-2, 1.8)
  };
  
  for(int i = 0; i < 4; i++) {
    double px = pts[i].x;
    double py = pts[i].y;
    pts[i].x = (px * rotation_cos - py * rotation_sin + this->position.Z + 80) * .2;
    pts[i].y = (px * rotation_sin + py * rotation_cos + this->position.X + 80) * .2;
  }
  
  return pts;
}

void Player::rotate(double phi) {
  // collision can occur during rotation
  this->rotation += phi;

  if(this->map->collides(this->getPolygon())) {

    this->rotation -= phi;
    double hi = phi;
    double lo = -hi / 1023;

    for(int it = 0; it < 10; it++) {
      double mid = (lo + hi) * .5;
      this->rotation += mid;

      if(this->map->collides(this->getPolygon())) {
	hi = mid;
      } else {
	lo = mid;
      }

      this->rotation -= mid;
    }

    this->rotation += lo;
  }
  
  this->node->setRotation(core::vector3df(0, 180/core::PI*this->rotation, 0));
}

void Player::update(double dt) {
  double rotation_sin = sin(this->rotation);
  double rotation_cos = cos(this->rotation);
  
  this->position.X += this->velocityX * dt;

  // handle collisin (X axis)
  if(this->map->collides(this->getPolygon())) {

    this->position.X -= this->velocityX * dt;
    double hi = this->velocityX * dt;
    double lo = -hi / 1023;

    for(int it = 0; it < 10; it++) {
      double mid = (lo + hi) * .5;
      this->position.X += mid;

      if(this->map->collides(this->getPolygon())) {
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
  if(this->map->collides(this->getPolygon())) {

    this->position.Z -= this->velocityZ * dt;
    double hi = this->velocityZ * dt;
    double lo = -hi / 1023;

    for(int it = 0; it < 10; it++) {
      double mid = (lo + hi) * .5;
      this->position.Z += mid;

      if(this->map->collides(this->getPolygon())) {
	hi = mid;
      } else {
	lo = mid;
      }

      this->position.Z -= mid;
    }

    this->position.Z += lo;
    this->velocityZ *= -this->bounce_factor;
  }

  // update position of scene node
  this->node->setPosition(this->position);

  // apply friction (with respect to orientation of player)
  double vertical_component =
    this->velocityX * rotation_cos - this->velocityZ * rotation_sin;
  double parallel_component =
    this->velocityX * rotation_sin + this->velocityZ * rotation_cos;
  vertical_component *= pow(this->vertical_friction, dt);
  parallel_component *= pow(this->parallel_friction, dt);
  this->velocityX = vertical_component * rotation_cos + parallel_component * rotation_sin;
  this->velocityZ = parallel_component * rotation_cos - vertical_component * rotation_sin;

  // update camera
  this->camera_rotation += 5 * dt * (this->rotation - this->camera_rotation);
}
