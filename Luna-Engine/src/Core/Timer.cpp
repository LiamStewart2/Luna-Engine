#include "Timer.h"

Timer* Timer::s_ApplicationTimer = nullptr;

Timer::Timer()
{
	m_StartFrame = std::chrono::steady_clock::now();
	Tick();
}

float Timer::Time()
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - m_StartFrame).count();
}

float Timer::DeltaTime()
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - m_LastFrame).count();
}

void Timer::Tick()
{
	m_LastFrame = std::chrono::steady_clock::now();
	m_TickCount++;

	m_TicksSinceFPSCalcutation++;
	float fpschecktime = std::chrono::duration<float>(std::chrono::steady_clock::now() - m_LastFPSCheck).count();
	if(fpschecktime >= 1)
	{
		m_FPS = m_TicksSinceFPSCalcutation / fpschecktime;
		m_TicksSinceFPSCalcutation = 0;
		m_LastFPSCheck = std::chrono::steady_clock::now();
	}
}
