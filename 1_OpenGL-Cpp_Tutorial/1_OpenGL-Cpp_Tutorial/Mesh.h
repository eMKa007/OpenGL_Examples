#pragma once

#include <iostream>
#include <vector>

#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"

class Mesh
{
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 ModelMatrix;

	void initVertexData( Vertex* vertexArray, const unsigned& nrofVertices, 
							GLuint* indexArray, const unsigned& nrOfIndices );
	void initVAO();
	void initModelMatrix();
	void updateUniforms( Shader* shader );

public:
	Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray,	const unsigned& nrOfIndices);
	~Mesh();

	void update();
	void render(Shader* shader);

};