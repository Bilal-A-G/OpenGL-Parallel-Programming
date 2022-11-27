#include "TSpch.h"
#include "Mesh.h"
#include "glad/glad.h"

namespace TESLA
{
    //Handles the creation of a vao, vbo, and ebo as well as loading the appropriate data to each
//Once done, assigns the mesh's vao
void Mesh::SetupGLObjects()
{
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint instancedPositions;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenBuffers(1, &instancedPositions);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, instancedPositions);
    glBufferData(GL_ARRAY_BUFFER, m_instancedModels.size() * sizeof(glm::mat4), m_instancedModels.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(float) * 3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(float) * 6));

    if(m_instanceCount > 1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, instancedPositions);
        
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), nullptr);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(sizeof(glm::vec4)));
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(sizeof(glm::vec4) * 2));
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(sizeof(glm::vec4) * 3));
        
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
    }

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);
    
    TS_LOG_MESSAGE(TESLA_LOGGER::INFO, "Sent to buffer: vertices = {0}, indices = {1}", m_vertices.size(), m_indices.size());
    
    m_vao = vao;
    glBindVertexArray(0);
}

void Mesh::Draw(glm::vec3 cameraPos, glm::vec3 lightPos)
{
    glUseProgram(m_shaderProgram);
    
    UpdateMVPMatrix(cameraPos, lightPos);
    
    glBindVertexArray(m_vao);
    if(m_instanceCount == 1)
    {
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
    }
    else if(m_instanceCount > 1)
    {
        glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr, m_instanceCount);
    }
}

//Wrapper functions around various glm transform functions. Just provides better syntax for now, but in the future will contain more functionality 

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

//Updates the model view projection matrix uniform in the vertex shader
void Mesh::UpdateMVPMatrix(glm::vec3 cameraPos, glm::vec3 lightPos)
{
    const GLint modelLocation = glGetUniformLocation(m_shaderProgram, "model");
    const GLint viewLocation = glGetUniformLocation(m_shaderProgram, "view");
    const GLint colourLocation = glGetUniformLocation(m_shaderProgram, "colour");
    const GLint lightColourLocation = glGetUniformLocation(m_shaderProgram, "lightColour");

    const GLint projectionLocation = glGetUniformLocation(m_shaderProgram, "projection");
    const GLint viewerLocation = glGetUniformLocation(m_shaderProgram, "viewerPosition");
    const GLint lightLocation = glGetUniformLocation(m_shaderProgram, "lightPosition");
    
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &(m_positionMatrix * m_scaleMatrix * m_rotationMatrix)[0][0]);
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &m_viewMatrix[0][0]);
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &m_projectionMatrix[0][0]);
    
    glUniform3f(viewerLocation, cameraPos.x, cameraPos.y, cameraPos.z);
    glUniform3f(colourLocation, m_colour.x, m_colour.y, m_colour.z);
    glUniform3f(lightLocation, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(lightColourLocation, m_lightColour.x, m_lightColour.y, m_lightColour.z);
}
}





