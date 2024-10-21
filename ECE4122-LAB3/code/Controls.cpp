/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 10/21/2024

Description:

*/

// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "Controls.h"

#include <iostream>

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3(5, 5, 5);

float theta = atan(sqrt(pow(position.x, 2) + pow(position.y, 2))/position.z);
float phi = atan(position.y / position.x);

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;



void computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	
	bool updatePosition = false;

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
		phi += 2 * deltaTime;
		updatePosition = true;
	}
	// Move forward
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
		phi -= 2 * deltaTime;
		updatePosition = true;
	}
	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		theta -= 2 * deltaTime;
		updatePosition = true;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		theta += 2 * deltaTime;
		updatePosition = true;
	}

	if (updatePosition) {
		glm::vec3 newPos(
		sin(theta) * cos(phi), 
		sin(theta) * sin(phi),
		cos(theta));
		position = glm::length(position) * newPos;
	}
	
	// Right vector
	glm::vec3 right = glm::vec3(
		cos(phi + 3.14f/2.0f), 
		sin(phi + 3.14f/2.0f),
		0
	);
	
	glm::vec3 direction(
		-position.x, 
		-position.y,
		-position.z);
	direction = glm::normalize(direction); 

	// Up vector
	glm::vec3 up = glm::cross( right, direction );
	
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
		if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}