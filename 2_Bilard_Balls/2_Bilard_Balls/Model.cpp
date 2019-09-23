#include "Model.h"

void Model::updateUniforms()
{

}

Model::Model(glm::vec3 position, 
		Material* material, 
		Texture* ovTexDiff,
		Texture* ovTexSpec,
		std::vector<Mesh*> meshes)
{
	this->position = position;
	this->material = material;
	this->overrideTextureDiffuse = ovTexDiff;
	this->overrideTextureSpecular = ovTexSpec;
	
	for( auto *i: meshes)
	{
		this->meshes.push_back(new Mesh(*i));
	}

	//Move every mesh of given offset
	for( auto &i : this->meshes)
	{
		i->move(this->position);
		i->setOrigin(this->position);
	}
}

Model::~Model()
{
	for( auto *&i: this->meshes)
	{
		delete i;
	}
}

void Model::render(Shader* shader)
{
	// Update uniforms (variables send to gpu [shader] from cpu)- every change they're updated.
	this->updateUniforms();

	// Update Uniforms
	this->material->sendToShader(*shader);

		// Use a program (shader) - need to tell what shader we want to use.
	shader->use();

		// Activate Texture
	overrideTextureDiffuse->bind(0);
	overrideTextureSpecular->bind(1);

		// Draw
	for( auto &i : this->meshes )
	{
		i->render(shader);
	}
}

void Model::rotate(const glm::vec3 rotation)
{
	for( auto & i : this->meshes )
	{
		i->rotate(rotation);
	}
}
