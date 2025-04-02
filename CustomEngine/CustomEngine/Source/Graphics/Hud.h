#pragma once
#include <SDL_timer.h>
#include <SDL_render.h>
#include <iostream>
#include "../Text/TextBox.h"
#include "../GameMode/GameMode.h"
#include "Button.h"
#include "../Graphics/Widget.h"

class Hud
{
public:
	virtual ~Hud() = default; 
	virtual void Render(SDL_Renderer* renderer) {
		RenderFadeOut(renderer);
		for (auto& widget : widgets) {
			if (widget->IsVisible()) {
				widget->Render();
			}
		}
	}

	virtual void LoadTextures() = 0; 

	bool PlayFadeOut(int fadeDuration);
	void RenderFadeOut(SDL_Renderer* renderer) const;

	const std::vector<std::unique_ptr<Widget>>& GetWidgetList() const {
		return widgets;
	}

	template <typename T, typename... Args>
	T* CreateWidget(SDL_Renderer* renderer, Args&&... args) {
		static_assert(std::is_base_of<Widget, T>::value, "T must be derived from Widget");

		auto widget = std::make_unique<T>(renderer, std::forward<Args>(args)...);
		T* widgetPtr = widget.get();
		widgets.push_back(std::move(widget));
		return widgetPtr;
	}

	template <typename T>
	T* GetWidgetOfClass() {
		for (const auto& widget : widgets) {
			if (T* castedWidget = dynamic_cast<T*>(widget.get())) {
				return castedWidget;
			}
		}
		return nullptr;
	}


private:

	Uint32 fadeInitTime = 0; // Time the fade init
	bool waitingForFadeOut = false;
	int fadeAlpha = 0;  // Fade opacity level (0 = invisible, 255 = completely black)

protected:
	struct PanelInfos
	{
		std::string textureID;
		int posX;
		int posY;
		int width;
		int height;
	};

	std::vector<std::unique_ptr<Widget>> widgets;
};

