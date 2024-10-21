/*
Author: Matthew Luyten
Class: ECE4122
Last Date Modified: 10/21/2024

Description:

*/

#include "Model.h"

Model::Model (std::string modelPath, std::string texturePath, int index,  glm::vec3 pos, float ang) : position(pos), angle(ang) {
    bool res = loadAssImp(modelPath.c_str(), indices, indexed_vertices, indexed_uvs, indexed_normals, index);

    // Load the texture
    Texture = loadDDS(texturePath.c_str());

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);
}

void Model::cleanup () {
		// Cleanup VBO and shader
		glDeleteBuffers(1, &vertexbuffer);
		glDeleteBuffers(1, &uvbuffer);
		glDeleteBuffers(1, &normalbuffer);
		glDeleteBuffers(1, &elementbuffer);
}

void Model::draw(GLuint& programID, GLuint& TextureID, glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix) {
    // BUT the Model matrix is different (and the MVP too)
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(TextureID, 0);
    
    glm::mat4 ModelMatrix = glm::mat4(1.0);
    ModelMatrix = glm::translate(ModelMatrix, position);
    ModelMatrix = glm::rotate(ModelMatrix, angle, glm::vec3(0, 0, 1));
    glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    // Draw the triangles !
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
}