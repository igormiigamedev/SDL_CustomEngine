#pragma once

#ifndef CHARACTER_H
#define CHARACTER_H

#include "../Object/GameObject.h"
#include <string>
#include "../Collision/CollisionTypes.h"
#include "../Physics/RigidBody.h"
#include "IDamage.h"


class Character : public GameObject, public IDamage
{
	public:
		Character(const Properties& props, Transform transform)
			: GameObject(props, transform) {
		}

		void InitGameObject() override {
			RegisterCollisionCallback();
			m_RigidBody.SetOwner(shared_from_this());
		}


		virtual ~Character() {
			std::cout << "Character destroyed\n";
		}

		virtual GameObjectType GetType() const = 0;

		virtual void Draw() = 0;
		virtual void Clean() = 0;
		virtual void Update(float dt) = 0;
		virtual void OnCollision(std::shared_ptr<GameObject> target) = 0;

		RigidBody& GetRigidBody() {
			return m_RigidBody;
		}


		void RegisterCollisionCallback() {
			EventDispatcher::GetInstance()->RegisterCollisionCallback(
				shared_from_this(), // Callback owner (the object that is registering)
				[this](RigidBody* otherBody) {
					OnCollision(otherBody->GetOwner()); // Calls the OnCollision method
				},
				[this](const CollisionEvent& event) {
					// Filters events relevant to the owner
					return event.bodyA->GetOwner().get() == this || event.bodyB->GetOwner().get() == this;
				}
			);
		}

		// Method to set Collider type
		void SetColliderAsCircle(float x, float y, float radius) {
			m_RigidBody.SetCollider(std::make_unique<CircleCollider>(x, y, radius));
		}

		void SetColliderAsRect(float x, float y, float width, float height) {
			m_RigidBody.SetCollider(std::make_unique<RectCollider>(x, y, width, height));
		}

	protected:

		std::string m_Name;
		virtual void OnTakeDamage(float damage) = 0;

	private:
		RigidBody m_RigidBody;

		void TakeDamage(float damage)override {
			OnTakeDamage(damage);
		};

};

#endif // !CHARACTER_H


