#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include "../../TSpch.h"
#include "../rendering/Texture.h"

namespace TESLA
{

#define IDENTITY_MAT glm::mat4(1.0f)
    
    struct Vertex
    {
        std::array<float, 3> position;
        std::array<float, 3> normal;
        std::array<float, 2> uv;
    };

    class Mesh
    {
    public:
        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const uint32_t& shaderProgram, glm::mat4& viewMatrix, glm::mat4& projectionMatrix, glm::vec3 colour, uint32_t texture, int instanceCount, std::vector<glm::mat4> instancedModels)
            : m_viewMatrix(viewMatrix), m_projectionMatrix(projectionMatrix), m_instanceCount(instanceCount)
        {
            m_vertices = vertices;
            m_indices = indices;
            m_shaderProgram = shaderProgram;
            m_colour = colour;
            m_texture = texture;
            m_instancedModels = instancedModels;
            this->SetupGLObjects();
        }

        Mesh& operator=(const Mesh& mesh)
        {
            m_vao = mesh.m_vao;
            m_colour = mesh.m_colour;
            m_instanceCount = mesh.m_instanceCount;
            return *this;
        }
    
        void Draw(glm::vec3 cameraPos, glm::vec3 lightPos);
        void Translate(glm::vec3 translation);
        void Rotate(float angle, glm::vec3 upVector);
        void Scale(glm::vec3 scale);
        void SetColour(glm::vec3 colour){m_colour = colour;}
        void SetLightColour(glm::vec3 colour){m_lightColour = colour;}
        void SetTexture(TESLA::Texture texture){m_texture = texture.GetGLTexture();}
    
        const uint32_t GetShaderProgram()
        {
            return m_shaderProgram;
        }
    public:
        glm::vec3 position = glm::vec3(0);
    private:
        void SetupGLObjects();
        void UpdateMVPMatrix(glm::vec3 cameraPos, glm::vec3 lightPos);
    
        std::vector<Vertex> m_vertices;
        std::vector<glm::mat4> m_instancedModels;
        std::vector<unsigned int> m_indices;
        uint32_t m_shaderProgram;
        uint32_t m_texture;
        uint32_t m_vao;
        glm::vec3 m_colour;
        glm::vec3 m_lightColour;

        int m_instanceCount;

        glm::mat4 m_scaleMatrix = IDENTITY_MAT;
        glm::mat4 m_rotationMatrix = IDENTITY_MAT;
        glm::mat4 m_positionMatrix = IDENTITY_MAT;
    
        glm::mat4& m_viewMatrix;
        glm::mat4& m_projectionMatrix;
    };
}

