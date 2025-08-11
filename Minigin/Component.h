#pragma once

enum class RenderLayer
{
	Default = 0,
	Gameplay = 1,
	HUD = 2,
};


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

		RenderLayer GetRenderLayer() const { return m_Layer; }
		void SetRenderLayer(RenderLayer layer);
	protected:
		GameObject* m_pOwner;
		RenderLayer m_Layer;
	};
}
