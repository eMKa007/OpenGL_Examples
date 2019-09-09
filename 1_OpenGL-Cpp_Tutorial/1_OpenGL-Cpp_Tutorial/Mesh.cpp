#include "Mesh.h"

/*	----------------------------------------------------------
*	Class constructor
*	Parameters: Vertex* vertexArray - pointer to array of Vertices
*				const unsigned nrOfVertices - number of vertices in given array
				GLuint* indices - pointer to array of indices
				const unsigned nrOfIndice - number of indices in given array 
				glm::vec3 position - vector3 containing mesh position (X/Y/Z)
				glm::vec3 rotation - vector3 containing mesh rotation (Yaw/Pitch/Roll)
				glm::vec3 scale - vector3 containing scale values (X/Y/Z)
*/
Mesh::Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices, 
			glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;

	this->initVAO(vertexArray, nrOfVertices, indexArray, nrOfIndices);	
	
	this->updateModelMatrix();
}

/*	----------------------------------------------------------
*	Class constructor
*	Parameters: Primitive* primitive - pointer to primitive class instance containing vertices and indices of mesh
				glm::vec3 position - vector3 containing mesh position (X/Y/Z)
				glm::vec3 rotation - vector3 containing mesh rotation (Yaw/Pitch/Roll)
				glm::vec3 scale - vector3 containing scale values (X/Y/Z)
*/
Mesh::Mesh(Primitive* primitive, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;

	this->initVAO(primitive);	
	
	this->updateModelMatrix();
}

/*	----------------------------------------------------------
*	Default class destructor
*	Parameters: none
*		Used to: delete previously created mesh instance
*/
Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);

	if( this->nrOfIndices > 0 )
		glDeleteBuffers(1, &this->EBO);
}

/*	----------------------------------------------------------
*	Function name: initVAO()
*	Parameters:	Vertex* vertexArray - pointer to array of Vertices
*				const unsigned& nrOfVertices - number of vertices in given array
				GLuint* indices - pointer to array of indices
				const unsigned& nrOfIndice - number of indices in given array
*	Used to: Initialize class variables and buffers ( VAO, VBO, EBO ) for given arrays.
*	Return:	void
*/
void Mesh::initVAO(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices)
{
	// Set Variables
	this->nrOfVertices = nrOfVertices;
	this->nrOfIndices = nrOfIndices;

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
	glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices*sizeof(Vertex), vertexArray, GL_STATIC_DRAW);

	/* GEN EBO AND BIND AND SEND DATA
	 * EBO (Element Buffer Object) - is gonna send the indices data of object.
	 */
	if( this->nrOfIndices > 0 )
	{
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO );	// Bind Element Array Buffer (EBO) to specific place inside VAO box.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices*sizeof(GLuint), indexArray, GL_STATIC_DRAW);	// Determine indices data to be sent to graphics card.
	}

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

/*	----------------------------------------------------------
*	Function name: initVAO()
*	Parameters:	Primitive* primitive - pointer to primitive class instance
*	Used to: Initialize class variables and buffers ( VAO, VBO, EBO ) for given primitive.
*	Return:	void
*/
void Mesh::initVAO(Primitive* primitive)
{
	// Set Variables
	this->nrOfVertices = primitive->getNrOfVertices();
	this->nrOfIndices = primitive->getNrOfIndices();

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
	glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices*sizeof(Vertex), primitive->getVertices(), GL_STATIC_DRAW);

	/* GEN EBO AND BIND AND SEND DATA
	 * EBO (Element Buffer Object) - is gonna send the indices data of object.
	 */
	if( this->nrOfIndices > 0 )
	{	
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO );	// Bind Element Array Buffer (EBO) to specific place inside VAO box.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices*sizeof(GLuint), primitive->getIndices(), GL_STATIC_DRAW);	// Determine indices data to be sent to graphics card.
	}

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

/*	----------------------------------------------------------
*	Function name: updateUniforms()
*	Parameters:	Shader* shader - pointer to created gpu program
*	Used to: Send to shader Model Matrix which is stored inside class instance.
*	Return:	void
*/
void Mesh::updateUniforms(Shader* shader)
{
	shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
}

/*	----------------------------------------------------------
*	Function name: updateModelMatrix()
*	Parameters:	none
*	Used to: Based on position, rotation and scale vectors new Model Matrix is calculated.
*	Return:	void
*/
void Mesh::updateModelMatrix()
{
	this->ModelMatrix = glm::mat4(1.f);

	this->ModelMatrix = glm::translate(this->ModelMatrix, this->position);	// vec3 - translation vector
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));		// Choose angle of rotation and then rotation axis. (X)
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));		// Choose angle of rotation and then rotation axis. (Y)
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));		// Choose angle of rotation and then rotation axis. (Z)
	this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);	// vec3 - scale vector ( 1 means = no scaling )
}

/*	----------------------------------------------------------
*	Function name: setPosition()
*	Parameters:	const glm::vec3& position - vector3 with coordinates of new position
*	Used to: Set new position of mesh.
*	Return:	void
*/
void Mesh::setPosition(const glm::vec3& position)
{
	this->position = position;
}

/*	----------------------------------------------------------
*	Function name: setRotation()
*	Parameters:	const glm::vec3& rotation - vector3 with new yaw/pitch/roll values
*	Used to: Set new rotation of mesh.
*	Return:	void
*/
void Mesh::setRotation(const glm::vec3& rotation)
{
	this->rotation = rotation;
}

/*	----------------------------------------------------------
*	Function name: scale()
*	Parameters:	const glm::vec3& scale - vector3 with new scale values
*	Used to: Set new scale factor of mesh.
*	Return:	void
*/
void Mesh::setScale(const glm::vec3& scale)
{
	this->scale = scale;
}

/*	----------------------------------------------------------
*	Function name: move()
*	Parameters:	const glm::vec3& position - vector3 with coordinates to be added to current position
*	Used to: Move current mesh position by given vector.
*	Return:	void
*/
void Mesh::move(const glm::vec3& position)
{
	this->position += position;
}

/*	----------------------------------------------------------
*	Function name: rotate()
*	Parameters:	const glm::vec3& rotation - vector3 values to be added to current mesh rotation
*	Used to: Rotate mesh by given rotation values
*	Return:	void
*/
void Mesh::rotate(const glm::vec3& rotation)
{
	this->rotation += rotation;
}

/*	----------------------------------------------------------
*	Function name: scaleUp()
*	Parameters:	const glm::vec3& scale - vector3 with with scale values to be added/subtracted to current mesh scale
*	Used to: Add/Subtract scale values by given vector.
*	Return:	void
*/
void Mesh::scaleUp(const glm::vec3& scale)
{
	this->scale += scale;
}

/*	----------------------------------------------------------
*	Function name: update()
*	Parameters:	none
*	Used to: 
*	Return:	void
*/
void Mesh::update(){ }

/*	----------------------------------------------------------
*	Function name: render()
*	Parameters:	Shader* shader - pointer to currently in use program
*	Used to: Draw mesh with given shader.
*	Return:	void
*/
void Mesh::render(Shader* shader)
{
	this->updateModelMatrix();
	this->updateUniforms(shader);

	// Bind Vertex Array Object (VAO) to the selected program (shaders).
	glBindVertexArray( this->VAO );

	shader->use();

	// RENDER
	if (this->nrOfIndices == 0 )
		glDrawArrays(GL_TRIANGLES, 0, this->nrOfVertices);
	else
		// Draw triangles. made of nrOfIndices which are unsigned int, starting from 0 index.
		glDrawElements(GL_TRIANGLES, this->nrOfIndices, GL_UNSIGNED_INT, 0);	
}
