void init_title() {
  title_image = irrDriver->getTexture("./res/img/title.png");
}

void update_title() {
  irrDriver->draw2DImage(title_image, core::position2d<s32>(0,0),
			 core::rect<s32>(0,0,800,600), 0,
                video::SColor(255,255,255,255), true);
}
