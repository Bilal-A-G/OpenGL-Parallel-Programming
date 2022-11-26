#pragma once
#include "Event.h"

namespace TESLA
{
    class WindowEvent : public Event
    {
    public:
        EventCategory GetCategory() override
        {
            return TESLA::EventCategory::Application;
        }
    };

    class WindowClosedEvent : public WindowEvent
    {
    public:
        EventType GetType() override
        {
            return TESLA::EventType::WindowClosed;
        }
    };

    class WindowFocusedEvent : public WindowEvent
    {
    public:
        EventType GetType() override
        {
            return TESLA::EventType::WindowFocused;
        }
    };

    class WindowLostFocusEvent : public WindowEvent
    {
    public:
        EventType GetType() override
        {
            return TESLA::EventType::WindowLostFocus;
        }
    };

    class WindowMovedEvent : public WindowEvent
    {
    public:
        EventType GetType() override
        {
            return TESLA::EventType::WindowMoved;
        }
    };

    class WindowResizedEvent : public WindowEvent
    {
    public:
        WindowResizedEvent(const unsigned int& windowHeight,const unsigned int& windowWidth)
            :m_windowHeight(windowHeight), m_windowWidth(windowWidth){}
        unsigned int GetWindowHeight()
        {
            return m_windowHeight;
        }
        unsigned int GetWindowWidth()
        {
            return m_windowWidth;
        }
    private:
        unsigned int m_windowHeight, m_windowWidth;
    };
}
