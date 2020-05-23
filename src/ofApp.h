#pragma once
/* Project 1

By Timothy Nguyen  CS 134 Spring 2020*/
#include "ofMain.h"
#include "ofxGui.h"
#include "SpriteSystem.h"
#include "Sprite.h"
#include "Emitter.h"
#include "Particle.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"

typedef enum { MoveStop, MoveLeft, MoveRight, MoveUp, MoveDown } MoveDir;

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void checkCollision();
	void checkPlayerCollision();
	void fastMovement();


	SpriteSystem *copyOfTurret; //Copies of SpriteSystems to check for Collisions
	SpriteSystem *enemyOne;
	SpriteSystem *enemyTwo;
	SpriteSystem *enemyThree;

	vector<Emitter *> emitters;       //Arrays of Emitters
	vector<Emitter *> invaderList;    //Array of Invaders
	Emitter *fastInvader;

	ParticleEmitter explosionOne;

	ImpulseRadialForce *explosionForce;

	ParticleSystem *sys;
	Particle *fastInvadePosition;
	ParticleEmitter *fastEmitter;

	ofPoint myMouse;
	ofImage defaultImage;
	ofVec3f mouse_last;

	bool imageLoaded;
	int score;
	int health;
	float playerContactDistance;
	int difficultyModifier;
	int explodeHereX, explodeHereY;
	float targetX, targetY;
	int timeOfHit;
	int lastHit;
	float deathOrbHeight;

	ofxFloatSlider direction;
	bool bHide;
	bool gameStarted;
	bool activateFastInvader;


	ofxFloatSlider rate;     //Editing a lot of different enemies/emitters
	ofxFloatSlider shootRate;
	ofxFloatSlider enemyOneRate;
	ofxFloatSlider enemyTwoRate;
	ofxFloatSlider enemyThreeRate;
	ofxFloatSlider enemyDirectionOne;
	ofxFloatSlider enemyDirectionTwo;
	ofxFloatSlider enemyDirectionThree;

	ofxLabel screenSize;
	ofxPanel gui;
	ofTrueTypeFont font, font2, font3;
	ofSoundPlayer explosion;
	ofSoundPlayer bgMusic;
};
