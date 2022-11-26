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


TESLA::Model* ImportModel(const char* fileName)
{
    TESLA::Shader vertexShader(TESLA::ShaderType::Vertex);
    TESLA::Shader fragmentShader(TESLA::ShaderType::Fragment);
    TESLA::Texture albedo(TESLA::TextureType::Albedo, TESLA::TextureExtension::Jpg);

    uint32_t shaderProgram = TESLA::Shader::CompileProgram({vertexShader, fragmentShader});

    return new TESLA::Model{fileName, shaderProgram, albedo.GetGLTexture(), view, projection};
}

std::vector<TESLA::Model*> sceneObjects;
bool rotate;
bool orthographic;
bool trace;

void Init()
{
    TESLA::Application::Start(windowWidth, windowHeight, "PBF Demo");

    TESLA::Texture goldBricks(TESLA::TextureType::Albedo, TESLA::TextureExtension::Jpg, "GoldBrick");
    
    TESLA::Model* sphere = ImportModel("Sphere");
    sphere->SetColour(glm::vec3(0.4, 0.1, 0.3));
    
    sceneObjects.push_back(new TESLA::Model(*sphere));
    sceneObjects[0]->Translate(glm::vec3(2, 0, 2));
    sceneObjects[0]->SetTexture(goldBricks);

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
        if(rotate)
        {
            model->Rotate(angle, glm::vec3(0, 1, 0));
            angle++;
        }
        if(trace)
        {
            model->SetLightColour({0.6, 0.1, 0.1});
            float xValue = glm::sin(moveTime) / 10;
            float zValue = glm::cos(moveTime) / 10;
            
            lightPosition = {xValue + lightPosition.x, 5.0f, zValue + lightPosition.z};
            moveTime += 0.005;
        }
        else
        {
            model->SetLightColour(glm::vec3(1.0f, 1.0f, 1.0f));
        }
        
        model->Draw(Camera::cameraPosition, lightPosition);
    }

    if(orthographic)
    {
        projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, nearPlane, farPlane);
    }
    else
    {
        projection = glm::perspective(glm::radians(70.0f), static_cast<float>(windowWidth)/static_cast<float>(windowHeight), nearPlane, farPlane);
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