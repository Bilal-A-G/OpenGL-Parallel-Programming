#include "TSpch.h"
#include "Event.h"

std::vector<TESLA::Subscriber> TESLA::EventListener::m_subscribers;

void TESLA::EventListener::Invoke(Event* event)
{
    for(int i = 0; i < m_subscribers.size(); i++)
    {
        Subscriber currentSubscriber = m_subscribers[i];
        
        if(currentSubscriber.type == event->GetType() && currentSubscriber.category == event->GetCategory())
        {
            currentSubscriber.function(event);
        }
    }
}

void TESLA::EventListener::UnSubscribe(EventFunction function)
{
    for (int i = 0; i < m_subscribers.size(); ++i)
    {
        if(m_subscribers[i].function == function)
        {
            m_subscribers.erase(m_subscribers.begin() + i);
        }
    }
}


void TESLA::EventListener::Subscribe(const Subscriber& subscriber)
{
    m_subscribers.push_back(subscriber);
}

