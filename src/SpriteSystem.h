/* Project 1

By Timothy Nguyen  CS 134 Spring 2020*/
#pragma once

#include "ofMain.h"
#include "Sprite.h"
//  Manages all Sprites in a system.  You can create multiple systems
//
class SpriteSystem {
public:
	void add(Sprite);
	void remove(int);
	void update();
	void draw();
	vector<Sprite> sprites;

};
