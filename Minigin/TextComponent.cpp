#pragma once
#include "TextComponent.h"

#include <stdexcept>
#include <SDL_ttf.h>
#include <unordered_set>

#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"


dae::TextComponent::TextComponent(GameObject& owner, const std::string& text, std::shared_ptr<Font> font)
	: Component(owner), m_needsUpdate(true), m_text(text), m_font(std::move(font)), m_TextTexture(nullptr)
{
}
void dae::TextComponent::Update()
{
    if (m_needsUpdate)
    {
        // Split the text into lines
        std::vector<std::string> lines;
        std::string currentLine; // Renamed to avoid conflict
        std::istringstream stream(m_text);
        while (std::getline(stream, currentLine))
        {
            lines.push_back(currentLine);
        }

        const SDL_Color color = { 255, 255, 255, 255 }; // White text
        TTF_Font* font = m_font->GetFont();

        // Calculate the total height and maximum width of the text block
        int totalHeight = 0;
        int maxWidth = 0;
        std::vector<SDL_Surface*> surfaces;

        for (const auto& line : lines)
        {
            SDL_Surface* surf = TTF_RenderText_Blended(font, line.c_str(), color);
            if (!surf)
            {
                throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
            }

            surfaces.push_back(surf);
            totalHeight += surf->h;
            if (surf->w > maxWidth)
            {
                maxWidth = surf->w;
            }
        }

        // Create a single surface to combine all lines
        SDL_Surface* combinedSurface = SDL_CreateRGBSurface(0, maxWidth, totalHeight, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        if (!combinedSurface)
        {
            throw std::runtime_error(std::string("Create combined surface failed: ") + SDL_GetError());
        }

        // Blit each line onto the combined surface
        int yOffset = 0;
        for (SDL_Surface* surf : surfaces)
        {
            SDL_Rect destRect = { 0, yOffset, surf->w, surf->h };
            SDL_BlitSurface(surf, nullptr, combinedSurface, &destRect);
            yOffset += surf->h;
            SDL_FreeSurface(surf); // Free individual line surface
        }

        // Create a texture from the combined surface
        auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), combinedSurface);
        if (!texture)
        {
            SDL_FreeSurface(combinedSurface);
            throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
        }

        SDL_FreeSurface(combinedSurface);

        // Update the single texture
        m_TextTexture = std::make_unique<Texture2D>(texture);
        m_needsUpdate = false;
    }
}
dae::TextComponent::~TextComponent()
{
	
}
glm::ivec2 dae::TextComponent::GetTextureSize() const
{
    if (m_TextTexture)
        return m_TextTexture->GetSize();
    return { 0, 0 };
}

void dae::TextComponent::Render() const
{
    if (m_TextTexture != nullptr)
    {
        const auto& pos = m_pOwner->GetTransform().GetPosition();
        dae::Renderer::GetInstance().RenderTexture(*m_TextTexture, pos.x, pos.y);
    }
}

void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}
