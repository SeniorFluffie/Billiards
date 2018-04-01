#pragma once

#include "GameEntity.h"

class Ball : public GameEntity {
public:
	// constructors
	Ball(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements);

	// control methods
	void launch(float deltaTime, glm::vec3 accel);

	// pocket methods
	glm::vec2 pocketCoords[6] = {glm::vec2(-0.84f,  0.56f), glm::vec2(0.00f,  0.58f), glm::vec2(0.84f,  0.56f), 
		glm::vec2(-0.84f, -0.56f), glm::vec2(0.00f, -0.58f), glm::vec2(0.84f, -0.56f)};

	bool inPocket = false;

	// collision methods
	bool isColliding(Ball ball);
	glm::vec3 ballCollision(Ball ball);

	// update methods
	void update(float deltaTime);
	void reset();

	// get methods
	inline glm::vec3 getPosition() { return position; }
	inline glm::vec3 getVelocity() { return velocity; }

	// set methods
	inline void setPosition(glm::vec3 newPos) { position = newPos; }
	inline void setVelocity(glm::vec3 newVel) { velocity = newVel; }
	inline void setMass(float newMass) { mass = newMass; }

protected:
	// counter for launching
	float launchCounter;
	// timer for launching
	float launchDelay = 70.0f;

	// flag for launching
	bool canLaunch;

	// mass of the ball
	float mass = 1.0f;
	// inverse mass
	float inverseMass = 1 / mass;
	// radius of the ball
	float radius;

	// slows down the ball
	float friction = 0.99f;
	// value for when the balls should be considered 'not moving'
	float movementBound = 0.025f;

	// acceleration of the ball
	glm::vec3 acceleration;
	// velocity of the ball
	glm::vec3 velocity;
	// coefficient of restitution for different objects
	float wallRestitution = 1.0f;
	float ballRestitution = 1.0f;
	// power coefficient to make the cue ball shoot faster
	glm::vec3 power = glm::vec3(5.0f, 5.0f, 0.0f);

	// calculation methods
	float distance(glm::vec3 b1, glm::vec3 b2);
	void slowDown(float deltaTime);
	void speedCheck(float deltaTime);
	void tableCollision(float deltaTime);
	void pocketCollision();
};