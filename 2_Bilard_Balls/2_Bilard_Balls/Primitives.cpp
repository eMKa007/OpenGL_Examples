#include "Primitives.h"

/* --------------------------------------  PRIMITIVE CLASS  ---------------------------------------------------------*/

/*	----------------------------------------------------------
*	Default class constructor
*	Parameters: none
*/
Primitive::Primitive(){}

/*	----------------------------------------------------------
*	Default class destructor
*	Parameters: none
*		Used to: delete previously created primitive instance
*/
Primitive::~Primitive(){}

/*	----------------------------------------------------------
*	Function name: set()
*	Parameters:	const Vertex* vertices - pointer to array of Vertices
*				const unsigned nrOfVertices - number of vertices in given array
				const GLuint* indices - pointer to array of indices
				const unsigned nrOfIndice - number of indices in given array
*	Used to: Set class variables: vertices and indices
*	Return:	void
*/
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

/* --------------------------------------  TRIANGLE CLASS  --------------------------------------------------------- */

/*	----------------------------------------------------------
*	Default class constructor
*	Parameters: none
*		Temporary coordinates are hardcoded inside constructor.
*/
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

/*	----------------------------------------------------------
*	Default class destructor
*	Parameters: none
*	Used to: delete previously created class instance
*/
Triangle::~Triangle()
{
}

/* --------------------------------------  QUAD CLASS  --------------------------------------------------------- */

/*	----------------------------------------------------------
*	Default class constructor
*	Parameters: none
*		Temporary coordinates are hardcoded inside constructor.
*/
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

/*	----------------------------------------------------------
*	Default class destructor
*	Parameters: none
*	Used to: delete previously created class instance
*/
Quad::~Quad()
{

}

/* --------------------------------------  Pyramid CLASS  --------------------------------------------------------- */

/*	----------------------------------------------------------
*	Default class constructor
*	Parameters: none
*		Temporary coordinates are hardcoded inside constructor.
*/
Pyramid::Pyramid()
{
	Vertex vertices[] =
	{
		// Position						// Color					// texcoord				// Normal
		// Triangle Front
		glm::vec3(0.f, 0.5f, 0.f),		glm::vec3(1.f, 0.f, 0.f),	glm::vec2(0.5f, 1.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 0
		glm::vec3(-0.5f, -0.5f, 0.5f),	glm::vec3(0.f, 1.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 1
		glm::vec3(0.5f, -0.5f, 0.5f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 2

		// Triangle Left
		glm::vec3(0.f, 0.5f, 0.f),		glm::vec3(1.f, 1.f, 0.f),	glm::vec2(0.5f, 1.f),	glm::vec3(-1.f, 0.f, 0.f),	// Vertex 0
		glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(0.f, 0.f),	glm::vec3(-1.f, 0.f, 0.f),	// Vertex 1
		glm::vec3(-0.5f, -0.5f, 0.5f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.f, 0.f),	glm::vec3(-1.f, 0.f, 0.f),	// Vertex 2

		// Triangle Back
		glm::vec3(0.f, 0.5f, 0.f),		glm::vec3(1.f, 1.f, 0.f),	glm::vec2(0.5f, 1.f),	glm::vec3(0.f, 0.f, -1.f),	// Vertex 0
		glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(0.f, 0.f, -1.f),	// Vertex 1
		glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 0.f, -1.f),	// Vertex 2

		// Triangle Right
		glm::vec3(0.f, 0.5f, 0.f),		glm::vec3(1.f, 1.f, 0.f),	glm::vec2(0.5f, 1.f),	glm::vec3(1.f, 0.f, 0.f),	// Vertex 0
		glm::vec3(0.5f, -0.5f, 0.5f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(0.f, 0.f),	glm::vec3(1.f, 0.f, 0.f),	// Vertex 1
		glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.f, 0.f),	glm::vec3(1.f, 0.f, 0.f),	// Vertex 2
		
	};
	unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

	this->set(vertices, nrOfVertices, nullptr, 0);
}

/*	----------------------------------------------------------
*	Default class destructor
*	Parameters: none
*	Used to: delete previously created class instance
*/
Pyramid::~Pyramid()
{
	
}

/* --------------------------------------  Box CLASS  --------------------------------------------------------- */

/*	----------------------------------------------------------
*	Default class constructor
*	Parameters: none
*		Temporary coordinates are hardcoded inside constructor.
*/
Box::Box()
{
	Vertex vertices[] =
	{
		glm::vec3(-2.f, 2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 0
		glm::vec3(-2.f, -2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 1
		glm::vec3(2.f, -2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 2
		glm::vec3(2.f, 2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 1.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 3
		glm::vec3(-2.f, 2.f, -2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(-1.f, 0.f, 0.f),	// Vertex 4
		glm::vec3(-2.f, -2.f, -2.f),	glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(-1.f, 0.f, 0.f),	// Vertex 5
		glm::vec3(2.f, -2.f, -2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 0.f),	glm::vec3(1.f, 0.f, 0.f),	// Vertex 6
		glm::vec3(2.f, 2.f, -2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 1.f),	glm::vec3(1.f, 0.f, 0.f),	// Vertex 7

		/*
		// Position						// Color					// texcoord				// Normal
		// Square 1
		glm::vec3(-2.f, 2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 0
		glm::vec3(-2.f, -2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 1
		glm::vec3(2.f, -2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 2
		glm::vec3(-2.f, 2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 0
		glm::vec3(2.f, -2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 2
		glm::vec3(2.f, 2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 1.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 3

		// Square 2
		glm::vec3(2.f, 2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(1.f, 0.f, 0.f),	// Vertex 3
		glm::vec3(2.f, -2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(1.f, 0.f, 0.f),	// Vertex 2
		glm::vec3(2.f, -2.f, -2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 0.f),	glm::vec3(1.f, 0.f, 0.f),	// Vertex 6
		glm::vec3(2.f, 2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(1.f, 0.f, 0.f),	// Vertex 3
		glm::vec3(2.f, -2.f, -2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 0.f),	glm::vec3(1.f, 0.f, 0.f),	// Vertex 6
		glm::vec3(2.f, 2.f, -2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 1.f),	glm::vec3(1.f, 0.f, 0.f),	// Vertex 7

		// Square 3
		glm::vec3(2.f, 2.f, -2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(0.f, 0.f, -1.f),	// Vertex 7
		glm::vec3(2.f, -2.f, -2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(0.f, 0.f, -1.f),	// Vertex 6
		glm::vec3(-2.f, -2.f, -2.f),	glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 0.f, -1.f),	// Vertex 5
		glm::vec3(2.f, 2.f, -2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(0.f, 0.f, -1.f),	// Vertex 7
		glm::vec3(-2.f, -2.f, -2.f),	glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 0.f, -1.f),	// Vertex 5
		glm::vec3(-2.f, 2.f, -2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 1.f),	glm::vec3(0.f, 0.f, -1.f),	// Vertex 4

		// Square 4
		glm::vec3(-2.f, 2.f, -2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(-1.f, 0.f, 0.f),	// Vertex 4
		glm::vec3(-2.f, -2.f, -2.f),	glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(-1.f, 0.f, 0.f),	// Vertex 5
		glm::vec3(-2.f, -2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 0.f),	glm::vec3(-1.f, 0.f, 0.f),	// Vertex 1
		glm::vec3(-2.f, 2.f, -2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(-1.f, 0.f, 0.f),	// Vertex 4
		glm::vec3(-2.f, -2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 0.f),	glm::vec3(-1.f, 0.f, 0.f),	// Vertex 1
		glm::vec3(-2.f, 2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 1.f),	glm::vec3(-1.f, 0.f, 0.f),	// Vertex 0

		// Square 5
		glm::vec3(-2.f, 2.f, -2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(0.f, 1.f, 0.f),	// Vertex 4
		glm::vec3(-2.f, 2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(0.f, 1.f, 0.f),	// Vertex 0
		glm::vec3(2.f, 2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 1.f, 0.f),	// Vertex 3
		glm::vec3(-2.f, 2.f, -2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(0.f, 1.f, 0.f),	// Vertex 4
		glm::vec3(2.f, 2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 1.f, 0.f),	// Vertex 3
		glm::vec3(2.f, 2.f, -2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 1.f),	glm::vec3(0.f, 1.f, 0.f),	// Vertex 7

		// Square 6
		glm::vec3(-2.f, -2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 1
		glm::vec3(-2.f, -2.f, -2.f),	glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 5
		glm::vec3(2.f, -2.f, -2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 6
		glm::vec3(-2.f, -2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 1
		glm::vec3(2.f, -2.f, -2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 6
		glm::vec3(2.f, -2.f, 2.f),		glm::vec3(0.f, 0.f, 0.f),	glm::vec2(1.f, 1.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 2

		*/
	};
	unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

	/* 
	 * Determine the sequence of vertices to be drawn.
	 */
	GLuint indices[] =
	{
		0, 1, 1, 2, 2, 3, 3, 0,
		2, 6, 6, 7, 7, 3,
		6, 5, 5, 4, 4, 7,
		5, 1, 4, 0
	};
	unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

	this->set(vertices, nrOfVertices, indices, nrOfIndices);
}

/*	----------------------------------------------------------
*	Default class destructor
*	Parameters: none
*	Used to: delete previously created class instance
*/
Box::~Box()
{
	
}