#pragma once
#include "ofMain.h"
#include "SpriteSystem.h"
#include "BaseObject.h"
/* Project 1
  General purpose Emitter class for emitting sprites
By Timothy Nguyen  CS 134 Spring 2020*/

//  This works similar to a Particle emitter

class Emitter : public BaseObject {
public:
	Emitter(SpriteSystem *);
	void draw();
	void stop();
	void constant();
	void setLifespan(float);
	void setVelocity(ofVec3f);
	void setChildImage(ofImage);
	void setInvaderImage(ofImage);
	void setImage(ofImage);
	void setRate(float);
	void update();
	void setEnemy();
	void setFriendly();
	SpriteSystem *sys;
	float rate;
	ofVec3f velocity;
	ofVec3f position;
	float lifespan;
	float lastSpawned;
	ofImage childImage;
	ofImage image;
	bool drawable;
	bool haveChildImage;
	bool haveImage;
	bool constantFire;
	bool enemy;
	float width, height;
	ofSoundPlayer laserSound; //Sound effect of shooting missiles

};