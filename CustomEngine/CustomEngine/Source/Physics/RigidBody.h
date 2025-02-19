#pragma once

#include"Vector2D.h"
#include "../Collision/CollisionTypes.h"
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
		RigidBody()
			: m_Mass(UNI_MASS), m_Gravity(GRAVITY) {
			auto collider = std::make_shared<RectCollider>(0, 0, 100, 100, PhysicsBody); // Initializes a default rectangular collision by default
			SetCollider(collider); 
		}

		std::shared_ptr<RigidBody> GetSharedPtr() {
			return std::shared_ptr<RigidBody>(this, [](RigidBody*) { /* no-op deleter */ });
		}

		~RigidBody() {
			std::cout << "RigidBody destroyed\n";
		}

		//Setter Gravity & Mass
		inline void SetMass(float mass) { m_Mass = mass; }
		inline void SetGravity(float gravity) { m_Gravity = gravity; }
		void SetCollider(std::shared_ptr<Collider> collider) {
			m_ColliderRB = std::move(collider);
			PhysicsWorld::GetInstance()->RegisterCollider(m_ColliderRB);
		}

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

		void SetOwner(const std::shared_ptr<GameObject>& owner) {
			m_ColliderRB->SetOwner(owner);
		}

		std::shared_ptr<GameObject> GetOwner() const {
			return m_ColliderRB->GetOwner();
		}
		
		void Update(float dt) {
			UpdateAcceleration();
			UpdateVelocity(dt);
			UpdatePosition(dt);
		}

		void UpdateAcceleration() {
			m_Acceleration.X = (m_Force.X + m_Friction.X) / m_Mass;
			m_Acceleration.Y = m_Gravity + (m_Force.Y / m_Mass);
		}

		// Stop downward velocity when on the ground
		void ResolveGroundCollision() {
			if (m_Velocity.Y >= 0) {
				SetVelocityY(0);
				SetAccelerationY(0);
				ApplyForceY((-1)*m_Gravity);
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

		std::shared_ptr<Collider> m_ColliderRB;
};