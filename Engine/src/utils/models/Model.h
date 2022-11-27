#pragma once
#include "Mesh.h"
#include "assimp/scene.h"

namespace TESLA
{
    class Model
    {
    public:
        std::string name;
        Model(const Model& model)
            :m_viewMatrix(model.m_viewMatrix), m_projectionMatrix(model.m_projectionMatrix)
        {
            m_shaderProgram = model.m_shaderProgram;
            m_texture = model.m_texture;
            m_meshes = model.m_meshes;
            m_instancedModels = model.m_instancedModels;
        }
        
        Model(const char* fileName, const uint32_t& shaderProgram, uint32_t texture, glm::mat4& viewMatrix, glm::mat4& projectionMatrix, int instanceCount, std::vector<glm::mat4> instancedModels)
            : m_viewMatrix(viewMatrix), m_projectionMatrix(projectionMatrix)
        {
            m_shaderProgram = shaderProgram;
            m_texture = texture;
            m_instanceCount = instanceCount;
            m_instancedModels = instancedModels;

            LoadModel(fileName);
        }
    
        void Draw(glm::vec3 cameraPos, glm::vec3 lightPos);

        void Translate(glm::vec3 translation);
        void Rotate(float angle, glm::vec3 upVector);
        void Scale(glm::vec3 scale);
        void SetColour(glm::vec3 colour);
        void SetLightColour(glm::vec3 colour);
        void SetTexture(TESLA::Texture texture);
        void UpdateInstancePositions(std::vector<glm::mat4> newModels);
        glm::vec3 GetPosition(){return m_position;}
        const std::vector<Mesh>& GetMeshes(){return m_meshes;}
    
    private:
        void LoadModel(const char* fileName);
        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    
        std::vector<Mesh> m_meshes;

        uint32_t m_shaderProgram;
        uint32_t m_texture;
        int m_instanceCount;

        std::vector<glm::mat4> m_instancedModels;
        glm::mat4& m_viewMatrix;
        glm::mat4& m_projectionMatrix;
        glm::vec3 m_position;
    };
}
