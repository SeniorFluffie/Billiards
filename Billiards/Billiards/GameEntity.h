#pragma once

#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>

#include "Shader.h"

class GameEntity {
public:
	// constructor
	GameEntity(glm::vec3 &entityPos, glm::vec3 &entityScale, float entityRotationAmount, GLuint entityTexture, GLint entityNumElements);

	// update / render functions
	void update(float deltaTime);
	void render(Shader &shader);

	// get / set methods
	inline glm::vec3 getPosition() { return position; }

protected:
	// orientation
	glm::vec3 position, scale;
	float rotationAmount;

	// texture and texture size
	GLuint texture;
	GLint numElements;
};