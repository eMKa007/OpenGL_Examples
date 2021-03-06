#include "Material.h"

/*	----------------------------------------------------------
*	Class constructor
*	Parameters: glm::vec3 ambient - vector3 containing ambient lighting values
*				glm::vec3 diffuse - vector3 containing diffuse lighting values
*				glm::vec3 specular - vector3 containing specular lighting values
*				GLint diffuseTex - address of texture unit containing diffuse texture 
*				GLint specularTex- address of texture unit containing specular texture 
*/
Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLint diffuseTex, GLint specularTex)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->diffuseTex = diffuseTex;
	this->specularTex = specularTex;
}

/*	----------------------------------------------------------
*	Default class destructor
*	Parameters: none
*		Used to: delete previously created material instance
*/
Material::~Material(){}

/*	----------------------------------------------------------
*	Function name: sendToShader()
*	Parameters:	Shader& programID - reference to shader
*	Used to: Send variables to specific material structure fields inside given shader.
*	Return:	void
*/
void Material::sendToShader(Shader& programID)
{
	programID.setVec3f(this->ambient, "material.ambient");
	programID.setVec3f(this->diffuse, "material.diffuse");
	programID.setVec3f(this->specular, "material.specular");
	programID.set1i(this->diffuseTex, "material.diffuseTex");
	programID.set1i(this->specularTex, "material.specularTex");
}

