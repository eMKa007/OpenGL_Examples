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
		glm::vec3(-2.f, 2.f, 2.f),		glm::vec3(1.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 0
		glm::vec3(-2.f, -2.f, 2.f),		glm::vec3(0.f, 1.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 1
		glm::vec3(2.f, -2.f, 2.f),		glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 2
		glm::vec3(2.f, 2.f, 2.f),		glm::vec3(1.f, 0.f, 0.f),	glm::vec2(1.f, 1.f),	glm::vec3(0.f, 0.f, 1.f),	// Vertex 3
		glm::vec3(-2.f, 2.f, -2.f),		glm::vec3(0.f, 1.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(-1.f, 0.f, 0.f),	// Vertex 4
		glm::vec3(-2.f, -2.f, -2.f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(0.f, 0.f),	glm::vec3(-1.f, 0.f, 0.f),	// Vertex 5
		glm::vec3(2.f, -2.f, -2.f),		glm::vec3(1.f, 0.f, 0.f),	glm::vec2(1.f, 0.f),	glm::vec3(1.f, 0.f, 0.f),	// Vertex 6
		glm::vec3(2.f, 2.f, -2.f),		glm::vec3(0.f, 1.f, 0.f),	glm::vec2(1.f, 1.f),	glm::vec3(1.f, 0.f, 0.f),	// Vertex 7
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

/* --------------------------------------  Sphere CLASS  --------------------------------------------------------- */

/*	----------------------------------------------------------
*	Default class constructor
*	Sphere initialization based on: http://www.songho.ca/opengl/gl_sphere.html
*	Parameters: float radius - sphere darius
*				int sectorCount - number of sphere sectors 
*				int stackCount - number of sphere stacks
*/
Sphere::Sphere(float radius, int sectorCount, int stackCount)
{
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;

	float x, y, z, xy;							// vertex position
	float nx, ny, nz, lengthInv = 1.f / radius;	// vertex normal
	float s, t;

	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	for( int i=0; i <= stackCount; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;
		xy = radius * cosf(stackAngle);
		z = radius * sinf(stackAngle);

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for( int j=0; j<=sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);			// r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);			// r * cos(u) * sin(u)
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);

			//vertex texcoord (s, t) range between [0, 1]
			s = (float)j / sectorCount;
			t = (float)i / stackCount;
			texCoords.push_back(s);
			texCoords.push_back(t);
		}
	}

	// Generate CCW index lis of sphere triangles
	std::vector<GLuint> indices;
	int k1, k2;

	for(int i = 0; i<stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);
		k2 = k1 + sectorCount + 1;

		for( int j = 0; j<sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if( i != 0 )
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1+1);
			}

			// k1+1 => k2 => k2+1
			if( i != stackCount-1 )
			{
				indices.push_back(k1+1);
				indices.push_back(k2);
				indices.push_back(k2+1);
			}
		}
	}

	std::vector<Vertex> vertex_arr;
	for( int i = 0; i<vertices.size()/3; i++)
	{
		Vertex temp = { glm::vec3(vertices[3*i], vertices[3*i+1], vertices[3*i+2]),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec2(texCoords[2*i], texCoords[2*i+1]),
			glm::vec3(normals[3*i], normals[3*i+1], normals[3*i+2])
		};

		vertex_arr.push_back(temp);
	}

	// Free Memory
	std::vector<float>().swap(vertices);
	std::vector<float>().swap(texCoords);
	std::vector<float>().swap(normals);

	this->set(vertex_arr.data(), vertex_arr.size(), indices.data(), indices.size());
}

/*	----------------------------------------------------------
*	Default class destructor
*	Parameters: none
*	Used to: delete previously created class instance
*/
Sphere::~Sphere()
{
	
}