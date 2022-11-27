#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>
#include "core//Application.h"
#include "utils/events/Event.h"
#include "utils/events/KeyEvents.h"
#include "utils/events/MouseEvents.h"
#include "GLFW/glfw3.h"

glm::vec3 globalUpVector = glm::vec3(0, 1, 0);

float yaw, pitch;
bool focused;

float moveSpeed = 10;
float mouseSensitivity = 20;

float deltaTime;
float timeLastFrame;

glm::vec2 mousePosLastFrame;
glm::vec2 mouseDelta;

bool wDown,aDown,sDown,dDown,spaceDown,ctrlDown;

void CursorCallback(TESLA::Event* event)
{
    const auto castedEvent = dynamic_cast<TESLA::MouseMovedEvent*>(event);
    
    if(focused)
    {
        mousePosLastFrame = glm::vec2(castedEvent->GetMouseX(), castedEvent->GetMouseY());
        focused = false;
    }
    mouseDelta = glm::vec2(castedEvent->GetMouseX(), castedEvent->GetMouseY()) - mousePosLastFrame;

    yaw += mouseDelta.x * mouseSensitivity * deltaTime;
    pitch -= mouseDelta.y * mouseSensitivity * deltaTime;

    pitch = glm::clamp(pitch, -89.0f, 89.0f);
        
    mousePosLastFrame = glm::vec2(castedEvent->GetMouseX(), castedEvent->GetMouseY());
}

void MouseCallback(TESLA::Event* event)
{
    const auto castedEvent = dynamic_cast<TESLA::MouseButtonEvent*>(event);
    if(castedEvent->GetKeycode() != GLFW_MOUSE_BUTTON_RIGHT) return;

    if(castedEvent->GetType() == TESLA::ButtonReleased)
    {
        TESLA::EventListener::UnSubscribe(CursorCallback);
        TESLA::Application::SetInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        focused = true;
    }
    else if(castedEvent->GetType() == TESLA::ButtonPressed)
    {
        TESLA::EventListener::Subscribe({CursorCallback, TESLA::EventType::MouseMoved, TESLA::EventCategory::Mouse});
        TESLA::Application::SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void KeyCallback(TESLA::Event* event)
{
    const auto castedEvent = dynamic_cast<TESLA::KeyboardButtonEvent*>(event);
    const bool pressed = castedEvent->GetType() == TESLA::ButtonPressed || castedEvent->GetType() == TESLA::ButtonHeld;
    switch (castedEvent->GetKeycode())
    {
    case GLFW_KEY_W:
        wDown = pressed;
        break;
    case GLFW_KEY_S:
        sDown = pressed;
        break;
    case GLFW_KEY_A:
        aDown = pressed;
        break;
    case GLFW_KEY_D:
        dDown = pressed;
        break;
    case GLFW_KEY_SPACE:
        spaceDown = pressed;
        break;
    case GLFW_KEY_LEFT_CONTROL:
        ctrlDown = pressed;
        break;
    default:
        break;
    }
}

void Camera::Init()
{
    TESLA::EventListener::Subscribe({MouseCallback, TESLA::EventType::ButtonHeld, TESLA::EventCategory::Mouse});
    TESLA::EventListener::Subscribe({MouseCallback, TESLA::EventType::ButtonPressed, TESLA::EventCategory::Mouse});
    TESLA::EventListener::Subscribe({MouseCallback, TESLA::EventType::ButtonReleased, TESLA::EventCategory::Mouse});

    TESLA::EventListener::Subscribe({KeyCallback, TESLA::EventType::ButtonHeld, TESLA::EventCategory::Keyboard});
    TESLA::EventListener::Subscribe({KeyCallback, TESLA::EventType::ButtonPressed, TESLA::EventCategory::Keyboard});
    TESLA::EventListener::Subscribe({KeyCallback, TESLA::EventType::ButtonReleased, TESLA::EventCategory::Keyboard});

    focused = true;
}


glm::mat4 Camera::CalculateView()
{
    deltaTime = TESLA::Application::GetTime() - timeLastFrame;

    cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraDirection.y = sin(glm::radians(pitch));
    cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraDirection = glm::normalize(cameraDirection);

    const glm::vec3 cameraRight = glm::normalize(glm::cross(globalUpVector, cameraDirection));
    const glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));
    const float cameraSpeed = moveSpeed * deltaTime;

    if(wDown)
    {
        cameraPosition += cameraDirection * cameraSpeed;
    }
    if(sDown)
    {
        cameraPosition -= cameraDirection * cameraSpeed;
    }
    if(aDown)
    {
        cameraPosition += cameraRight * cameraSpeed;
    }
    if(dDown)
    {
        cameraPosition -= cameraRight * cameraSpeed;
    }
    if(spaceDown)
    {
        cameraPosition += cameraUp * cameraSpeed;
    }
    if(ctrlDown)
    {
        cameraPosition -= cameraUp * cameraSpeed;
    }

    timeLastFrame = TESLA::Application::GetTime();
    
    return glm::lookAt(cameraPosition, cameraPosition + cameraDirection, globalUpVector);
}

