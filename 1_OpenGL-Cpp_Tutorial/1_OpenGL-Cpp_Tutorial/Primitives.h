#pragma once

#include <vector>

#include <glew.h>
#include <glfw3.h>

#include "Vertex.h"


class Primitive
{
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

public:
	Primitive();
	virtual ~Primitive();

	/* FUNCTIONS */
	void set(const Vertex* vertices, const unsigned nrOfVertices, const GLuint* indices, const unsigned nrOfIndices );
	inline Vertex* getVertices() { return this->vertices.data(); }
	inline GLuint* getIndices() { return this->indices.data(); }
	inline const unsigned getNrOfVertices() { return this->vertices.size(); }
	inline const unsigned getNrOfIndices() { return this->indices.size(); }

};

class Triangle : public Primitive
{
private:

public:
	Triangle();
	~Triangle();
};

class Quad : public Primitive
{
private:

public:
	Quad();
	~Quad();
};