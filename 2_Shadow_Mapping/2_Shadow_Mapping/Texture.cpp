#include "Texture.h"

/*	----------------------------------------------------------
*	Default class constructor
*	Parameters: const char* fileName - path to image source
*			GLenum type - type of desired texture
*/
Texture::Texture(const char* fileName, GLenum type)
{
	/* INIT TEXTURE
	 * Load an input image using SOIL library. Generate and bind texture object.
	 */
	this->type = type;

	unsigned char* image = SOIL_load_image( fileName, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

	glGenTextures(1, &this->id);	// Generate empty texture object. 

	glBindTexture(type, this->id);	// Bind texture to work on it. 

		//SET TEXTURE PARAMETERS (OPTIONS)
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Repeat texture if is enough space to do so. S- is tex coordinates (x coordinate). Same as U and V.
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);	// Y coordinate.
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);	 // Characteristic while magnifying (zooming). Anti-aliasing for texture.
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Characteristic while minimization. Similar as magnification. 

	if( image )	//Check for errors. If image is loaded correctly - create specific OpenGL texture object. Automatically associated with previously bound 'texture0' variable.
	{
		// Create OpenGL texture using image data stored inside 'image' variable.
		glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

		// Mipmap is basically same image in several different resolutions to use it depends of distance of viewing object.
		glGenerateMipmap(type);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED" << fileName << "\n";
	}

	
	glBindTexture(type, 0);	// Unbound any texture!!! Tidy up purpose. 
	SOIL_free_image_data(image);	// Image is loaded, not need to hold its raw data.
}

/*	----------------------------------------------------------
*	Default class destructor
*	Parameters: none
*	Used to: delete previously created program instance
*/
Texture::~Texture()
{
	glDeleteTextures(1, &this->id );
}

/*	----------------------------------------------------------
*	Function name: bind()
*	Parameters:	none
*	Used to: Bind texture to specified texture unit and type for further work.
*	Return:	void
*/
void Texture::bind(const GLint texture_unit)
{
	glActiveTexture(GL_TEXTURE0+texture_unit);	// Put created texture to selected texture unit.
	glBindTexture(this->type, this->id);		// Bind a texture object to that activated texture unit.
}

/*	----------------------------------------------------------
*	Function name: unbind()
*	Parameters:	none
*	Used to: Unbind texture from specified type.
*	Return:	void
*/
void Texture::unbind()
{
	
	glBindTexture(this->type,0);
}

/*	----------------------------------------------------------
*	Function name: loadFromFile()
*	Parameters:	const char* fileName- path to image source
*	Used to: Load new image file to already created texture class
*	Return:	void
*/
void Texture::loadFromFile(const char* fileName)
{
	if( this->id )
	{
		glDeleteTextures(1, &this->id);
	}

	/* INIT TEXTURE
	 * Load an input image using SOIL library. Generate and bind texture object.
	 */
	unsigned char* image = SOIL_load_image( fileName, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

	glGenTextures(1, &this->id);	// Generate empty texture object. 

	glBindTexture(type, this->id);	// Bind texture to work on it. 

		//SET TEXTURE PARAMETERS (OPTIONS)
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Repeat texture if is enough space to do so. S- is tex coordinates (x coordinate). Same as U and V.
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);	// Y coordinate.
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);	 // Characteristic while magnifying (zooming). Anti-aliasing for texture.
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Characteristic while minimization. Similar as magnification. 

	if( image )	//Check for errors. If image is loaded correctly - create specific OpenGL texture object. Automatically associated with previously bound 'texture0' variable.
	{
		// Create OpenGL texture using image data stored inside 'image' variable.
		glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

		// Mipmap is basically same image in several different resolutions to use it depends of distance of viewing object.
		glGenerateMipmap(type);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::LOAD_FROM_FILE::TEXTURE_LOADING_FAILED" << fileName << "\n";
	} 

	
	glBindTexture(type, 0);	// Unbound any texture!!! Tidy up purpose. 
	SOIL_free_image_data(image);	// Image is loaded, not need to hold its raw data.
}
