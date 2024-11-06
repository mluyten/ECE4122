/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 10/21/2024

Description:
This is the header for the model class. It is a utility for loading meshes
*/

#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <GL/glew.h>
#include <common/objloader.hpp>
#include <common/texture.hpp>

// This class represents one mesh. The mesh is loaded in from a .obj file and textured with a .dds file. It can be loaded at any
// position or angle around the z axis
class Model {
public:
	Model (std::string modelPath, std::string texturePath, int index=0,  glm::vec3 pos=glm::vec3(0,0,0), float ang=0);

	void cleanup ();

	void draw(GLuint& programID, GLuint& TextureID, glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix);

private:
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;

    // Desired angle and position of mesh
	glm::vec3 position;
	float angle;

	GLuint Texture;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;
	// Generate a buffer for the indices as well
	
};