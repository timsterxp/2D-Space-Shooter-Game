#pragma once
#include "ofMain.h"
/* Project 1

By Timothy Nguyen  CS 134 Spring 2020*/

// This is a base object that all drawable object inherit from
// It is possible this will be replaced by ofNode when we move to 3D

class BaseObject {
public:
	BaseObject();
	ofVec2f trans, scale;
	float	rot;
	void setPosition(ofVec3f);
};