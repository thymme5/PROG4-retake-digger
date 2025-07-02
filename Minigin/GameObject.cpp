#include "GameObject.h"
#include "Transform.h"

using namespace dae;

GameObject::GameObject()
    : m_transform{ this }
    , m_Parent{ nullptr }
{
}

GameObject::~GameObject()
{
    m_Children.clear();
    m_Components.clear();
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
    m_transform.SetLocalPosition(x, y, 0.0f);

    // The cycle of neglect continues (make children dirty)
    for (auto* child : m_Children)
    {
        if (child)
            child->m_transform.SetDirty();
    }
}

glm::vec3 GameObject::GetWorldPosition()
{
    return m_transform.GetWorldPosition();
}

void GameObject::UpdateWorldPosition()
{
    m_transform.UpdateWorldPosition();
}

Transform& GameObject::GetTransform()
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

GameObject* GameObject::GetChildAt(int index)
{
    if (index < 0 || index >= static_cast<int>(m_Children.size()))
        return nullptr;

    return m_Children[index];
}

void GameObject::SetParent(GameObject* parent)
{
    // prevent setting self as parent
    if (parent == this)
        return;

    // prevent circular parenting (don't allow setting a child as your parent)
    GameObject* ancestor = parent;
    while (ancestor)
    {
        if (ancestor == this)
            return;
        ancestor = ancestor->m_Parent;
    }

    // remove from current parent
    if (m_Parent)
    {
        auto& siblings = m_Parent->m_Children;
        siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
    }

    glm::vec3 worldPosBefore = m_transform.GetWorldPosition();

    m_Parent = parent;

    if (m_Parent)
    {
        m_Parent->m_Children.push_back(this);
        glm::vec3 parentWorldPos = m_Parent->GetWorldPosition();
        m_transform.SetLocalPosition(worldPosBefore - parentWorldPos);
    }
    else
    {
        m_transform.SetLocalPosition(worldPosBefore);
    }

    m_transform.SetDirty();

    // Domino effect of disappointment begins here (make children dirty)
    for (auto* child : m_Children)
    {
        if (child)
            child->m_transform.SetDirty();
    }
}
