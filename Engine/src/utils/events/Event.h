#pragma once
#include "../../TSpch.h"

namespace TESLA
{
    enum EventCategory
    {
        Mouse,
        Keyboard,
        Application
    };

    enum EventType
    {
        ButtonPressed,
        ButtonReleased,
        ButtonHeld,
        MouseScrolled,
        MouseMoved,

        WindowClosed,
        WindowResized,
        WindowFocused,
        WindowLostFocus,
        WindowMoved
    };

    class Event
    {
    public:
        virtual EventCategory GetCategory() = 0;
        virtual EventType GetType() = 0;
        void Handle()
        {
            m_Handled = true;
        }
        bool GetHandled()
        {
            return m_Handled;
        }
    private:
        bool m_Handled = false;
    };

    using EventFunction = void (*)(TESLA::Event*);

    struct Subscriber
    {
        Subscriber(EventFunction function, const TESLA::EventType type, const TESLA::EventCategory category)
            :function(function), type(type), category(category){}
        
        EventFunction function;
        TESLA::EventType type;
        TESLA::EventCategory category;
    };
    
    class EventListener
    {
    public:
        EventListener() = delete;
        static void Invoke(Event* event);
        static void Subscribe(const Subscriber& subscriber);
        static void UnSubscribe(EventFunction function);
    private:
        static std::vector<Subscriber> m_subscribers;
    };
}
