#pragma once
#include <string>
#include <memory>

#include "GameObject.h"
#include "Transform.h"
#include "Component.h"

namespace dae
{
	class Font;
	class Texture2D;
	class Transform;
	class Component;
	class GameObject;

	class TextComponent final : public Component
	{
	public:
		void Update() override;
		void Render() const override;

		void SetText(const std::string& text);

		TextComponent(GameObject& owner, const std::string& text, std::shared_ptr<Font> font);
		virtual ~TextComponent();

		glm::ivec2 GetTextureSize() const;

		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		bool m_needsUpdate;
		std::string m_text;
		std::shared_ptr<Font> m_font;
		std::unique_ptr<dae::Texture2D> m_TextTexture;
	}; 
}


