#include "GameObject.h"

using namespace dae;

GameObject::GameObject() = default;

GameObject::~GameObject()
{
	if (m_Parent)
	{
		auto& siblings = m_Parent->m_Children;
		siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
	}

	for (auto* child : m_Children)
		child->m_Parent = nullptr;

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

void GameObject::SetLocalPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.f);
	MarkDirty();
}

const glm::vec3& GameObject::GetLocalPosition() const
{
	return m_Transform.GetLocalPosition();
}

glm::vec3 GameObject::GetWorldPosition()
{
	if (m_IsDirty)
	{
		if (m_Parent)
		{
			glm::vec3 worldPos = m_Parent->GetWorldPosition() + m_Transform.GetLocalPosition();
			m_Transform.SetWorldPosition(worldPos);
		}
		else
		{
			m_Transform.SetWorldPosition(m_Transform.GetLocalPosition());
		}

		m_IsDirty = false;
	}
	return m_Transform.GetWorldPosition();
}

Transform& GameObject::GetTransform()
{
	return m_Transform;
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

void GameObject::SetParent(GameObject* newParent, bool keepWorldPosition)
{
	if (newParent == this || m_Parent == newParent)
		return;

	// Prevent circular parenting
	GameObject* ancestor = newParent;
	while (ancestor)
	{
		if (ancestor == this)
			return;
		ancestor = ancestor->m_Parent;
	}

	Reparent(newParent, keepWorldPosition);
}

void GameObject::Reparent(GameObject* newParent, bool keepWorldPosition)
{
	glm::vec3 worldPos = GetWorldPosition();

	// Remove from old parent
	if (m_Parent)
	{
		auto& siblings = m_Parent->m_Children;
		siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
	}

	m_Parent = newParent;

	if (m_Parent)
		m_Parent->m_Children.push_back(this);

	if (keepWorldPosition)
	{
		if (m_Parent)
		{
			glm::vec3 parentWorld = m_Parent->GetWorldPosition();
			m_Transform.SetPosition(worldPos - parentWorld);
		}
		else
		{
			m_Transform.SetPosition(worldPos);
		}
	}

	MarkDirty();
}

void GameObject::MarkDirty()
{
	m_IsDirty = true;

	for (auto* child : m_Children)
	{
		if (child)
			child->MarkDirty();
	}
}
