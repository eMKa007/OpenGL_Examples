#include "Primitives.h"

Primitive::Primitive()
{
}

Primitive::~Primitive()
{
}

void Primitive::set(const Vertex* vertices, const unsigned nrOfVertices, 
						const GLuint* indices, const unsigned nrOfIndices)
{
	for( int i = 0; i<nrOfVertices; i++)
	{
		this->vertices.push_back(vertices[i]);
	}

	for( int i = 0; i<nrOfIndices; i++)
	{
		this->indices.push_back(indices[i]);
	}
}
/* --------------------------------------  TRIANGLE CLASS  ---------------------------------------------------------*/
Triangle::Triangle()
{
	/* 
	 * Declare vertex positions/color/texture coordinates.
	 * Will be used to draw triangle in this case.
	 */
	Vertex vertices[] =
	{
		// Position						// Color					// texcoord				// Normal
		glm::vec3(-0.5f, 0.5f, 0.f),	glm::vec3(1.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 0
		glm::vec3(-0.5f, -0.5f, 0.f),	glm::vec3(0.f, 1.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 1
		glm::vec3(0.5f, -0.5f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 2
		glm::vec3(0.5f, 0.5f, 0.f),		glm::vec3(1.f, 1.f, 0.f),	glm::vec2(1.f, 1.f),	glm::vec3(0.f, 0.f, 1.f)	// Vertex 3
		
	};
	unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

	/* 
	 * Determine the sequence of vertices to be drawn.
	 */
	GLuint indices[] =
	{
		0, 1, 2	// Triangle 1
	};
	unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

	this->set(vertices, nrOfVertices, indices, nrOfIndices);
}

Triangle::~Triangle()
{
}

/* --------------------------------------  QUAD CLASS  ---------------------------------------------------------*/
Quad::Quad()
{
	/* 
	 * Declare vertex positions/color/texture coordinates.
	 * Will be used to draw triangle in this case.
	 */
	Vertex vertices[] =
	{
		// Position						// Color					// texcoord				// Normal
		glm::vec3(-0.5f, 0.5f, 0.f),	glm::vec3(1.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 0
		glm::vec3(-0.5f, -0.5f, 0.f),	glm::vec3(0.f, 1.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 1
		glm::vec3(0.5f, -0.5f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 2
		glm::vec3(0.5f, 0.5f, 0.f),		glm::vec3(1.f, 1.f, 0.f),	glm::vec2(1.f, 1.f),	glm::vec3(0.f, 0.f, 1.f)	// Vertex 3
		
	};
	unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

	/* 
	 * Determine the sequence of vertices to be drawn.
	 */
	GLuint indices[] =
	{
		0, 1, 2,	// Triangle 1
		0, 2, 3		// Triangle 2
	};
	unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

	this->set(vertices, nrOfVertices, indices, nrOfIndices);
}

Quad::~Quad()
{

}
