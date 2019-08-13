#include "Material.h"

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLint diffuseTex, GLint specularTex)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->diffuseTex = diffuseTex;
	this->specularTex = specularTex;
}

Material::~Material(){}

void Material::sendToShader(Shader& programID)
{
	programID.setVec3f(this->ambient, "material.ambient");
	programID.setVec3f(this->diffuse, "material.diffuse");
	programID.setVec3f(this->specular, "material.specular");
	programID.set1i(this->diffuseTex, "material.diffuseTex");
	programID.set1i(this->specularTex, "material.specularTex");
}

