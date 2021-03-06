#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "ShadowMapFBO.h"


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
    Model(glm::vec3 position, 
		Material* material, 
		Texture* ovTexDiff,
		Texture* ovTexSpec,
		std::vector<Mesh> meshes );
	~Model();

	/* FUNCTIONS */
	void update();
	void render(Shader* shader, GLenum mode, ShadowMapFBO* shadowMapFBO = nullptr);
	void rotate(const glm::vec3 rotation);
	void move();
	void switch_dxdydz( Mesh* mesh, Mesh* mesh_two);
    void setScale(glm::vec3 vec)
    {
        for (Mesh* mesh: this->meshes)
        {
            mesh->setScale(vec);
        }
    }

    void moveMeshes(glm::vec3 vec)
    {
        for (Mesh* mesh: this->meshes)
        {
            mesh->setPosition(vec);
        }
    }
};

