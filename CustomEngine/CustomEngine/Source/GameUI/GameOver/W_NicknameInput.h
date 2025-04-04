#pragma once
#include "../../Graphics/Widget.h"
#include "../../Text/TextBox.h"
#include <SDL.h>
#include <memory>
#include <iostream>

class W_NicknameInput : public Widget {
public:
    W_NicknameInput(SDL_Renderer* renderer)
        : Widget(renderer), nickname(""), showCursor(true), lastCursorToggle(SDL_GetTicks()), isActive(true) {
        SDL_StartTextInput();
    }

    ~W_NicknameInput() override {
        SDL_StopTextInput();
    }

    void Init() override {
        SDL_Color textColor = { 255, 255, 255, 255 };
        SDL_Rect rect = { 300, 300, 300, 50 };

        nicknameTextBox = std::make_unique<TextBox>(m_Renderer, rect, textColor, 200, "|");
        textBoxes.push_back(std::move(nicknameTextBox));
    }

    void HandleEvent(const SDL_Event& event) {
        if (!isActive || !isVisible) return;

        switch (event.type) {
        case SDL_TEXTINPUT:
            nickname += event.text.text;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_BACKSPACE && !nickname.empty()) {
                nickname.pop_back();
            }
            else if (event.key.keysym.sym == SDLK_RETURN) {
                ConfirmNickname();
            }
            break;
        }
    }

    void Render() {
        std::cout << "Chamando NickName Render " << nickname << std::endl;
        if (!isVisible) return;

        UpdateCursorBlink();
        if (!textBoxes.empty()) {
            std::string displayText = nickname + (showCursor ? "|" : "");
            textBoxes[0]->SetText(displayText);
        }

        /*Widget::Render(); */// chama o render padrão da base
    }

    std::string GetNickname() const {
        return nickname;
    }

    void ConfirmNickname() {
        isActive = false;
        SDL_StopTextInput();
        std::cout << "Nickname confirmado: " << nickname << std::endl;
    }

private:
    std::string nickname;
    bool isActive;
    bool showCursor;
    Uint32 lastCursorToggle;
    const Uint32 cursorBlinkInterval = 500;

    std::unique_ptr<TextBox> nicknameTextBox;

    void UpdateCursorBlink() {
        Uint32 now = SDL_GetTicks();
        if (now - lastCursorToggle >= cursorBlinkInterval) {
            showCursor = !showCursor;
            lastCursorToggle = now;
        }
    }
};