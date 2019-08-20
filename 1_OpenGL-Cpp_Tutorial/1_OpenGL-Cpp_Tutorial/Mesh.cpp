#include "Mesh.h"

Mesh::Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices)
{
	this->initVertexData(vertexArray, nrOfVertices, indexArray, nrOfIndices);
	this->initVAO();
	this->initModelMatrix();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}

void Mesh::initVertexData(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices)
{
	for( int i = 0; i< nrOfVertices; i++)
	{
		this->vertices.push_back(vertexArray[i]);
	}

	for( int i = 0; i< nrOfIndices; i++)
	{
		this->indices.push_back(indexArray[i]);
	}
}

void Mesh::initVAO()
{
	/* VAO, VBO, EBO */
	/* GEN VAO AND BIND
	 * VAO (Vertex Array Object) - big box to hold all model (all triangle)
	 */
	glCreateVertexArrays(1, &this->VAO);
	glBindVertexArray( VAO );	// Activate the box! Anything we'll do to the others buffers it will bind to this VAO. 

	/* GEN VBO AND BIND AND SEND DATA
	 * VBO (Vertex Buffer Object) - is gonna send the vertex data of object.
	 */
	glGenBuffers(1, &this->VBO);
		// Make sure that VBO is put in the specific place inside VAO box. Bc can be more than one buffers inside VAO.
	glBindBuffer( GL_ARRAY_BUFFER, this->VBO );	// Bind VBO as ARRAY_BUFFER inside VAO box. 
		// Data we'll send to the graphics card. Target is ARRAY_BUFFER, send all vertices (size and pointer to data), STATIC_DRAW (as we'll not change vertices often)
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size()*sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);

	/* GEN EBO AND BIND AND SEND DATA
	 * EBO (Element Buffer Object) - is gonna send the indices data of object.
	 */
	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO );	// Bind Element Array Buffer (EBO) to specific place inside VAO box.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size()*sizeof(GLuint), this->indices.data(), GL_STATIC_DRAW);	// Determine indices data to be sent to graphics card.

	/* SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
	 * Setting specific vertex attributes (position, color and texture coordinate) to determine in what order they are set inside memory.
	 */
	// Position Attribute	// Get position (location) of that attribute inside core_program.
	// 0 - is location inside vertex shader. So we assume to let graphics card to know order of position coordinates inside big table of vertexes.
	// 3 - is size of single data pack used by location 0 (inside vertex shader). 3 floats.
	// GL_FLOAT - data type, 
	// stride = sizeof(Vertex) - how many bytes to move forwart on table to get into next vertex position
	// offsetof - to store offset from first vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);	// Need to be same number (location) as inside vertex shader (used by core_program).

		// Color Attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);	// Need to be same number (location) as inside vertex shader (used by core_program).

		// Texture Coordinates Attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);	// Need to be same number (location) as inside vertex shader (used by core_program).

		// Normal Vectors Attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(3);

	/* BIND VAO 0 (unbind previously active one)
	 * Unbind previously active VAO. To make sure no VAO is active now.
	 */
	glBindVertexArray( 0 );
}

void Mesh::initModelMatrix()
{
	this->position = glm::vec3(0.f);
	this->rotation = glm::vec3(0.f);
	this->scale = glm::vec3(1.f);
	this->ModelMatrix = glm::mat4(1.f);	//Create identity matrix

	// Initialize all three modifications with zero values.
	this->ModelMatrix = glm::translate(this->ModelMatrix, this->position);	// vec3 - translation vector
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));		// Choose angle of rotation and then rotation axis. (X)
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));		// Choose angle of rotation and then rotation axis. (Y)
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));		// Choose angle of rotation and then rotation axis. (Z)
	this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);	// vec3 - scale vector ( 1 means = no scaling )

}

void Mesh::updateUniforms(Shader* shader)
{
	shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
}

void Mesh::update()
{
}

void Mesh::render(Shader* shader)
{
	this->updateUniforms(shader);

	// Bind Vertex Array Object (VAO) to the selected program (shaders).
	glBindVertexArray( this->VAO );

	shader->use();

	// RENDER
	// Draw triangles. made of nrOfIndices which are unsigned int, starting from 0 index.
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);	
}
