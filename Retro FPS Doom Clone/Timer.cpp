#include "Timer.h"


Timer::Timer()
{
	m_elapsedTicks = 0;
	m_totalTicks = 0;
	m_frameCount = 0;
	m_framesPerSecond = 0;
	m_framesThisSecond = 0;
	m_qpcSecondCounter = 0;

	if (!QueryPerformanceFrequency(&m_qpcFrequency))
	{
		throw std::runtime_error("Error");
	}

	if (!QueryPerformanceCounter(&m_qpcLastTime))
	{
		throw std::runtime_error("Error");
	}

	// Initialize max delta to 1/10 of a second.
	m_qpcMaxDelta = m_qpcFrequency.QuadPart / 10;
}

void Timer::Update()
{
	// calculates the time from the program starting
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	UINT64 timeDelta = currentTime.QuadPart - m_qpcLastTime.QuadPart;
	m_qpcLastTime = currentTime;
	m_qpcSecondCounter += timeDelta;

	timeDelta *= TicksPerSecond;
	timeDelta /= m_qpcFrequency.QuadPart;

	UINT32 lastFrameCount = m_frameCount;

	m_elapsedTicks = timeDelta;
	m_totalTicks += timeDelta;
	m_frameCount++;

	// Track the current framerate.
	if (m_frameCount != lastFrameCount)
	{
		m_framesThisSecond++;
	}

	if (m_qpcSecondCounter >= static_cast<UINT64>(m_qpcFrequency.QuadPart))
	{
		m_framesPerSecond = m_framesThisSecond;
		m_framesThisSecond = 0;
		m_qpcSecondCounter %= m_qpcFrequency.QuadPart;
	}
}

