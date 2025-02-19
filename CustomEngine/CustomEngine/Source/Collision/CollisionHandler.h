#pragma once

#include "SDL.h"
#include <vector>
#include "../Physics/Collider.h"
#include "../Physics/RectCollider.h"
#include "../Physics/CircleCollider.h"

class CollisionHandler{

	public:
		bool CheckCollision(Collider& a, Collider& b);

		bool CheckRectCircleCollision(const CircleCollider& circleCollider, const RectCollider& rectCollider) const;
		bool CheckCircleCollision(const CircleCollider& checker, const CircleCollider& other) const;

		inline static CollisionHandler* GetInstance() { return s_Instance = (s_Instance != nullptr) ? s_Instance : new CollisionHandler(); }

		void CheckCollisionsWithCollidersOfType(Collider& sourceCollider, ObjectResponses typeOfCollision);

	private:
		CollisionHandler();
		
		static CollisionHandler* s_Instance;

		std::vector < std::shared_ptr<Collider>> m_ColliderList;
};

