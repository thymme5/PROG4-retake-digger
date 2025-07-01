#pragma once
#include "Component.h"
#include <iostream>
#include "Texture2D.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"

class GameObject;

namespace dae
{
	class TextureComponent final : public Component
	{
	public:
		TextureComponent(GameObject& pOwner, const std::string& filename, float scale = 1.f, int frames = 0);
		virtual ~TextureComponent();
		void Update() override;
		void Render() const override;

		void SetTexture(const std::string& filename);
		void SetSourceRect(const SDL_Rect& srcRect);
		void ClearSourceRect();

		float GetWidth() const noexcept;
		float GetHeight() const noexcept;
		std::shared_ptr<Texture2D> GetTexture() const noexcept { return m_texture; }

		void SetVisible(bool visible);
		bool GetVisible() const noexcept;
	private:
		std::shared_ptr<Texture2D> m_texture{};
		float m_Scale;
		SDL_Rect m_SrcRect{ 0, 0, 0, 0 };
		bool m_UseSrcRect{ false };
		bool m_IsVisible{ true };
	};
}

