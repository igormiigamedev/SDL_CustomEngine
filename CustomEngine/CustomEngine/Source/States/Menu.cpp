#include "Menu.h"
#include "../Core/Engine.h"
#include "../Inputs/InputHandler.h"

Menu::Menu() {}

void Menu::SetHud()
{
}

bool Menu::Init(){
	GameState::Init();

	std::cout << "menu initialized" << std::endl;
	return true;
}

bool Menu::Exit(){
	std::cout << "Exit Menu!" << std::endl;
	return true;
}

void Menu::Update(){

}

void Menu::Render(){
	SDL_SetRenderDrawColor(m_Ctxt, 16, 45, 70, 255);
	SDL_RenderClear(m_Ctxt);
	SDL_RenderPresent(m_Ctxt);
}

//Callbacks
void Menu::StartGame(){
	std::cout << "Start Game! \n" << std::endl;
}

void Menu::Settings(){
	std::cout << "Options Mode \n" << std::endl;
}

void Menu::Editor(){
	std::cout << "Editor Mode \n" << std::endl;
}

void Menu::Quit(){

}
