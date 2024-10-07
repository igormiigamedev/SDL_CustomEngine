#pragma once

#include"Vector2D.h"

#define UNI_MASS 1.0f
#define GRAVITY 9.8f

#define FORWARD 1
#define BACKWARD -1

#define UPWARD -1
#define DOWNWARD +1

class RigidBody
{
	public:
		RigidBody() {
			m_Mass = UNI_MASS;
			m_Gravity = GRAVITY;
		}

		//Setter Gravity & Mass
		inline void SetMass(float mass) { m_Mass = mass; }
		inline void SetGravity(float gravity) { m_Gravity = gravity; }

		//Force
		inline void ApplyForce(Vector2D force) { m_Force = force; }
		inline void ApplyForceX(float forceX) { m_Force.X = forceX; }
		inline void ApplyForceY(float forceY) { m_Force.Y = forceY; }
		inline void UnSetForceY() { m_Force.Y = 0; }
		inline void UnSetForce() { m_Force = Vector2D(0, 0); }

		//Friction
		inline void ApplyFriction(Vector2D friction) { m_Friction = friction; }
		inline void UnSetFriction() { m_Friction = Vector2D(0, 0); }

		//Getters
		inline float GetMass() { return m_Mass; }
		inline Vector2D GetPosition() { return m_Position; }
		inline Vector2D GetVelocity() { return m_Velocity; }
		inline Vector2D GetAcceleration() { return m_Acceleration; }
			
		//Update Metods
		void Update(float dt) {
			m_Acceleration.X = (m_Force.X + m_Friction.X) / m_Mass;
			m_Acceleration.Y = m_Gravity + (m_Force.Y / m_Mass);
			m_Velocity = m_Acceleration * dt;
			m_Position = m_Velocity * dt;
		}


	private:
		float m_Mass;
		float m_Gravity;

		Vector2D m_Force;
		Vector2D m_Friction;

		Vector2D m_Position;
		Vector2D m_Velocity;
		Vector2D m_Acceleration;

};

