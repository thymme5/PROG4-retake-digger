#include "Component.h"

dae::Component::Component(GameObject& owner):
	m_pOwner{ &owner } 
{

}
dae::Component::~Component()
{
	m_pOwner = nullptr;
}
