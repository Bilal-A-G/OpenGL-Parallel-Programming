#include "Te_S_La.h"
#include "Camera.h"
#include "utils/events/KeyEvents.h"
#include "GLFW/glfw3.h"

int windowHeight = 800;
int windowWidth = 1200;

float nearPlane = 0.1f;
float farPlane = 200.0f;

glm::mat4 projection = glm::perspective(glm::radians(70.0f), static_cast<float>(windowWidth)/static_cast<float>(windowHeight), nearPlane, farPlane);
glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.6f), glm::vec3(0), glm::vec3(0, 1, 0));


TESLA::Model* ImportModel(const char* fileName, int instanceCount)
{
    TESLA::Shader vertexShader(TESLA::ShaderType::Vertex);
    TESLA::Shader fragmentShader(TESLA::ShaderType::Fragment);
    TESLA::Texture albedo(TESLA::TextureType::Albedo, TESLA::TextureExtension::Jpg);

    uint32_t shaderProgram = TESLA::Shader::CompileProgram({vertexShader, fragmentShader});
    std::vector<glm::mat4> modelMatrices;
    for(int i = 0; i < instanceCount; i++)
    {
        modelMatrices.push_back(glm::translate(glm::mat4(1), glm::vec3(rand() % 100 - 1, rand() % 100 - 1, rand() % 100 - 1)));
    }

    return new TESLA::Model{fileName, shaderProgram, albedo.GetGLTexture(), view, projection, instanceCount, modelMatrices};
}

std::vector<TESLA::Model*> sceneObjects;
bool rotate;
bool orthographic;
bool trace;

void Init()
{
    TESLA::Application::Start(windowWidth, windowHeight, "PBF Demo");
    
    TESLA::Model* sphere = ImportModel("Sphere", 100000);
    sphere->Scale(glm::vec3(0.3, 0.3, 0.3));
    
    sceneObjects.push_back(new TESLA::Model(*sphere));

    TESLA::EventListener::Subscribe({[](TESLA::Event* event)
    {
        auto castedEvent = dynamic_cast<TESLA::KeyboardButtonEvent*>(event);
        if(castedEvent->GetKeycode() == GLFW_KEY_O)
        {
            rotate = !rotate;
        }
        else if(castedEvent->GetKeycode() == GLFW_KEY_SPACE)
        {
            orthographic = !orthographic;
        }
        else if(castedEvent->GetKeycode() == GLFW_KEY_M)
        {
            TESLA::ExitApplication();
        }
        else if(castedEvent->GetKeycode() == GLFW_KEY_P)
        {
            trace = !trace;
        }
    },TESLA::EventType::ButtonPressed, TESLA::EventCategory::Keyboard
    });

    Camera::Init();
}

float angle;
float moveTime;
glm::vec3 lightPosition = glm::vec3(-1, 5, 0);

void Render()
{
    for (TESLA::Model* model : sceneObjects)
    {
        model->SetLightColour(glm::vec3(1.0f, 1.0f, 1.0f));
        model->Draw(Camera::cameraPosition, lightPosition);
    }
        
    glm::mat4 newView = Camera::CalculateView();
    if(newView != glm::mat4(1))
    {
        view = newView;
    }
}

void CleanUp()
{
    TS_LOG_MESSAGE(TESLA_LOGGER::DEBUG, "Application ended");
    sceneObjects.clear();
}