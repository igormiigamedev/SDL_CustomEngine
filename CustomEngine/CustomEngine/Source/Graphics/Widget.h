#pragma once
#include <vector>
#include <memory>
#include "Button.h"
#include "../Text/TextBox.h"
#include "TextureManager.h"

class Widget {
public:
    Widget(SDL_Renderer* renderer) : m_Renderer(renderer), isVisible(true) {}
    virtual ~Widget() = default;

    virtual void Init() = 0; // Each child class defines how it will be created

    void Render() {
        if (!isVisible) return;

        for (auto& panel : imagePanels) {
            TextureManager::GetInstance()->Draw(panel.textureID, panel.posX, panel.posY, 1.0f, 1.0f);
        }
        for (auto& button : buttonList) {
            button->Render(m_Renderer);
        }
        for (auto& text : textBoxes) {
            text->Render(m_Renderer);
        }
    }

    void Show() { isVisible = true; }
    void Hide() { isVisible = false; }
    bool IsVisible() { return isVisible; }

    const std::vector<std::unique_ptr<Button>>& GetButtonList() const {
        return buttonList;
    }

protected:
    SDL_Renderer* m_Renderer;
    bool isVisible;

    struct ImagePanel {
        std::string textureID;
        int posX, posY;
        int width, height;
    };

    std::vector<ImagePanel> imagePanels;
    std::vector<std::unique_ptr<Button>> buttonList;
    std::vector<std::unique_ptr<TextBox>> textBoxes;
};


