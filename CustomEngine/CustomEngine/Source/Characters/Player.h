#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "../Animation/Animation.h"
#include "Character.h"
#include "../Physics/RigidBody.h"

class Player : public Character
{
	public:
		Player(Properties* props);

		virtual void Draw();
		virtual void Clean();
		virtual void Update(float dt);
	private:

		Animation* m_Animation;
		RigidBody* m_RigidBody;

};

#endif // !PLAYER_H