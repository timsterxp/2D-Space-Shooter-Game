#include "ofApp.h"
/* Project 1
Does the regular setup / draw / update, with added collision methods
By Timothy Nguyen  CS 134 Spring 2020*/
/* Project 1

By Timothy Nguyen  CS 134 Spring 2020*/

//--------------------------------------------------------------
void ofApp::setup() {

	//Loads beginning font
	font.load("seaside.ttf", 20, true, true, true);
	font2.load("seaside.ttf", 30, true, true, true);
	font3.load("seaside.ttf", 15, true, true, true);
	ofSetVerticalSync(true);
	explosion.load("sound/explosion.aiff", true);
	ofSetColor(255, 255, 255);

	//Background music, note that some machines may not be able to play ogg files (works on my PC/Laptop, I believe iOS/Mac may have problems)
	bgMusic.load("sound/epicend.ogg", true);
	bgMusic.setLoop(true);
	bgMusic.play();

	//These are the mouse positions of where I first spawn the emitter
	myMouse.x = ofGetWindowWidth() / 2.0;
	myMouse.y = ofGetWindowHeight() / 2.0;

	// create an image for sprites being spawned by emitter(aka the ammo sprites)
	//
	if (defaultImage.load("images/ammo.png")) {
		imageLoaded = true;
		defaultImage.resize(defaultImage.getWidth()*.7, defaultImage.getHeight()*.7);
	}
	else {
		ofLogFatalError("can't load image: images/ammo.png");
		ofExit();
	}

	//Set up explosion forces
	explosionForce = new ImpulseRadialForce(1000);
	explosionOne.sys->addForce(explosionForce);
	explosionOne.setParticleRadius(2);
	explosionOne.setVelocity(ofVec3f(0, 0, 0));
	explosionOne.setLifespan(2);
	explosionOne.setEmitterType(RadialEmitter);
	explosionOne.setGroupSize(25);
	explosionOne.setOneShot(true);



	//Sets up our special "death orb" invader
	fastInvader = new Emitter(new SpriteSystem());
	ofImage fastInvade;
	deathOrbHeight = fastInvade.getHeight();
	fastInvade.load("images/fastinvader.png");
	fastInvader->setImage(fastInvade);
	fastInvader->drawable = true;
	fastInvader->setPosition(ofVec3f(-300, -300, 0)); // I want to hide this until later stages


	//Sets up the turret emitter with the correct image
	Emitter *emit = new Emitter(new SpriteSystem());
	ofImage turret;
	turret.load("images/turret.png");
	turret.resize(turret.getWidth()*.50, turret.getHeight()*.50);
	playerContactDistance = turret.getHeight() / 3.0;
	emit->setImage(turret);
	emit->setChildImage(defaultImage);
	emit->setPosition(ofVec3f(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0));
	emit->drawable = true;
	emit->setFriendly();
	emitters.push_back(emit);

	//Create 3 enemy emitters
	for (int i = 0; i < 3; i++) {
		Emitter *invaders = new Emitter(new SpriteSystem());
		ofImage invaderImage;
		if (i == 0) { //One of them sends out a different invader ship
			invaderImage.load("images/invaderTwo.png");
		}
		else {
			invaderImage.load("images/invaders.png");
		}
		invaderImage.resize(invaderImage.getWidth()*.50, invaderImage.getHeight()*.50);
		invaders->setInvaderImage(invaderImage);
		//Set positions, Top, left, and right
		if (i == 0)invaders->setPosition(ofVec3f(ofGetWindowWidth() / 2.0, 0, 0));
		if (i == 1)invaders->setPosition(ofVec3f(0, ofGetWindowHeight() / 2, 0));
		if (i == 2) invaders->setPosition(ofVec3f(ofGetWindowWidth(), ofGetWindowHeight() / 2, 0));
		invaders->drawable = false;
		invaders->setEnemy();
		invaderList.push_back(invaders);
	}

	//Sets up the GUI, I have various parameters for testing
	gui.setup();
	gui.add(enemyOneRate.setup("Enemy One Spawn Rate", 0, 0, 10));
	gui.add(enemyTwoRate.setup("Enemy Two Spawn Rate", 1, 0, 10));
	gui.add(enemyThreeRate.setup("Enemy Three Spawn Rate", 1, 0, 10));
	gui.add(shootRate.setup("Shoot Rate", 1, 0, 10));
	gui.add(direction.setup("Turret Direction", 0, -500, 500));  //For the sake of "gaming" logic, I only allowed the direction of fire to be towards the front of the turret.
	gui.add(enemyDirectionOne.setup("Top Enemy", 0, -500, 500));
	gui.add(enemyDirectionTwo.setup("Left Enemy", 640, 300, 800));
	gui.add(enemyDirectionThree.setup("Right Enemy", -640, -900, -300));
	bHide = true;

	//begin health at 100, last hit is 0 as we are just starting
	health = 100;
	lastHit = 0;

	//Use particles for the "death orb" fast invader to allow physics to be added to it
	fastInvadePosition = new Particle();
	fastInvadePosition->lifespan = -1;
	fastInvadePosition->position = (ofVec3f(-200, -200, 0));
	sys = new ParticleSystem();
	sys->add(*fastInvadePosition);
	fastEmitter = new ParticleEmitter(sys);
	fastEmitter->visible = true;

}
//In charge of updating any emitter 
//--------------------------------------------------------------
void ofApp::update() {

	Emitter *emitter = emitters[0];
	emitter->setRate(shootRate);
	emitter->setLifespan(-1);
	emitter->setVelocity(ofVec3f(direction, -200, 0));
	emitter->update();
	copyOfTurret = emitter->sys;
	sys->update();

	//This only happens when you progress through the game over 600+points
	if (activateFastInvader) {
		fastMovement();
		fastInvader->setPosition(sys->particles[0].position);
		fastInvadePosition->position = sys->particles[0].position;
	}
	// Goes through the enemies and sets them to aim for player mouse. Slight deviations with random values
	for (int i = 0; i < invaderList.size(); i++) {
		Emitter *invader = invaderList[i];
		float randValueX = rand() % 100;
		float randValueY = rand() % 80;
		float modifier = rand() % 2;

		if (modifier == 0) { //Randomly chooses the random modifier to be negative
			randValueX *= -1;
			randValueY *= -1;
		}
		targetX = -1 * (invader->trans.x - myMouse.x) + randValueX;
		targetY = -1 * (invader->trans.y - myMouse.y) + randValueY;

		invader->setLifespan(-1);
		if (i == 0) {
			invader->setRate(enemyOneRate);
			invader->setVelocity(ofVec3f(targetX, targetY, 0));
			enemyOne = invader->sys;

		}
		if (i == 1) {
			invader->setRate(enemyTwoRate);
			invader->setVelocity(ofVec3f(targetX, targetY, 0));
			enemyTwo = invader->sys;

		}
		if (i == 2) {
			invader->setRate(enemyThreeRate);

			invader->setVelocity(ofVec3f(targetX, targetY, 0));
			enemyThree = invader->sys;

		}

		invader->update();
	}

	checkPlayerCollision(); //Checks if player collided with enemies
	checkCollision();       //Checks if player missiles hit enemies

	//Modifier to make the game harder as the game goes on
	if (score < 500 && health >= 0) {
		difficultyModifier = (score / 100) + 1;
	}
	//Increase shooting rate every 100 points to keep up with enemies
	shootRate = difficultyModifier;
	if (difficultyModifier > 2) { //Spawns a different invader
		enemyOneRate = difficultyModifier - 2;
		if (difficultyModifier > 4) activateFastInvader = true; //Score needs to be 500 to activate the "death orb" fast invader


		// Randomize where the invaders are coming from after 200 points
		for (int i = 0; i < invaderList.size(); i++) {
			float randX = rand() % ofGetWindowWidth();
			float randY = rand() % ofGetWindowHeight();
			if (i == 0) {
				invaderList[i]->setPosition(ofVec3f(randX, 0, 0));
			}
			else {
				invaderList[i]->setPosition(ofVec3f(0, randY, 0));
			}
		}
	}
	enemyTwoRate = difficultyModifier;
	enemyThreeRate = difficultyModifier;
	explosionOne.update();

}



//Sets up the background and also initial idle state conditions. Also in charge of keeping the GUI drawn/hidden
void ofApp::draw() {

	ofImage background;
	background.load("images/space.png");
	background.draw(0, 0);
	ofSetColor(255, 255, 255);
	if (!gameStarted) {
		string setText = "Please press \nSpacebar to start \nthe game";
		ofPushMatrix();
		ofTranslate(ofGetWindowWidth() / 100, ofGetWindowHeight() - 300);
		font.drawString(setText, font.stringWidth(setText) / 4, 0);
		ofPopMatrix();
		string titleText = "Untilted Space Shooter";
		ofPushMatrix();

		ofTranslate(ofGetWindowWidth() / 30, ofGetWindowHeight() / 4);
		font2.drawString(titleText, font.stringWidth(titleText) / 15, 0);
		ofPopMatrix();
	}
	else {
		//Sets up the score counter
		ofSetColor(255, 255, 255);
		std::string myScore = std::to_string(score);
		string setText = "Score:" + myScore;
		ofPushMatrix();
		ofTranslate(ofGetWindowWidth() - 225, 50);
		ofSetColor(0, 255, 255);

		font3.drawString("Score:" + myScore, font.stringWidth(setText) / 2.0, 0);
		ofPopMatrix();
		std::string myHealth = std::to_string(health);
		string setHealthText = "Health:" + myHealth;
		ofPushMatrix();
		ofTranslate((ofGetWindowWidth() / 2.0) - 150, 50);
		if (health > 50) {
			ofSetColor(255, 255, 0);
		}
		else {
			ofSetColor(255, 0, 0);
		}
		font3.drawString("Health:" + myHealth, font.stringWidth(setHealthText) / 2.0, 0);
		ofPopMatrix();
	}
	ofSetColor(255, 255, 255);
	for (int i = 0; i < emitters.size(); i++) {
		emitters[i]->draw();
	}
	ofSetColor(255, 255, 255);
	for (int i = 0; i < invaderList.size(); i++) {
		invaderList[i]->draw();
	}

	if (!bHide) {
		gui.draw();
	}

	ofPushMatrix();
	ofTranslate(explodeHereX, explodeHereY);
	explosionOne.draw();
	ofPopMatrix();
	ofSetColor(255, 255, 255);

	ofSetColor(255, 255, 255);
	ofPushMatrix();
	fastInvader->draw();
	ofPopMatrix();
	ofSetColor(255, 255, 255);

	if (health <= 0) {
		enemyOneRate = 0;
		enemyTwoRate = 0;
		enemyThreeRate = 0;
		shootRate = 0;
		difficultyModifier = 0;
		ofImage background;
		background.load("images/space.png");
		background.draw(0, 0);
		ofSetColor(255, 255, 255);
		ofPopMatrix();
		std::string endScore = std::to_string(score);
		string setEndScore = "Your Score was:" + endScore;
		ofPushMatrix();
		ofTranslate(ofGetWindowWidth() / 6, ofGetWindowHeight() / 2.0);
		font3.drawString("You died! Your score was: " + endScore, font.stringWidth(setEndScore) / 4.0, 0);
	}

}


//Checks via contact distance(height of images) if collision happened. For the Death Orb, it must cover entire player as it is an "instant death"
void ofApp::checkPlayerCollision() {

	ofVec3f deathOrbPosition = fastInvadePosition->position;
	float deathOrbDistance = emitters[emitters.size() - 1]->position.distance(deathOrbPosition);
	float playerContact = playerContactDistance;
	if (deathOrbDistance <= (playerContact + deathOrbHeight * 2) && gameStarted) {
		health = 0;
	}

	for (int i = 0; i < enemyOne->sprites.size(); i++) {
		ofVec3f enemyPosition = enemyOne->sprites[i].position;
		float enemyContact = enemyOne->sprites[i].spriteContactDistance;
		float playerDistance = emitters[emitters.size() - 1]->position.distance(enemyPosition);
		float playerContact = playerContactDistance;
		if (playerDistance <= (playerContact + enemyContact)) {
			timeOfHit = ofGetElapsedTimeMillis();
			if (timeOfHit / 700 > lastHit / 700) {
				health -= 5;
				lastHit = timeOfHit;
				ofSetColor(255, 0, 0);
			}
			break;
		}
	}
	for (int i = 0; i < enemyTwo->sprites.size(); i++) {
		ofVec3f enemyPosition = enemyTwo->sprites[i].position;
		float enemyContact = enemyTwo->sprites[i].spriteContactDistance;
		float playerDistance = emitters[emitters.size() - 1]->position.distance(enemyPosition);
		float playerContact = playerContactDistance;
		if (playerDistance <= (playerContact + enemyContact)) {
			timeOfHit = ofGetElapsedTimeMillis();
			if (timeOfHit / 700 > lastHit / 700) {
				health -= 5;
				lastHit = timeOfHit;
				ofSetColor(255, 0, 0);
			}
			break;
		}
	}
	for (int i = 0; i < enemyThree->sprites.size(); i++) {
		ofVec3f enemyPosition = enemyThree->sprites[i].position;
		float enemyContact = enemyThree->sprites[i].spriteContactDistance;
		float playerDistance = emitters[emitters.size() - 1]->position.distance(enemyPosition);
		float playerContact = playerContactDistance;
		if (playerDistance <= (playerContact + enemyContact)) {
			timeOfHit = ofGetElapsedTimeMillis();
			if (timeOfHit / 700 > lastHit / 700) {
				health -= 5;
				lastHit = timeOfHit;
				ofSetColor(255, 0, 0);
			}
			break;
		}
	}


}

//Activates our "death orb" invader using physics to follow the player. 
void ofApp::fastMovement() {

	float goToX = fastInvadePosition->position.x - emitters[emitters.size() - 1]->position.x;
	float goToY = fastInvadePosition->position.y - emitters[emitters.size() - 1]->position.y;
	ofVec3f goTo = ofVec3f(goToX, goToY, 0);
	ofVec3f editedGoTo = ofVec3f(10 * (goTo.x*-1), 10 * goTo.y*-1, 0);
	sys->addForce(new ThrusterForce(editedGoTo));


}

//Checks if player missiles have collided with enemies and if so remove them
void ofApp::checkCollision() {
	//In charge of checking for collisions, a nested for loop, the first loop for the turret and other 3 for invader sprites

	for (int j = 0; j < copyOfTurret->sprites.size(); j++) {

		float turretContact = copyOfTurret->sprites[j].spriteContactDistance;
		ofVec3f missilePosition = copyOfTurret->sprites[j].position;


		//Collision is based off the sum of two sprite contact distances. If the current distance is smaller than the total contact then it collided
		for (int i = 0; i < enemyOne->sprites.size(); i++) {
			float enemyContact = enemyOne->sprites[i].spriteContactDistance;
			float totalContact = turretContact + enemyContact;
			ofVec3f enemyPosition = enemyOne->sprites[i].position;
			float distance = missilePosition.distance(enemyPosition);



			//Checks for collision
			if (distance <= totalContact) {
				enemyOne->sprites[i].lifespan = 0;
				copyOfTurret->sprites[j].lifespan = 0;
				explosion.play();
				score += 10;
				explodeHereX = enemyPosition.x;
				explodeHereY = enemyPosition.y;

				explosionOne.sys->reset();
				explosionOne.start();



				ofSetColor(255, 255, 255);
				continue;
			}
		}

		for (int i = 0; i < enemyTwo->sprites.size(); i++) {
			float enemyContact = enemyTwo->sprites[i].spriteContactDistance;
			float totalContact = turretContact + enemyContact;
			ofVec3f enemyPosition = enemyTwo->sprites[i].position;
			float distance = missilePosition.distance(enemyPosition);


			//checks for collision
			if (distance <= totalContact) {
				enemyTwo->sprites[i].lifespan = 0;
				copyOfTurret->sprites[j].lifespan = 0;
				explosion.play();
				score += 5;
				explodeHereX = enemyPosition.x;
				explodeHereY = enemyPosition.y;


				explosionOne.sys->reset();
				explosionOne.start();


				ofSetColor(255, 255, 255);
				continue;

			}
		}

		for (int i = 0; i < enemyThree->sprites.size(); i++) {
			float enemyContact = enemyThree->sprites[i].spriteContactDistance;
			float totalContact = turretContact + enemyContact;
			ofVec3f enemyPosition = enemyThree->sprites[i].position;
			float distance = missilePosition.distance(enemyPosition);

			//checks for collision
			if (distance <= totalContact) {
				enemyThree->sprites[i].lifespan = 0;
				copyOfTurret->sprites[j].lifespan = 0;
				score += 5;
				explosion.play();
				explodeHereX = enemyPosition.x;
				explodeHereY = enemyPosition.y;

				explosionOne.sys->reset();
				explosionOne.start();

				ofSetColor(255, 255, 255);
				continue;

			}
		}


	}


}

//--------------------------------------------------------------

//Moves turret with the mouse utilizing matrixes that we learned
void ofApp::mouseMoved(int x, int y) {
	ofVec3f myPosition = emitters[emitters.size() - 1]->position;
	if (gameStarted) {
		ofPushMatrix();
		ofTranslate(x, y);
		emitters[emitters.size() - 1]->setPosition(ofVec3f(x, y, 0));
		emitters[emitters.size() - 1]->position = ofVec3f(x, y, 0);
		ofPopMatrix();
		myMouse.x = x;
		myMouse.y = y;
		//turretEmitter->start();
	}


}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}
// In charge of keeping track of WASD keys as well as spacebar for movement and shooting
void ofApp::keyPressed(int key) {
	Emitter *emitToStart = emitters[emitters.size() - 1];
	int movementIncrement = 10;
	switch (key) {
	case 'C':
	case 'c':
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;


	case 'H':
	case 'h':
		bHide = !bHide;
		break;
	case 'r':
		break;


	case 'u':
		break;
	case OF_KEY_LEFT:
		if (gameStarted) {

			if (myMouse.x - movementIncrement > 0) {
				myMouse.x = myMouse.x - movementIncrement;
				emitters[emitters.size() - 1]->setPosition(ofVec3f(myMouse.x, myMouse.y, 0));
				emitters[emitters.size() - 1]->position = ofVec3f(myMouse.x, myMouse.y, 0);
			}
		}
		break;
	case OF_KEY_DOWN:
		if (gameStarted) {
			if (myMouse.y + movementIncrement < ofGetWindowHeight()) {
				myMouse.y = myMouse.y + movementIncrement;
				emitters[emitters.size() - 1]->setPosition(ofVec3f(myMouse.x, myMouse.y, 0));
				emitters[emitters.size() - 1]->position = ofVec3f(myMouse.x, myMouse.y, 0);
			}
		}
		break;
	case OF_KEY_UP:
		if (gameStarted) {
			if (myMouse.y - movementIncrement > 0) {
				myMouse.y = myMouse.y - movementIncrement;
				emitters[emitters.size() - 1]->setPosition(ofVec3f(myMouse.x, myMouse.y, 0));
				emitters[emitters.size() - 1]->position = ofVec3f(myMouse.x, myMouse.y, 0);
			}
		}

		break;
	case OF_KEY_RIGHT:
		if (gameStarted) {
			if (myMouse.x + movementIncrement < ofGetWindowWidth()) {
				myMouse.x = myMouse.x + movementIncrement;

				emitters[emitters.size() - 1]->setPosition(ofVec3f(myMouse.x, myMouse.y, 0));
				emitters[emitters.size() - 1]->position = ofVec3f(myMouse.x, myMouse.y, 0);
			}
		}

		break;
	case OF_KEY_ALT:
		break;
	case OF_KEY_CONTROL:
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;
	case ' ':
		emitToStart->constant();
		gameStarted = true;
		for (int i = 0; i < invaderList.size(); i++) {
			Emitter *invader = invaderList[i];
			invader->constant();
		}

		break;
	}
}



void ofApp::keyReleased(int key) {
	switch (key) {
	case OF_KEY_LEFT:
	case OF_KEY_RIGHT:
	case OF_KEY_UP:
	case OF_KEY_DOWN:
		break;
	case OF_KEY_ALT:
		break;
	case OF_KEY_CONTROL:
		break;
	case OF_KEY_SHIFT:
		break;
	case ' ':

		break;
	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

