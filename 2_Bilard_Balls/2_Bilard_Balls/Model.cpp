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
	this->overrideTextureDiffuse = nullptr;
	this->overrideTextureSpecular = nullptr;

	if( ovTexDiff != nullptr )
		this->overrideTextureDiffuse = ovTexDiff;

	if( ovTexDiff != nullptr )
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

void Model::update()
{

}

void Model::render(Shader* shader, GLenum mode)
{
	// Update uniforms (variables send to gpu [shader] from cpu)- every change they're updated.
	this->updateUniforms();

	// Update Uniforms
	this->material->sendToShader(*shader);

		// Use a program (shader) - need to tell what shader we want to use.
	shader->use();

		// Activate Texture
	if( overrideTextureDiffuse != nullptr )
		overrideTextureDiffuse->bind(0);
	if( overrideTextureSpecular != nullptr )
		overrideTextureSpecular->bind(1);

		// Draw
	for( auto &i : this->meshes )
	{
		i->render(shader, mode);
	}
}

void Model::rotate(const glm::vec3 rotation)
{
	for( auto & i : this->meshes )
	{
		i->rotate(rotation);
	}
}

void Model::move()
{
	//if( this->position.x >= 2.f-0.25f || this->position.x <= -2.f+0.25f )
	//	this->dx = -this->dx;

	//if( position.y >= 2.f-0.25f || this->position.y <= -2.f+0.25f )
	//	this->dy = -this->dy;

	//if( position.z >= 2.f-0.25f || this->position.z <= -2.f+0.25f )
	//	this->dz = -this->dz;

	//this->position += glm::vec3(this->dx, this->dy, this->dz);

	for( auto & i : this->meshes )
	{
		//i->move(glm::vec3(this->dx, this->dy, this->dz));
	}
}
