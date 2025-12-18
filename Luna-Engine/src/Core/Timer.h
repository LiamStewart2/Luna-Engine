#pragma once

#include <chrono>

class Timer
{
public:
	Timer();

	float AverageTPS() { return m_FPS; }
	float Time();

	float DeltaTime();
	void Tick();

	static void CreateApplicationTimer() 
	{ 
		if (s_ApplicationTimer == nullptr)
			s_ApplicationTimer = new Timer(); 
	}

	static Timer* GetApplicationTimer() 
	{ 
		return s_ApplicationTimer; 
	}

	static void ResetApplicationTimer()
	{
		if (s_ApplicationTimer != nullptr)
		{
			delete s_ApplicationTimer;
			s_ApplicationTimer = nullptr;
		}
		CreateApplicationTimer();
	}

private:
	std::chrono::steady_clock::time_point m_LastFrame;
	std::chrono::steady_clock::time_point m_StartFrame;

	unsigned int m_TickCount = 0;
	unsigned int m_TicksSinceFPSCalcutation = 0;
	std::chrono::steady_clock::time_point m_LastFPSCheck;

	float m_FPS = 0.0f;

	static Timer* s_ApplicationTimer;
};

