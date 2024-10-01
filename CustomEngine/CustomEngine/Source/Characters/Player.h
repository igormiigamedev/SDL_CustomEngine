#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"

class Player : public Character
{
	public:
		Player(Properties* props);

		virtual void Draw();
		virtual void Clean();
		virtual void Update(float dt);
	private:
		int m_Row, m_Frame, m_FrameCount;
		int m_AnimSpeed;

};

#endif // !PLAYER_H