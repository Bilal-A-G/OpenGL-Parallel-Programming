#pragma once
#include "Event.h"

namespace TESLA
{
    class KeyEvent : public Event
    {
    public:
        KeyEvent(const int& keycode, const bool& pressed, const bool repeat) 
            : m_keycode(keycode), m_pressed(pressed), m_repeat(repeat){}
        int GetKeycode()
        {
            return m_keycode;
        }

        EventType GetType() override
        {
            if(m_repeat)
            {
                return TESLA::EventType::ButtonHeld;
            }
            return m_pressed ? TESLA::EventType::ButtonPressed : TESLA::EventType::ButtonReleased;
        }
    protected:
        int m_keycode;
        bool m_pressed;
        bool m_repeat;
    };

    class KeyboardButtonEvent : public KeyEvent
    {
    public:
        KeyboardButtonEvent(const int& keycode, const bool& pressed, const bool repeat)
            :KeyEvent(keycode, pressed, repeat){}
        
        EventCategory GetCategory() override
        {
            return TESLA::EventCategory::Keyboard;
        }
    };

    class MouseButtonEvent : public KeyEvent
    {
    public:
        MouseButtonEvent(const int& keycode, const bool& pressed, const bool repeat)
            :KeyEvent(keycode, pressed, repeat){}

        EventCategory GetCategory() override
        {
            return TESLA::EventCategory::Mouse;
        }
    };
}
