#pragma once


#include <iostream>

/* OpenGL Extension Wrangler */
#include <glew.h>

#include <glfw3.h>

/* Open Asset Import Library (assimp) */
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Model.h"

class AssimpLoader
{
public:
    AssimpLoader( std::string ModelPath, unsigned pFlags)
    : path(ModelPath), importer(new Assimp::Importer)
    {
        if( this->path.size() == 0 )
        {
            std::cout << "ERROR::ASSIMP::EMPTY_MODEL_PATH" << std::endl;
            glfwTerminate();
            exit(0);
        }

        this->scene = importer->ReadFile(this->path, pFlags);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
        {
            std::cout << "ERROR::ASSIMP::LOADING_MODEL" << std::endl;
            glfwTerminate();
            exit(0);
        }

        this->directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, this->scene);
    }

    ~AssimpLoader()
    {
        free(importer);
    }

    std::vector<Mesh*> GetMeshes() const { return this->meshes; }

private:
    Assimp::Importer* importer;
    std::string path;
    std::string directory;
    const aiScene* scene;
    std::vector<Mesh*> meshes;


    void processNode(aiNode* node, const aiScene* scene)
    {
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; 
            meshes.push_back( processMesh(mesh, scene));
        }
        // then do the same for each of its children
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            this->processNode(node->mChildren[i], scene);
        }
    }

    Mesh* processMesh( aiMesh* mesh, const aiScene* scene )
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for( unsigned int i = 0; i<mesh->mNumVertices; i++)
        {
            Vertex vertex = {glm::vec3(0.f), glm::vec3(0.5f), glm::vec2(0.f), glm::vec3(0.f)};
            glm::vec3 vector3;

            // process vertex position, normals and texture coords.
            vector3.x = mesh->mVertices[i].x;
            vector3.y = mesh->mVertices[i].y;
            vector3.z = mesh->mVertices[i].z;
            vertex.position = vector3;

            vector3.x = mesh->mNormals[i].x;
            vector3.y = mesh->mNormals[i].y;
            vector3.z = mesh->mNormals[i].z;
            vertex.normal = vector3;

            if( mesh->mTextureCoords[0] )
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.texcoord = vec;
            }

            vertices.push_back(vertex);
        }

        // Process Indices
        for( unsigned int i = 0; i< mesh->mNumFaces; i++ )
        {
            aiFace face = mesh->mFaces[i];
            for( unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        //Process Material
       /* if( mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            std::vector<Texture> specularMaps = loadMaterialTextures( material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }*/

        return new Mesh(vertices, indices, textures);
    }

    std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, const char* str)
    {
       /* std::vector<Texture> textures;
        for(unsigned int i = 0; i < material->GetTextureCount(type); i++)
        {
            aiString str;
            material->GetTexture(type, i, &str);
            Texture texture(str.C_Str(), );
        }*/
    }
};