#pragma once
#ifndef __PARTICLE__
#define __PARTICLE__
#include "DisplayObject.h"

class Particle : public DisplayObject
{
public:
	// constructors
	Particle();
	
	// destructor
	~Particle();
	
	// life cycle functions
	void draw() override;
	void update() override;
	void clean() override;

	// animation
	void Animate();

	//getters
	bool isBeingThrown();
	float getDeltaX();
	float getDeltaY();
	float getDeltaTotalX();
	float getDeltaTotalY();
	glm::vec2 getInitialPos();
	float getInitialVelocity();
	float getLaunchAngle();
	float getGravity();
	float getTotalFlightTime();
	bool getIsAnimating();
	int getAnimationCount();

	//setters
	void setIsBeingThrown(bool beingThrown);
	void setInitialPos(glm::vec2 pos);
	void setInitialVelocity(float vel);
	void setLaunchAngle(float angle);
	void setGravity(float grav);
	void clearThrownSettings();
	void setIsAnimated(bool isAnimating);
	void setIsPlaying(bool playing);
	
private:
	float m_gravity;
	bool m_isGrounded;
	bool m_isBeingThrown;
	bool m_isAnimating;

	double launchAngleDeg;
	int spinAngle;
	float initialVelocity;
	int pixelsPerMeter;
	glm::vec2 initialPos;
	int animationTime;
	int animationCount;

	float totalFlightTime;
	float deltaX;
	float deltaY;
	bool isPlaying;
};

#endif /* defined (__OBSTACLE__) */