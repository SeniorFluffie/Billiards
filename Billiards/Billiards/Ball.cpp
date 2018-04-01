#include "Ball.h"

Ball::Ball(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements)
	: GameEntity(entityPos, entityScale, entityRotationAmount, entityTexture, entityNumElements)
{
	radius = (entityScale.x + entityScale.y) / 4;
}

void Ball::launch(float deltaTime, glm::vec3 accel) {
	if (canLaunch) {
		// update acceleration
		acceleration = (accel * power) / mass;
		// now update velocity based on that acceleration
		velocity.x += acceleration.x;
		velocity.y += acceleration.y;
		// reset acceleration
		acceleration *= 0.0f;

		launchCounter = 0.0f;
		canLaunch = false;
	}
}

void Ball::reset() {
	// reset all values
	position = glm::vec3(-0.6f, 0.0f, 0.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	launchCounter = 100.0f;
	inPocket = false;
}

void Ball::update(float deltaTime) {
	// update ball position
	position.x += velocity.x * deltaTime;
	position.y += velocity.y * deltaTime;
	// friction slowdown
	slowDown(deltaTime);
	// stop moving if too slow
	speedCheck(deltaTime);
	// table collision
	tableCollision(deltaTime);

	// delay for shooting the cueball
	if (launchCounter < launchDelay)
		launchCounter += 0.5f;
	else
		canLaunch = true;

	// pocket collision
	pocketCollision();
}

void Ball::speedCheck(float deltaTime) {
	// if they are moving below the bound
	if ((fabs(velocity.x) + fabs(velocity.y) + fabs(velocity.z)) <= movementBound) {
		// reset velocity
		velocity.x = 0.0f;
		velocity.y = 0.0f;
	}
}

void Ball::slowDown(float deltaTime) {
	// add the friction to the ball
	velocity *= friction;
}

float Ball::distance(glm::vec3 b1, glm::vec3 b2) {
	return sqrt(pow(b2.x - b1.x, 2) + pow(b2.y - b1.y, 2));
}

bool Ball::isColliding(Ball ball) {
	return distance(position, ball.position) < radius + ball.radius;
}

glm::vec3 Ball::ballCollision(Ball ball) {
	// find the distance vector between the balls
	glm::vec3 distVector = glm::vec3(position.x - ball.position.x, position.y - ball.position.y, 0.0f);
	// find the distance value
	float distValue = distance(distVector, glm::vec3(0.0f, 0.0f, 0.0f));

	// get the collision normal
	glm::vec3 normal = distVector;
	// normalize the vector
	FLT_NORMALIZE(normal);
	// divide by magnitude of the vector 
	normal /= distValue;

	// get velocity
	glm::vec3 relativeVelocity = velocity - ball.velocity;
	// get velocity value 
	float velocityValue = glm::dot(relativeVelocity, normal);

	// calculate impulse
	float impulse = -(1 + ballRestitution) * velocityValue / (mass + ball.mass);

	// calculate momentum
	glm::vec3 momentum = impulse * mass * normal;
	glm::vec3 oppositeMomentum = impulse * ball.mass * normal;

	// update velocity
	velocity += momentum;
	ball.velocity -= momentum;

	// return other ball velocity (as it cannot be updated from within this class)
	return ball.velocity;
}

void Ball::pocketCollision() {	
	for (glm::vec2 coord : pocketCoords) {
		if (position.x - 0.03f < coord.x && position.x + 0.03f > coord.x && position.y + 0.03f > coord.y && position.y - 0.03f < coord.y)
			inPocket = true;
	}
}

void Ball::tableCollision(float deltaTime) {
	// flag to determine if the ball collided
	bool hitWall = false;
	// normal for collisions
	glm::vec3 normal;

	// set normal (we are only checking left and right border so only x)
	if (position.x < -0.87f || position.x > 0.87f) {
		// if left border
		if (position.x < 0.0f)
			position.x = -0.87f;
		// if right border
		else
			position.x = 0.87f;
		// set the normal to be the wall
		normal = glm::vec3(1.0f, 0.0f, 0.0f);
		// collision happened
		hitWall = true;
	}

	// set normal (we are only checking top and bottom border so only y)
	else if (position.y < -0.58f || position.y > 0.58f) {
		// if left border
		if (position.y < 0.0f)
			position.y = -0.58f;
		// if right border
		else
			position.y = 0.58f;
		// set the normal to be the wall
		normal = glm::vec3(0.0f, 1.0f, 0.0f);
		// collision happened
		hitWall = true;
	}

	// if the ball actually did collide
	if (hitWall) {
		// as the table does not move, it is only ball velocity
		glm::vec3 relativeVelocity = velocity;
		// impulse calculation
		glm::vec3 impulse = (-(1.0f + wallRestitution) * glm::dot(relativeVelocity, normal) / inverseMass) * normal;
		// update momentum
		glm::vec3 momentum = inverseMass * impulse;
		// update velocity
		velocity += momentum;
	}
}