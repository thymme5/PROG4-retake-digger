#include "Subject.h"
#include "Observer.h"

void dae::Subject::AddObserver(Observer* observer)
{
    m_Observers.push_back(observer);
}

void dae::Subject::RemoveObserver(Observer* observer)
{
    m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
}

void dae::Subject::Notify(Event event, GameObject* pGameObject)
{
    for (auto* observer : m_Observers)
    {
        if (observer)
            observer->OnNotify(event, pGameObject);
    }
}
