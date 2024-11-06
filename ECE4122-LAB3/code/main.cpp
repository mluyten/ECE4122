/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 10/21/2024

Description:
This is the main loop for this program. It loads in all of the objects, runs the draw loop, and updates the view based on user inputs.
*/

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <utility>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

#include "Model.h"

// This struct makes it easier to keep track of the texture and locations of many coppies of the same chess piece asset.
struct Piece {
	Piece(std::string t, std::vector<glm::vec3> p) {
		texturePath = t;
		positions = p;
	}
	Piece()=default;
	std::string texturePath; // Texture for this mesh
	std::vector<glm::vec3> positions; // Locations of the coppies of this mesh
};

// Main loop
int main( void )
{
	// Initialize GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make macOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Lab3", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it is closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "StandardShading.vertexshader", "StandardShading.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	std::list<Model> models;
	models.emplace_back("board/board.obj", "board/chessboard.dds");
	std::vector<std::string> pieceTextures = {};

	std::map<int, Piece> pieces;
	// White Pawns
	glm::vec3 offset(2.2075, 0.5425, 0);
	pieces[0] = Piece("pieces/woodlight0.dds", {
				glm::vec3(offset.x, -0.16 + offset.y * 0, 0),
				glm::vec3(offset.x, -0.16 + offset.y * 1, 0),
				glm::vec3(offset.x, -0.16 + offset.y * 2, 0),
				glm::vec3(offset.x, -0.16 + offset.y * 3, 0),
				glm::vec3(offset.x, -0.16 + offset.y * 4, 0),
				glm::vec3(offset.x, -0.16 + offset.y * 5, 0),
				glm::vec3(offset.x, -0.16 + offset.y * 6, 0),
				glm::vec3(offset.x, -0.16 + offset.y * 7, 0)});
	// White Rooks
	offset = glm::vec3(2.73, 0.5425, 0);
	pieces[1] = Piece("pieces/woodlight1.dds", {
				glm::vec3(offset.x, -3.5425 + offset.y * 0, 0),
				glm::vec3(offset.x, -3.5425 + offset.y * 7, 0)});
	// White Knights
	offset = glm::vec3(2.73, 0.5425, 0);
	pieces[2] = Piece("pieces/woodlight2.dds", {
				glm::vec3(offset.x, -2.87 + offset.y * 1, 0),
				glm::vec3(offset.x, -2.87 + offset.y * 6, 0)});
	// White Bishops
	offset = glm::vec3(2.73, 0.5425, 0);
	pieces[3] = Piece("pieces/woodlight3.dds", {
				glm::vec3(offset.x, -2.18 + offset.y * 2, 0),
				glm::vec3(offset.x, -2.18 + offset.y * 5, 0)});
	// White Queen
	pieces[4] = Piece("pieces/woodlight4.dds", {
				glm::vec3(2.73, 0.1475, 0)});
	// White King
	pieces[5] = Piece("pieces/woodlight5.dds", {
				glm::vec3(2.73, 1.3725, 0)});
	for (int i = 0; i < 6; i++) {
		for (auto& pos : pieces[i].positions) models.emplace_back("pieces/pieces.obj", pieces[i].texturePath, i, pos, 3.14/2);
	}

	// Black Pawns
	offset = glm::vec3(-.545, 0.5425, 0);
	pieces[6] = Piece("pieces/wooddark0.dds", {
				glm::vec3(offset.x, -3.625 + offset.y * 0, 0),
				glm::vec3(offset.x, -3.625 + offset.y * 1, 0),
				glm::vec3(offset.x, -3.625 + offset.y * 2, 0),
				glm::vec3(offset.x, -3.625 + offset.y * 3, 0),
				glm::vec3(offset.x, -3.625 + offset.y * 4, 0),
				glm::vec3(offset.x, -3.625 + offset.y * 5, 0),
				glm::vec3(offset.x, -3.625 + offset.y * 6, 0),
				glm::vec3(offset.x, -3.65 + offset.y * 7, 0)});
	// Black Rooks
	offset = glm::vec3(-1.0675, 0.5425, 0);
	pieces[7] = Piece("pieces/wooddark1.dds", {
				glm::vec3(offset.x, -0.25 + offset.y * 0, 0),
				glm::vec3(offset.x, -0.25 + offset.y * 7, 0)});
	// Black Knights
	offset = glm::vec3(-1.0675, 0.5425, 0);
	pieces[8] = Piece("pieces/wooddark2.dds", {
				glm::vec3(offset.x, -0.935 + offset.y * 1, 0),
				glm::vec3(offset.x, -0.935 + offset.y * 6, 0)});
	// Black Bishops
	offset = glm::vec3(-1.0675, 0.5425, 0);
	pieces[9] = Piece("pieces/wooddark3.dds", {
				glm::vec3(offset.x, -1.62 + offset.y * 2, 0),
				glm::vec3(offset.x, -1.62 + offset.y * 5, 0)});
	// Black Queen
	pieces[10] = Piece("pieces/wooddark4.dds", {
				glm::vec3(-1.0675, -0.69, 0)});
	// Black King
	pieces[11] = Piece("pieces/wooddark5.dds", {
				glm::vec3(-1.0675, -0.825, 0)});
	for (int i = 6; i < 12; i++) {
		for (auto& pos : pieces[i].positions) models.emplace_back("pieces/pieces.obj", pieces[i].texturePath, i, pos, -3.14/2);
	}

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	GLuint LightToggle = glGetUniformLocation(programID, "lightToggle");

	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	do{

		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		//if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
			// printf and reset
			//nbFrames = 0;
			//lastTime += 1.0;
		//}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard input and get light mode
		int lightMode = computeMatricesFromInputs();
		// Set light mode in shader
		glUniform1i(LightToggle, lightMode);

		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();

		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		
		// Set the light up
		glm::vec3 lightPos = glm::vec3(0, 0, 5);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Draw all meshes
		for (auto& model : models) {
			model.draw(programID, TextureID, ProjectionMatrix, ViewMatrix);
		}

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Clean up all buffers we used
	for (auto& model : models) {
			model.cleanup();
		}

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

