#include "GameObject.h"

using namespace dae;

GameObject::GameObject()
    : m_transform{}
    , m_Parent{ nullptr }
    , m_positionIsDirty{ true }
    , m_LocalPosition{ 0.f }
    , m_WorldPosition{ 0.f }
{
}

GameObject::~GameObject()
{
    m_Children.clear();
}

void GameObject::Update()
{
    for (const auto& component : m_Components)
        component->Update();

    for (auto* child : m_Children)
        child->Update();
}

void GameObject::Render() const
{
    for (const auto& component : m_Components)
        component->Render();

    for (auto* child : m_Children)
        child->Render();
}

void GameObject::RenderImGui()
{
    for (const auto& component : m_Components)
    {
        if (auto* imguiComp = dynamic_cast<ImGuiComponent*>(component.get()))
            imguiComp->ImGuiRender();
    }

    for (auto* child : m_Children)
        child->RenderImGui();
}

void GameObject::SetPosition(float x, float y)
{
    m_transform.SetPosition(x, y, 0.0f);
    m_LocalPosition = glm::vec3(x, y, 0.f);
    m_positionIsDirty = true;

    for (auto* child : m_Children)
        child->m_positionIsDirty = true;
}

Transform GameObject::GetTransform() const
{
    return m_transform;
}

GameObject* GameObject::GetParent() const noexcept
{
    return m_Parent;
}

int GameObject::GetChildCount() const noexcept
{
    return static_cast<int>(m_Children.size());
}

void GameObject::SetParent(GameObject* parent)
{
    if (parent == this || std::find(m_Children.begin(), m_Children.end(), parent) != m_Children.end())
        return;

    if (m_Parent)
    {
        auto it = std::remove(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), this);
        m_Parent->m_Children.erase(it, m_Parent->m_Children.end());
    }

    glm::vec3 worldPos = GetWorldPosition();
    m_Parent = parent;

    if (m_Parent)
    {
        m_Parent->m_Children.push_back(this);
        glm::vec3 parentWorldPos = m_Parent->GetWorldPosition();
        m_LocalPosition = worldPos - parentWorldPos;
    }
    else
    {
        m_LocalPosition = worldPos;
    }

    m_transform.SetPosition(m_LocalPosition.x, m_LocalPosition.y, m_LocalPosition.z);

    m_positionIsDirty = true;

    for (auto* child : m_Children)
        child->m_positionIsDirty = true;
}

GameObject* GameObject::GetChildAt(int index)
{
    if (index < 0 || index >= static_cast<int>(m_Children.size()))
        return nullptr;

    return m_Children[index];
}

glm::vec3 GameObject::GetWorldPosition()
{
    if (m_positionIsDirty)
        UpdateWorldPosition();

    return m_WorldPosition;
}

void GameObject::UpdateWorldPosition()
{
    if (!m_positionIsDirty)
        return;

    if (m_Parent)
    {
        m_Parent->UpdateWorldPosition();
        m_WorldPosition = m_Parent->m_WorldPosition + m_LocalPosition;
    }
    else
    {
        m_WorldPosition = m_LocalPosition;
    }

    m_positionIsDirty = false;

    for (auto* child : m_Children)
    {
        child->UpdateWorldPosition();
    }
}

void GameObject::AddObserver(Observer* pObserver)
{
    m_Observers.push_back(pObserver);
}

void GameObject::RemoveObserver(Observer* pObserver)
{
    m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), pObserver), m_Observers.end());
}

void GameObject::NotifyObservers(Event event)
{
    for (auto* observer : m_Observers)
        observer->OnNotify(event, this);
}
