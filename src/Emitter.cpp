
#include "Emitter.h"
/* Project 1
Emitter system from Labs, necessary as I used turret as a plain-emitter
By Timothy Nguyen  CS 134 Spring 2020*/
//  Create a new Emitter - needs a SpriteSystem
//
Emitter::Emitter(SpriteSystem *spriteSys) {
	sys = spriteSys;
	lifespan = 3000;    // milliseconds

	lastSpawned = 0;
	rate = 1;    // sprites/sec
	haveChildImage = false;
	haveImage = true;
	velocity = ofVec3f(100, 100, 0);
	drawable = true;
	width = 50;
	height = 50;
	laserSound.load("sound/laser.wav", true);
}

//  Draws the emitter


void Emitter::draw() {
	if (drawable) {

		if (haveImage) {
			image.draw(-image.getWidth() / 2.0 + trans.x, -image.getHeight() / 2.0 + trans.y);
		}
		else {
			ofSetColor(0, 0, 200);
			ofDrawRectangle(-width / 2 + trans.x, -height / 2 + trans.y, width, height);
		}
	}

	// draw sprite system
	//
	sys->draw();
}

//  Update the Emitter. This only runs if the space bar was either pressed or is currently pressed.
//  also in charge of playing the sound file
//
void Emitter::update() {



	float time = ofGetElapsedTimeMillis();

	if (((time - lastSpawned) > (1000.0 / rate)) && (constantFire == true)) {
		//	 spawn a new sprite
		Sprite sprite;
		if (haveChildImage) sprite.setImage(childImage);
		//If it is our turret, play a sound
		if (!enemy)laserSound.play();

		sprite.velocity = velocity;
		sprite.lifespan = lifespan;
		sprite.setPosition(trans);
		sprite.birthtime = time;
		sys->add(sprite);
		lastSpawned = time;

	}


	sys->update();
}

// Starts the emitter
//

void Emitter::constant() {
	constantFire = true;
}

//Stops the emitter ( if space bar is let go)
void Emitter::stop() {
	constantFire = false;
}
//Invader emitters
void Emitter::setEnemy() {
	enemy = true;
}

//This is the player turret
void Emitter::setFriendly() {
	enemy = false;
}

//Change lifespan
void Emitter::setLifespan(float life) {
	lifespan = life;
}
//Change velocity
void Emitter::setVelocity(ofVec3f v) {
	velocity = v;
}
//Changes what the Emitter "emits"
void Emitter::setChildImage(ofImage img) {
	childImage = img;
	haveChildImage = true;
}
//Added to differentiate Invaders/Friendly
void Emitter::setInvaderImage(ofImage img) {
	childImage = img;
	haveChildImage = true;
}
//Set image for main emitter
void Emitter::setImage(ofImage img) {
	image = img;
}
//Rate at which emitter emits
void Emitter::setRate(float r) {
	rate = r;
}
