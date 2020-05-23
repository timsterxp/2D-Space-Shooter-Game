#include "ofMain.h"
#include "ofApp.h"
/* Project 1

By Timothy Nguyen  CS 134 Spring 2020*/
//========================================================================
int main() {
	ofSetupOpenGL(640, 1136, OF_WINDOW);			// <-------- setup the GL context
	// This resolution is of an iPhone SE, (Aka my phone).

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
}
