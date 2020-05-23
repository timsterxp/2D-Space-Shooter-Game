/* Project 1

By Timothy Nguyen  CS 134 Spring 2020*/
#pragma once
#include "BaseObject.h"

//  General Sprite class  (similar to a Particle)
//
class Sprite : public BaseObject {
public:
	Sprite();
	void draw();
	float age();
	void setImage(ofImage);
	float speed;    //   in pixels/sec
	ofVec3f velocity; // in pixels/sec
	ofImage image;
	float birthtime; // elapsed time in ms
	float lifespan;  //  time in ms
	string name;
	bool haveImage;
	float width, height;
	ofVec3f position;
	float spriteContactDistance; //Necessary for collisions
};