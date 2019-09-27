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
	//this->position += glm::vec3(this->dx, this->dy, this->dz);

	for( auto & i : this->meshes )
	{
		//i->move(glm::vec3(this->dx, this->dy, this->dz));
		glm::vec3 meshPosition = i->getPosition();
		
		if( meshPosition.x >= 2.f-0.25f || meshPosition.x <= -2.f+0.25f )
			i->reverse_dx();

		if( meshPosition.y >= 2.f-0.25f || meshPosition.y <= -2.f+0.25f )
			i->reverse_dy();

		if( meshPosition.z >= 2.f-0.25f || meshPosition.z <= -2.f+0.25f )
			i->reverse_dz();

		i->move(i->getDeltaMove());
	}


}
