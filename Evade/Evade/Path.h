#ifndef Path_h__
#define Path_h__

#include "PluginSDK.h"

template<typename T>
struct Path
{
	T			LastStep;
	double		TotalCost;
	Path<T>*	PrevSteps;

	Path(T start)
	{
		LastStep	= start;
		PrevSteps	= nullptr;
		TotalCost	= 0;
	}

	Path(T lastStep, Path<T> prevSteps, double totalCost)
	{
		LastStep = lastStep;
		PrevSteps = prevSteps;
		TotalCost = totalCost;
	}

	Path<T> AddStep(T step, double stepCost)
	{
		return Path<T>(step, this, TotalCost + stepCost);
	}
};

#endif // Path_h__
