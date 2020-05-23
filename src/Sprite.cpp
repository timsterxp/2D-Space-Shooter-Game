/* Project 1

By Timothy Nguyen  CS 134 Spring 2020*/
#include "Sprite.h"
//
// Basic Sprite Object
//
Sprite::Sprite() {
	speed = 0;
	velocity = ofVec3f(0, 30, 0);
	lifespan = 5;      // lifespan of -1 => immortal 
	birthtime = 0;
	haveImage = false;
	name = "UnnamedSprite";
	width = 60;
	height = 80;
}

// Return a sprite's age in milliseconds
//
float Sprite::age() {
	return (ofGetElapsedTimeMillis() - birthtime);
}

//  Set an image for the sprite. If you don't set one, a rectangle
//  gets drawn.
//
void Sprite::setImage(ofImage img) {
	image = img;
	haveImage = true;
	width = image.getWidth();
	height = image.getHeight();
	spriteContactDistance = height / 3.0;
}


//  Render the sprite
//
void Sprite::draw() {


	ofSetColor(255, 255, 255, 255);

	// draw image centered and add in translation amount
	//
	if (haveImage) {
		image.draw(-width / 2.0 + trans.x, -height / 2.0 + trans.y);

	}
	else {
		// in case no image is supplied, draw something.
		// 
		ofSetColor(255, 0, 0);
		ofDrawRectangle(-width / 2.0 + trans.x, -height / 2.0 + trans.y, width, height);
	}
}