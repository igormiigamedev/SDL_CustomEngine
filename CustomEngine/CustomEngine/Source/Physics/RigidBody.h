#pragma once

#include"Vector2D.h"
#include "../Collision/CollisionTypes.h"
#include "Collider.h"
#include "../Collision/CollisionHandler.h"
#include "../Events/EventDispatcher.h"
#include "../Collision/PhysicsWorld.h"

#define UNI_MASS 1.0f
#define GRAVITY 9.8f

#define FORWARD 1
#define BACKWARD -1

#define UPWARD -1
#define DOWNWARD +1

class RigidBody{
	public:
		RigidBody(GameObject* owner)
			: m_Owner(owner), m_Mass(UNI_MASS), m_Gravity(GRAVITY) {
			m_ColliderRB = std::make_unique<Collider>();
		}

		//Setter Gravity & Mass
		inline void SetMass(float mass) { m_Mass = mass; }
		inline void SetGravity(float gravity) { m_Gravity = gravity; }

		//Force
		inline void ApplyForce(Vector2D force) { m_Force = force; }
		inline void ApplyForceX(float forceX) { m_Force.X = forceX; }
		inline void ApplyForceY(float forceY) { m_Force.Y = forceY; }
		inline void SetVelocityX(float velocityX) { m_Velocity.X = velocityX; }
		inline void SetVelocityY(float velocityY) { m_Velocity.Y = velocityY; }
		inline void SetAccelerationY(float accelerationY) { m_Acceleration.Y = accelerationY; }
		inline void UnSetForceY() { m_Force.Y = 0; }
		inline void UnSetForce() { m_Force = Vector2D(0, 0); }

		//Friction
		inline void ApplyFriction(Vector2D friction) { m_Friction = friction; }
		inline void UnSetFriction() { m_Friction = Vector2D(0, 0); }

		//Getters
		inline float GetMass() { return m_Mass; }
		inline Vector2D GetDeltaPosition() { return m_DeltaPosition; }
		inline Vector2D GetVelocity() { return m_Velocity; }
		inline Vector2D GetAcceleration() { return m_Acceleration; }
		inline Collider* GetCollider() const {
			return m_ColliderRB.get();
		}

		std::shared_ptr<GameObject> GetOwner() const {
			return m_Owner;
		}
		
		void Update(float dt) {
			UpdateAcceleration();
			HandleCollision(dt);
			UpdateVelocity(dt);
			UpdatePosition(dt);
		}

		void UpdateAcceleration() {
			m_Acceleration.X = (m_Force.X + m_Friction.X) / m_Mass;
			m_Acceleration.Y = m_Gravity + (m_Force.Y / m_Mass);
		}

		// Handle collision response
		void HandleCollision(float dt) {
			if (m_ColliderRB == nullptr) return;

			switch (m_ColliderRB->GetCollisionResponse(WorldFloor)) {
			case IGNORE:
				break;
			case BLOCK:
				CheckMapCollision(dt);
				break;
			case OVERLAP:
				// Handle overlap case if needed
				break;
			default:
				break;
			}

			switch (m_ColliderRB->GetCollisionResponse(PhysicsBody)) {
			case IGNORE:
				break;
			case BLOCK:
				break;
			case OVERLAP:
				CheckCollisionWithRigidBodies();
				break;
			default:
				break;
			}
		}

		void CheckCollisionWithRigidBodies() {
			auto& rigidBodies = PhysicsWorld::GetInstance()->GetRigidBodies();
			for (const auto& other : rigidBodies) {
				if (other.get() == this) continue;

				if (CollisionHandler::GetInstance()->CheckCollision(m_ColliderRB->Get(), other->GetCollider()->Get())) {
					CollisionEvent event(this, other.get());
					EventDispatcher::GetInstance()->DispatchCollisionEvent(event);
				}
			}
		}

		void CheckMapCollision(float dt) {
			switch (CollisionHandler::GetInstance()->DetectTileCollision(m_ColliderRB->Get())) {
			case CollisionLocation::Below:
				ResolveGroundCollision(dt);
				break;
			case CollisionLocation::Top:
				ResolveCeilingCollision();
				break;
			default:
				break;
			}
		}

		// Stop downward velocity when on the ground
		void ResolveGroundCollision(float dt) {
			if (m_Velocity.Y >= 0) {
				SetVelocityY(0);
				SetAccelerationY(0);
			}
		}

		// Stop upward velocity when hitting a ceiling
		void ResolveCeilingCollision() {
			if (m_Velocity.Y < 0) {
				SetVelocityY(0);
			}
		}

		void UpdateVelocity(float dt) {
			m_Velocity.X += m_Acceleration.X * dt;
			m_Velocity.Y += m_Acceleration.Y * dt;
		}

		void UpdatePosition(float dt) {
			m_DeltaPosition = m_Velocity * dt;
		}


	private:
		float m_Mass;
		float m_Gravity;

		Vector2D m_Force;
		Vector2D m_Friction;

		Vector2D m_DeltaPosition;
		Vector2D m_Velocity;
		Vector2D m_Acceleration;

		std::unique_ptr<Collider> m_ColliderRB;

		std::shared_ptr<GameObject> m_Owner;
};