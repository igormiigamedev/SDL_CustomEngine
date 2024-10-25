#pragma once

//const int TARGET_FPS = 1;
const float TARGET_DELTATIME = 0.02f;

class Timer{
	public:
		void Tick();
		inline float GetDeltaTime() { return m_DeltaTime; }
		inline static Timer* GetInstance() { 
			return s_Instance = (s_Instance != nullptr) ? s_Instance : new Timer();
		}

	private:
		Timer() {}
		static Timer* s_Instance;
		float m_DeltaTime;
		float m_LastTime;
};

