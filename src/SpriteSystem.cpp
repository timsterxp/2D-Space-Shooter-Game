#include "SpriteSystem.h"
/* Project 1

By Timothy Nguyen  CS 134 Spring 2020*/

//  Add a Sprite to the Sprite System
//
void SpriteSystem::add(Sprite s) {
	sprites.push_back(s);
}

// Remove a sprite from the sprite system. Note that this function is not currently
// used. The typical case is that sprites automatically get removed when the reach
// their lifespan.
//
void SpriteSystem::remove(int i) {
	sprites.erase(sprites.begin() + i);
}


//  Update the SpriteSystem by checking which sprites have exceeded their
//  lifespan (and deleting).  Also the sprite is moved to it's next
//  location based on velocity and direction.
//
void SpriteSystem::update() {

	if (sprites.size() == 0) return;
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;

	// check which sprites have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, use an iterator.
	//
	while (s != sprites.end()) {
		if (s->lifespan != -1 && s->age() > s->lifespan) {
			//			cout << "deleting sprite: " << s->name << endl;
			tmp = sprites.erase(s);
			s = tmp;
		}
		//Remove the sprites if they leave the window area(I gave some leeway because pixel images have transparent areas that are bigger than their model
		else if (s->position.x > ofGetWindowWidth() + 50 || s->position.x<-50 || s->position.y>ofGetWindowHeight() + 150 || s->position.y < -50) {
			tmp = sprites.erase(s);
			s = tmp;
		}
		else s++;
	}

	//  Move sprite
	//
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].trans += sprites[i].velocity / ofGetFrameRate();
		sprites[i].position = sprites[i].trans;
	}
}

//  Render all the sprites
//
void SpriteSystem::draw() {
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].draw();
	}
}