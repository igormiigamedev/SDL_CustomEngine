#pragma once

#ifndef CHARACTER_H
#define CHARACTER_H

#include "../Object/GameObject.h"
#include <string>
#include "../Collision/CollisionTypes.h"

class Character : public GameObject
{
	public:
		Character(const Properties& props, Transform transform) : GameObject(props, transform) {}

		virtual void Draw() = 0;
		virtual void Clean() = 0;
		virtual void Update(float dt) = 0;

	protected:
		std::string m_Name;

};

#endif // !CHARACTER_H


