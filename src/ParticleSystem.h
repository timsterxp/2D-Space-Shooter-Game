#pragma once
/* Project 1

By Timothy Nguyen  CS 134 Spring 2020*/
#include "ofMain.h"
#include "Particle.h"


//  Pure Virtual Function Class - must be subclassed to create new forces.
//
class ParticleForce {
protected:
public:
	bool applyOnce = false;
	bool applied = false;
	virtual void updateForce(Particle *) = 0;
};

class ParticleSystem {
public:
	void add(const Particle &);
	void addForce(ParticleForce *);
	void remove(int);
	void update();
	void setLifespan(float);
	void reset();
	int removeNear(const ofVec3f & point, float dist);
	void draw();
	vector<Particle> particles;
	vector<ParticleForce *> forces;
};



// Some convenient built-in forces
//
class ThrusterForce : public ParticleForce {
	ofVec3f direction;
public:
	void set(const ofVec3f &dir) { direction = dir; }
	ThrusterForce(const ofVec3f &dir);
	ThrusterForce() { direction.set(0, 0, 0); }
	void updateForce(Particle *);
};

class ImpulseRadialForce : public ParticleForce {
	float magnitude;
public:
	ImpulseRadialForce(float magnitude);
	void updateForce(Particle *);
};


