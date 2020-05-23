#pragma once
#include "ofMain.h"
/* Project 1

By Timothy Nguyen  CS 134 Spring 2020*/
//  Base class for any object that needs a transform.
//
class TransformObject {
public:
	TransformObject();
	ofVec3f position, scale;
	float	rotation;
	bool	bSelected;
	void setPosition(const ofVec3f &);
};