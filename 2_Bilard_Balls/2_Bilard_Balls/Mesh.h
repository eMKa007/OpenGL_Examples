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
	glm::vec3 origin;
	glm::vec3 scale;
	glm::mat4 ModelMatrix;

	float dx;
	float dy;
	float dz;

	void initVAO();
	void updateUniforms( Shader* shader );
	void updateModelMatrix();

public:
	Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray,	const unsigned& nrOfIndices, 
			glm::vec3 position = glm::vec3(0.f), glm::vec3 origin = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f));
    Mesh(std::vector<Vertex>, std::vector<unsigned int> indices, std::vector<Texture> textures);
	Mesh(Primitive* primitive, glm::vec3 deltaMove = glm::vec3(0.f), glm::vec3 position = glm::vec3(0.f), glm::vec3 origin = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f));
	Mesh(const Mesh& obj);
	~Mesh();

	void update();
	void render(Shader* shader, GLenum mode);

	/* ACCESSORS */
	glm::vec3 getDeltaMove();
	glm::vec3 getPosition();

	/* MODIFIERS */
	void setPosition( const glm::vec3& position );
	void setOrigin( const glm::vec3& origin );
	void setRotation( const glm::vec3& rotation );
	void setScale( const glm::vec3& scale );
	void set_dxdydz(glm::vec3 new_dxdydz);
	void set_dx( float dx );
	void set_dy( float dy );
	void set_dz( float dz );
	void reverse_dx();
	void reverse_dy();
	void reverse_dz();

	/* FUNCTIONS*/
	void move( const glm::vec3& position );
	void rotate( const glm::vec3& rotation);
	void scaleUp( const glm::vec3& scale );

};