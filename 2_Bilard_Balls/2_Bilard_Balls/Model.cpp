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

	for( auto & mesh : this->meshes )
	{
		//mesh->move(glm::vec3(this->dx, this->dy, this->dz));
		glm::vec3 meshPosition = mesh->getPosition();

		// Check for box limits
		if( meshPosition.x >= 2.f-0.2f && mesh->getDeltaMove().x > 0 )
			mesh->reverse_dx();
		else if( meshPosition.x <= -2.f+0.2f && mesh->getDeltaMove().x < 0 )
			mesh->reverse_dx();

		if( meshPosition.y >= 2.f-0.2f && mesh->getDeltaMove().y > 0 )
			mesh->reverse_dy();
		else if( meshPosition.y <= -2.f+0.2f && mesh->getDeltaMove().y < 0 )
			mesh->reverse_dy();

		if( meshPosition.z >= 2.f-0.2f && mesh->getDeltaMove().z > 0) 
			mesh->reverse_dz();
		else if( meshPosition.z <= -2.f+0.2f && mesh->getDeltaMove().z < 0 )
			mesh->reverse_dz();

		// Check distance between balls
		for( auto& mesh_two : this->meshes )
		{
			// Skip checking itself mesh
			if( mesh == mesh_two )
				continue;

			glm::vec3 secondMeshPos = mesh_two->getPosition();

			if( glm::distance( meshPosition, secondMeshPos) < 0.4f )
				switch_dxdydz( mesh, mesh_two );

		}

		mesh->move(mesh->getDeltaMove());
	}
}

void Model::switch_dxdydz(Mesh* mesh, Mesh* mesh_two)
{
	glm::vec3 meshTwo_dxdydz = mesh_two->getDeltaMove();

	mesh_two->set_dxdydz(mesh->getDeltaMove());
	mesh->set_dxdydz(meshTwo_dxdydz);
}
