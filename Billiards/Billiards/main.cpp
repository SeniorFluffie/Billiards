#include <iostream>
#include <stdexcept>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h> 
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>
#include <chrono>
#include <thread>
#include <vector>

#include "Shader.h"
#include "Window.h"
#include "PoolTable.h"

using namespace std;

// Macro for printing exceptions
#define PrintException(exception_object)\
	std::cerr << exception_object.what() << std::endl

// Globals that define the OpenGL window and viewport
const std::string window_title_g = "Billiards";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.0);

// Global texture info
const int numTextures = 12;
GLuint tex[numTextures];

// Create the geometry for a square (with two triangles)
// Return the number of array elements that form the square
int CreateSquare(void) {
	// The face of the square is defined by four vertices and two triangles

	GLfloat vertex[] = {
		// Position		 Color				    Texcoords
		 -0.5f, 0.5f,	 1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // Top-left
		  0.5f, 0.5f,	 0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // Top-right
		 0.5f, -0.5f,	 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f,	 1.0f, 1.0f, 1.0f,		0.0f, 1.0f  // Bottom-left
	};

	GLuint face[] = {0, 1, 2,
					 2, 3, 0 };

	GLuint vbo, ebo;

	// Create buffer for vertices
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	// Create buffer for faces (index buffer)
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face), face, GL_STATIC_DRAW);

	// Return number of elements in array buffer (6 in this case)
	return sizeof(face) / sizeof(GLuint);
}

void setthisTexture(GLuint w, char * fname)
{
	glBindTexture(GL_TEXTURE_2D, w);

	int width, height;
	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	// Texture Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Texture Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void setallTexture(void)
{
	glGenTextures(numTextures, tex);
	// setup table textures
	setthisTexture(tex[0], "table.png");
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	// setup ball textures
	for (char i = 0; i < numTextures - 1; i++) {
		// create string of image file
		string fileString = "ball_" + to_string(i) + ".png";
		// put into a char array
		char * fileChar = &fileString[0];
		// set textures
		setthisTexture(tex[i+1], fileChar);
		// bind textures
		glBindTexture(GL_TEXTURE_2D, tex[i+1]);
	}
}

// Main function that builds and runs the game
int main(void) {
	try {
		// Setup window
		Window window(window_width_g, window_height_g, window_title_g);

		// Set up z-buffer for rendering
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// Enable Alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Create geometry of the square
		int size = CreateSquare();

		// Set up shaders
		Shader shader("shader.vert", "shader.frag");

		// Set Textures
		setallTexture();

		// Setup Game
		PoolTable billiards = PoolTable(window.getWindow(), glm::vec2(window_width_g, window_height_g), tex, size, numTextures - 1);

		// Run the main loop
		while (!glfwWindowShouldClose(window.getWindow())) {
			// Clear background
			window.clear(viewport_background_color_g);

			// Select proper shader program to use
			shader.enable();

			// Run and render the game
			billiards.runGame();
			billiards.render(shader);

			glDisable(GL_TEXTURE_2D);
			// Render background - turn off depth testing so it will render behind everything
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_DEPTH_TEST);

			// Update other events like input handling
			glfwPollEvents();

			// Push buffer drawn in the background onto the display
			glfwSwapBuffers(window.getWindow());
		}
	}
	catch (std::exception &e) {
		// print exception and sleep so error can be read
		PrintException(e);
		std::this_thread::sleep_for(std::chrono::milliseconds(100000));
	}
	return 0;
}
