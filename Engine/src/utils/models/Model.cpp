#include "TSpch.h"
#include "Model.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "../../core/Core.h"


namespace TESLA
{
    void Model::LoadModel(const char* fileName)
    {
        const std::string filePath = resourcePath + modelsDirectory + fileName + modelExtension;
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
        
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            TS_LOG_MESSAGE(TESLA_LOGGER::ERR, "Assimp failed to load mesh: {0}", importer.GetErrorString());
            return;
        }
        
        TS_LOG_MESSAGE(TESLA_LOGGER::INFO, "Successfully loaded mesh: {0}", fileName);

        ProcessNode(scene->mRootNode, scene);
    }

    void Model::Draw(glm::vec3 cameraPos, glm::vec3 lightPos)
    {
        for (int i = 0; i < m_meshes.size(); ++i)
        {
            m_meshes[i].Draw(cameraPos, lightPos);
        }
    }

    void Model::ProcessNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_meshes.push_back(ProcessMesh(mesh, scene));
        }

        TS_LOG_MESSAGE(TESLA_LOGGER::INFO, "Loaded: {0} meshes", m_meshes.size());

        for (unsigned int i = 0; i < node->mNumChildren; ++i)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
        {
            Vertex vertex;
            
            vertex.position = std::array<float, 3>{mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
            vertex.normal = std::array<float, 3>{mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};

            if(mesh->mTextureCoords[0])
            {
                vertex.uv = std::array<float, 2>{mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            }
            else
            {
                vertex.uv = std::array<float, 2>{0.0f, 0.0f};
            }

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
        {
            aiFace face = mesh->mFaces[i];
            
            for (unsigned int j = 0; j < face.mNumIndices; ++j)
                indices.push_back(face.mIndices[j]);
        }

        return Mesh(vertices, indices, m_shaderProgram, m_viewMatrix, m_projectionMatrix, glm::vec3(1, 1, 1), m_texture, m_instanceCount, m_instancedModels);
    }

    void Model::Scale(glm::vec3 scale)
    {
        for (int i = 0; i < m_meshes.size(); ++i)
        {
            m_meshes[i].Scale(scale);
        }
    }

    void Model::SetColour(glm::vec3 colour)
    {
        for (int i = 0; i < m_meshes.size(); i++)
        {
            m_meshes[i].SetColour(colour);
        }
    }

    void Model::SetTexture(TESLA::Texture texture)
    {
        for (int  i = 0; i < m_meshes.size(); i++)
        {
            m_meshes[i].SetTexture(texture);
        }
    }

        
    void Model::SetLightColour(glm::vec3 colour)
    {
        for (int i = 0 ; i < m_meshes.size(); i++)
        {
            m_meshes[i].SetLightColour(colour);
        }
    }


    void Model::Translate(glm::vec3 translation)
    {
        m_position = translation;
        for (int i = 0; i < m_meshes.size(); ++i)
        {
            m_meshes[i].Translate(translation);
        }
    }

    void Model::Rotate(float angle, glm::vec3 upVector)
    {
        for (int i = 0; i < m_meshes.size(); ++i)
        {
            m_meshes[i].Rotate(angle, upVector);
        }
    }

    void Model::UpdateInstancePositions(std::vector<glm::mat4> newModels)
    {
        for(int i = 0; i < m_meshes.size(); i++)
        {
            m_meshes[i].UpdateInstancePositions(newModels);
        }
    }

}






