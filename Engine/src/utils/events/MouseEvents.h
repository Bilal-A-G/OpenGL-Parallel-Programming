#pragma once
#include "Event.h"

namespace TESLA
{
    class MouseEvent : public Event
    {
    public:
        EventCategory GetCategory() override
        {
            return TESLA::EventCategory::Mouse;
        }
    };

    class MouseMovedEvent : public MouseEvent
    {
    public:
        MouseMovedEvent(const float& mouseX, const float& mouseY)
            :MouseEvent(), m_mouseX(mouseX), m_mouseY(mouseY){}
        EventType GetType() override
        {
            return TESLA::EventType::MouseMoved;
        }
        float GetMouseX()
        {
            return m_mouseX;
        }
        float GetMouseY()
        {
            return m_mouseY;
        }
    private:
        float m_mouseX, m_mouseY;
    };

    class MouseScrolledEvent : public MouseEvent
    {
    public:
        MouseScrolledEvent(float xOffset, float yOffset)
            :MouseEvent(), m_xOffset(xOffset), m_yOffset(yOffset){}
        EventType GetType() override
        {
            return TESLA::EventType::MouseScrolled;
        }
        float GetXOffset()
        {
            return m_xOffset;
        }
        float GetYOffset()
        {
            return m_yOffset;
        }
    
    private:
        float m_xOffset, m_yOffset;
    };
}
