#pragma once

namespace dae
{
	class GameObject;
	
	class Component
	{
	public:
		Component(GameObject& owner);
		virtual ~Component();

		virtual void Update() = 0;
		virtual void Render() const = 0;
		GameObject* GetOwner() const { return m_pOwner; };


	protected:
		GameObject* m_pOwner;
	};
}
