#include "Time.h"
#include "Time.h"


namespace Time
{
	namespace
	{
		double frameStart = 0.0;
		double frameEnd = 0.0;

		double deltaTime = 0.0;
	}

	void SetStartTime(double time)
	{
		frameStart = 0.0f;
	}

	void SetEndTime(double time)
	{
		frameEnd = time;
		deltaTime = frameEnd - frameStart;
	}

	double DeltaTime()
	{
		return deltaTime;
	}
}