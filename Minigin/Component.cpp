#include "Component.h"
#include "GameObject.h"

dae::Component::Component(GameObject& owner):
	m_pOwner{ &owner }, m_Layer(RenderLayer::Default)
{

}
dae::Component::~Component()
{
	m_pOwner = nullptr;
}

void dae::Component::SetRenderLayer(RenderLayer layer)
{
	m_Layer = layer;
}
