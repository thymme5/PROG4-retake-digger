#pragma once
#include <vector>
#include <algorithm>

namespace dae
{
    class Observer;
    class GameObject;
    enum class Event;

    class Subject
    {
    public:
        virtual ~Subject() = default;

        void AddObserver(Observer* observer);
        void RemoveObserver(Observer* observer);
        void Notify(Event event, GameObject* pGameObject);

    protected:
        std::vector<Observer*> m_Observers;
    };
}
