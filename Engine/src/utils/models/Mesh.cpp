#include "TSpch.h"
#include "Mesh.h"
#include "glad/glad.h"

namespace TESLA
{
    void Mesh::SetupGLObjects()
    {
        GLuint vao;
        GLuint vbo;
        GLuint ebo;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        glGenBuffers(1, &m_positionsBuffer);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_positionsBuffer);
        glBufferData(GL_ARRAY_BUFFER, m_instancedModels.size() * sizeof(glm::mat4), m_instancedModels.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(float) * 3));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(float) * 6));

        glBindBuffer(GL_ARRAY_BUFFER, m_positionsBuffer);
        
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), nullptr);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(sizeof(glm::vec4)));
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(sizeof(glm::vec4) * 2));
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(sizeof(glm::vec4) * 3));
            
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);
        glEnableVertexAttribArray(6);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        TS_LOG_MESSAGE(TESLA_LOGGER::INFO, "Sent to buffer: vertices = {0}, indices = {1}", m_vertices.size(), m_indices.size());
        
        m_vao = vao;
        glBindVertexArray(0);

        m_modelLocation = glGetUniformLocation(m_shaderProgram, "model");
        m_viewLocation = glGetUniformLocation(m_shaderProgram, "view");
        m_colourLocation = glGetUniformLocation(m_shaderProgram, "colour");
        m_lightColourLocation = glGetUniformLocation(m_shaderProgram, "lightColour");

        m_projectionLocation = glGetUniformLocation(m_shaderProgram, "projection");
        m_viewerPositionLocation = glGetUniformLocation(m_shaderProgram, "viewerPosition");
        m_lightLocation = glGetUniformLocation(m_shaderProgram, "lightPosition");
    }
    
    GLuint prevShaderProgram;
    GLuint prevVAO;

    void Mesh::Draw(glm::vec3 cameraPos, glm::vec3 lightPos)
    {
        if(prevShaderProgram == 0)
        {
            glUseProgram(m_shaderProgram);
            prevShaderProgram = m_shaderProgram;
        }
        
        UpdateMVPMatrix(cameraPos, lightPos);

        if(prevVAO == 0)
        {
            glBindVertexArray(m_vao);
            prevVAO = m_vao;
        }
        
        if(m_instanceCount == 1)
        {
            glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
        }
        else if(m_instanceCount > 1)
        {
            glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr, m_instanceCount);
        }
    }

    void Mesh::Rotate(float angle, glm::vec3 upVector)
    {
        m_rotationMatrix = glm::rotate(IDENTITY_MAT, glm::radians(angle), upVector);
    }

    void Mesh::Scale(glm::vec3 scale)
    {
        m_scaleMatrix = glm::scale(IDENTITY_MAT, scale);
    }

    void Mesh::Translate(glm::vec3 translation)
    {
        m_positionMatrix = glm::translate(IDENTITY_MAT, translation);
        position = translation;
    }
    
    void Mesh::UpdateMVPMatrix(glm::vec3 cameraPos, glm::vec3 lightPos)
    {
        glUniformMatrix4fv(m_modelLocation, 1, GL_FALSE, &(m_positionMatrix * m_scaleMatrix * m_rotationMatrix)[0][0]);
        glUniformMatrix4fv(m_viewLocation, 1, GL_FALSE, &m_viewMatrix[0][0]);

        glUniformMatrix4fv(m_projectionLocation, 1, GL_FALSE, &m_projectionMatrix[0][0]);
        glUniform3f(m_colourLocation, m_colour.x, m_colour.y, m_colour.z);
        
        // glUniform3f(m_viewerPositionLocation, cameraPos.x, cameraPos.y, cameraPos.z);
        // glUniform3f(m_lightLocation, lightPos.x, lightPos.y, lightPos.z);
        // glUniform3f(m_lightColourLocation, m_lightColour.x, m_lightColour.y, m_lightColour.z);
    }

    void Mesh::UpdateInstancePositions(std::vector<glm::mat4> newModels)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_positionsBuffer);
        glBufferData(GL_ARRAY_BUFFER, m_instancedModels.size() * sizeof(glm::mat4), newModels.data(), GL_DYNAMIC_DRAW);
    }
}





