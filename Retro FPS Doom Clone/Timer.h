#pragma once
#include "pch.h"
#include <stdexcept>

// A large section of this is taken from the StepTimer 
// in the Windows DirectX UWP Example app and is refactored for this
// projects needs

class Timer
{
public:
	Timer();
	~Timer() = default;
	
	// Update the timer.
	void Update();

	// Get the current delta tick rate.
	UINT64 GetDeltaTicks() const { return m_elapsedTicks; }
	double GetDeltaTicksInSeconds() const { return TicksToSeconds(m_elapsedTicks); }
 
	// Get total time since the start of the program.
	UINT64 GetTotalTicks() const { return m_totalTicks; }
	double GetTotalSeconds() const { return TicksToSeconds(m_totalTicks); }

	// Get total number of updates since start of the program.
	UINT32 GetFrameCount() const { return m_frameCount; }

	// Get the current framerate.
	UINT32 GetFramesPerSecond() const { return m_framesPerSecond; }

	static const UINT64 TicksPerSecond = 10000000;
	static double TicksToSeconds(UINT64 ticks) { return static_cast<double>(ticks) / TicksPerSecond; }

private:

	LARGE_INTEGER m_qpcFrequency;
	LARGE_INTEGER m_qpcLastTime;
	UINT64 m_qpcMaxDelta;

	UINT64 m_elapsedTicks;
	UINT64 m_totalTicks;

	UINT32 m_frameCount;
	UINT32 m_framesPerSecond;
	UINT32 m_framesThisSecond;
	UINT32 m_qpcSecondCounter;
};

