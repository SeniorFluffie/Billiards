#include "PoolTable.h"

PoolTable::PoolTable(GLFWwindow * window, glm::vec2 windowDimension, GLuint textures[], GLint size, int gameSize)
	: GameEntity(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 1.5f, 0.0f), 0.0f, textures[0], size)
{
	// set window / textures
	gameWindow = window;
	windowSize = windowDimension;
	textureSize = size;
	tableTextures = textures;
	
	// type of pool game
	numPoolBalls = gameSize;

	// get current time
	lastTime = glfwGetTime();
	
	// setup game
	setupPoolBalls();
}

void PoolTable::update(float deltaTime) {
	// update all pool balls
	for (int i = 0; i < numPoolBalls; i++)
		if (!poolBalls[i].inPocket)
			poolBalls[i].update(deltaTime);
	// update / collision methods
	ballCollision(deltaTime);
	pocketCheck();
}

void PoolTable::render(Shader &shader) {
	// update all pool balls
	for (int i = 0; i < numPoolBalls; i++)
		if (!poolBalls[i].inPocket)
			poolBalls[i].render(shader);
	// game entity
	this->GameEntity::render(shader);
}

void PoolTable::runGame() {
	// Calculate delta time
	float currentTime = glfwGetTime();
	float deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	// update game
	update(deltaTime);
	updateControls(deltaTime);
}

void PoolTable::setupPoolBalls() {
	// add cue ball to array
	poolBalls.push_back(Ball(initialSetup[0], glm::vec3(0.09f, 0.115f, 0.0f), 0.0f, tableTextures[1], textureSize));
	poolBalls.begin()->setMass(3.0f);

	for (int i = 2; i < numPoolBalls + 1; i++)
		poolBalls.push_back(Ball(initialSetup[i-1], glm::vec3(0.09f, 0.115f, 0.0f), 0.0f, tableTextures[i], textureSize));
}

void PoolTable::updateControls(float deltaTime) {
	// Get mouse location
	double mouseX, mouseY;
	glfwGetCursorPos(gameWindow, &mouseX, &mouseY);

	// Get mouse position in terms of canvas [-1, 1]
	float screenSpaceMouseX = (mouseX / windowSize.x) * 2 - 1;
	float screenSpaceMouseY = -((mouseY / windowSize.y) * 2 - 1);

	// if space is pressed and all the balls have stopped
	if (glfwGetKey(gameWindow, GLFW_KEY_SPACE) == GLFW_PRESS && ballsStopped == numPoolBalls) {
		// launch ball
		glm::vec3 addForce = glm::vec3(poolBalls[0].getPosition().x - screenSpaceMouseX, poolBalls[0].getPosition().y - screenSpaceMouseY, 0.0f);
		poolBalls[0].launch(deltaTime, addForce);
	}
}

void PoolTable::ballCollision(float deltaTime) {
	// Collision detection
	for (int i = 0; i < numPoolBalls; i++)
		for (int j = i + 1; j < numPoolBalls; j++)
			if (poolBalls[i].isColliding(poolBalls[j]))
				poolBalls[j].setVelocity(poolBalls[i].ballCollision(poolBalls[j]));
}

void PoolTable::pocketCheck() {
	ballsStopped = 0;
	// if in the pocket, remove the pool ball
	for (int i = 0; i < numPoolBalls; i++) {
		// if the ball is not moving, increment
		if (poolBalls[i].getVelocity() == glm::vec3(0.0f, 0.0f, 0.0f))
			ballsStopped++;
		// if the balls is in the pocket
		if (i != 0 && poolBalls[i].inPocket) {
			// remove it and change counter
			poolBalls.erase(poolBalls.begin() + i);
			numPoolBalls--;
		}
	}
	// if all balls are stopped (while loop here instead)
	if (ballsStopped == (numPoolBalls - 1) && poolBalls[0].inPocket)
		poolBalls[0].reset();
}