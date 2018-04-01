#pragma once

#include <vector>
#include "Window.h"
#include "GameEntity.h"
#include "Ball.h"

using namespace std;

class PoolTable : public GameEntity {
public:
	// constructors
	PoolTable(GLFWwindow * window, glm::vec2 windowDimension, GLuint textures[], GLint size, int gameSize);

	// run / render methods
	void runGame();
	void render(Shader &shader);

private:
	// game window
	GLFWwindow * gameWindow;
	// window dimensions
	glm::vec2 windowSize;
	// texture size
	GLint textureSize;
	// object textures
	GLuint * tableTextures;

	// container to store the pool balls
	vector<Ball> poolBalls;
	int numPoolBalls;
	
	// intial game setup
	void setupPoolBalls();
	glm::vec3 initialSetup[11] = { glm::vec3(-0.6f, 0.0f, 0.0f),  glm::vec3(0.3f, 0.00f, 0.0f),  
								   glm::vec3(0.4f, 0.075f, 0.0f), glm::vec3(0.4f, -0.075f, 0.0f),
								   glm::vec3(0.5f, 0.15f, 0.0f),  glm::vec3(0.5f, 0.0f, 0.0f),   glm::vec3(0.5f, -0.15f, 0.0f),
								   glm::vec3(0.6f, 0.20f, 0.0f),  glm::vec3(0.6f, 0.065f, 0.0f), glm::vec3(0.6f, -0.065f, 0.0f), glm::vec3(0.6f, -0.20f, 0.0f)};

	// counter to determine whether all balls have stopped
	int ballsStopped;

	// time calculation
	double lastTime;

	// update methods
	void update(float deltaTime);
	void updateControls(float deltaTime);
	void ballCollision(float deltaTime);
	void pocketCheck();
};