#include "Te_S_La.h"
#include "Camera.h"
#include "utils/events/KeyEvents.h"
#include "GLFW/glfw3.h"
#include "utils/physics/Physics.h"

int windowHeight = 800;
int windowWidth = 1200;

float nearPlane = 0.1f;
float farPlane = 200.0f;

glm::mat4 projection = glm::perspective(glm::radians(70.0f), static_cast<float>(windowWidth)/static_cast<float>(windowHeight), nearPlane, farPlane);
glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.6f), glm::vec3(0), glm::vec3(0, 1, 0));
std::vector<glm::vec4> instancePositions;

TESLA::Model* ImportModel(const char* fileName, int instanceCount)
{
    TESLA::Shader vertexShader(TESLA::ShaderType::Vertex);
    TESLA::Shader fragmentShader(TESLA::ShaderType::Fragment);
    
    TESLA::Texture albedo(TESLA::TextureType::Albedo, TESLA::TextureExtension::Jpg);

    uint32_t shaderProgram = TESLA::Shader::CompileProgram({vertexShader, fragmentShader});
    for(int i = 0; i < instanceCount; i++)
    {
        instancePositions.push_back(glm::vec4(rand() % 100 - 1, rand() % 100 - 1, rand() % 100 - 1, 0));
    }

    return new TESLA::Model{fileName, shaderProgram, albedo.GetGLTexture(), view, projection, instanceCount, instancePositions};
}

TESLA::Model* particleInstance;

int workGroupWidth = 256;
int workGroupHeight = 256;

void Init()
{
    TESLA::Application::Start(windowWidth, windowHeight, "PBF Demo");

    TS_LOG_MESSAGE(TESLA_LOGGER::INFO, "OpenGL Renderer {0}", glGetString(GL_RENDERER));
    
    TESLA::Model* sphere = ImportModel("Sphere", 65536);
    sphere->Scale(glm::vec3(0.1, 0.1, 0.1));

    TESLA::Shader computeShader(TESLA::ShaderType::Compute, "compute");
    TESLA::Texture computeBuffer(TESLA::TextureType::Compute, workGroupWidth, workGroupHeight);
    uint32_t computeShaderProgram = TESLA::Shader::CompileProgram({computeShader});
    
    TESLA::Physics::Init(computeShaderProgram, computeBuffer, workGroupWidth, workGroupHeight, instancePositions);
    
    particleInstance = new TESLA::Model(*sphere);

    TESLA::EventListener::Subscribe({[](TESLA::Event* event)
    {
        auto castedEvent = dynamic_cast<TESLA::KeyboardButtonEvent*>(event);
        if(castedEvent->GetKeycode() == GLFW_KEY_M)
        {
            TESLA::ExitApplication();
        }
    },TESLA::EventType::ButtonPressed, TESLA::EventCategory::Keyboard
    });

    Camera::Init();
}

float angle;
float moveTime;
glm::vec3 lightPosition = glm::vec3(-1, 5, 0);

double timeLastFrame;

void Render()
{
    double timeThisFrame = glfwGetTime();

    particleInstance->UpdateInstancePositions(instancePositions);
    particleInstance->Draw(Camera::cameraPosition, lightPosition);

    TESLA::Physics::ComputePhysics();
    instancePositions = TESLA::Physics::GetPositionData();
        
    glm::mat4 newView = Camera::CalculateView();
    if(newView != glm::mat4(1))
    {
        view = newView;
    }

    TS_LOG_MESSAGE(TESLA_LOGGER::TRACE, "Current FPS: {0}", 1/(timeThisFrame - timeLastFrame));
    timeLastFrame = timeThisFrame;
}

void CleanUp()
{
    TS_LOG_MESSAGE(TESLA_LOGGER::DEBUG, "Application ended");
    delete(particleInstance);
}