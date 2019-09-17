#pragma once

#include <iostream>
#include <vector>

#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Primitives.h"

class Mesh
{
private:
	Vertex* vertexArray;
	unsigned nrOfVertices;
	GLuint* indexArray;
	unsigned nrOfIndices;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 ModelMatrix;

	void initVAO();
	void updateUniforms( Shader* shader );
	void updateModelMatrix();

public:
	Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray,	const unsigned& nrOfIndices, 
			glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f));
	Mesh(Primitive* primitive, glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f));
	~Mesh();

	void update();
	void render(Shader* shader);

	/* ACCESSORS */

	/* MODIFIERS */
	void setPosition( const glm::vec3& position );
	void setRotation( const glm::vec3& rotation );
	void setScale( const glm::vec3& scale );

	/* FUNCTIONS*/
	void move( const glm::vec3& position );
	void rotate( const glm::vec3& rotation );
	void scaleUp( const glm::vec3& scale );

};