#pragma once

#ifndef CHARACTER_H
#define CHARACTER_H

#include "../Object/GameObject.h"
#include <string>
#include "../Collision/CollisionTypes.h"
#include "../Physics/RigidBody.h"

class Character : public GameObject
{
	public:
		Character(const Properties& props, Transform transform) : GameObject(props, transform) {
			m_RigidBody = std::make_shared<RigidBody>(this);  
			PhysicsWorld::GetInstance()->RegisterRigidBody(m_RigidBody);
		}
		virtual ~Character() {
			PhysicsWorld::GetInstance()->UnregisterRigidBody(m_RigidBody); 
		}

		virtual GameObjectType GetType() const = 0;

		virtual void Draw() = 0;
		virtual void Clean() = 0;
		virtual void Update(float dt) = 0;

		std::shared_ptr<RigidBody> GetRigidBody() const {
			return m_RigidBody;
		}

	protected:
		std::string m_Name;

	private:
		std::shared_ptr<RigidBody> m_RigidBody;

};

#endif // !CHARACTER_H


