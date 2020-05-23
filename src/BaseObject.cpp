#pragma once
#include "BaseObject.h"
/* Project 1

By Timothy Nguyen  CS 134 Spring 2020*/

//Base Object using setPosition
BaseObject::BaseObject() {
	trans = ofVec3f(0, 0, 0);
	scale = ofVec3f(1, 1, 1);
	rot = 0;
}

//Used to allow me to move my emitter either with the mouse or WASD keys
void BaseObject::setPosition(ofVec3f pos) {
	trans = pos;
}