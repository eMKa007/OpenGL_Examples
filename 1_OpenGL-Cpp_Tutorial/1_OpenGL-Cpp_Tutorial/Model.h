#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"


class Model
{
private:
	Material* material;
	Texture* overrideTextureDiffuse;
	Texture* overrideTextureSpecular;
	std::vector<Mesh*> meshes;
	glm::vec3 position;

	/* PRIVATE FUNCTIONS */
	void updateUniforms();
		

public:
	Model(glm::vec3 position, 
		Material* material, 
		Texture* ovTexDiff,
		Texture* ovTexSpec,
		std::vector<Mesh*> meshes );
	~Model();

	/* FUNCTIONS */
	void update();
	void render(Shader* shader);
};

