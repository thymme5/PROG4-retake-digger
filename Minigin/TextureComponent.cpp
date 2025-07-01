#include "TextureComponent.h"

dae::TextureComponent::TextureComponent(GameObject& owner, const std::string& filename, float scale, int frames)
	: Component(owner), m_texture(ResourceManager::GetInstance().LoadTexture(filename)), m_Scale(scale)
{
	//very simple thing that makes sure just the first sprite gets selected from a spritesheet
	//this is only handy when a spritesheet does not have animations but rather states
	if (frames > 0 && m_texture)
	{
		int textureWidth = m_texture->GetSize().x;
		int textureHeight = m_texture->GetSize().y;

		int frameWidth = textureWidth / frames;

		m_SrcRect = SDL_Rect{ 0, 0, frameWidth, textureHeight };
		m_UseSrcRect = true;
	}
}
dae::TextureComponent::~TextureComponent()
{

}
void dae::TextureComponent::Update() 
{

}
void dae::TextureComponent::Render() const 
{
	if (m_IsVisible)
	{
		const auto& pos = m_pOwner->GetWorldPosition();

		if (!m_texture) return;

		if (m_UseSrcRect)
		{
			float width = static_cast<float>(m_SrcRect.w) * m_Scale;
			float height = static_cast<float>(m_SrcRect.h) * m_Scale;

			Renderer::GetInstance().RenderTexture(*m_texture, m_SrcRect, pos.x, pos.y, width, height);
		}
		else
		{
			Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y, m_texture->GetSize().x * m_Scale, m_texture->GetSize().y * m_Scale);
		}
	}
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}
void dae::TextureComponent::SetSourceRect(const SDL_Rect& srcRect)
{
	m_SrcRect = srcRect;
	m_UseSrcRect = true;
}
void dae::TextureComponent::ClearSourceRect()
{
	m_UseSrcRect = false;
}

float dae::TextureComponent::GetWidth() const noexcept
{
	return float(m_texture->GetSize().x);
}
float dae::TextureComponent::GetHeight() const noexcept
{
	return float(m_texture->GetSize().y);
}
void dae::TextureComponent::SetVisible(bool visible)
{
	m_IsVisible = visible;
}
bool dae::TextureComponent::GetVisible() const noexcept
{
	return m_IsVisible;
}