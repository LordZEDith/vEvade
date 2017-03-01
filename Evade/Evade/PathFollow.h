#ifndef PathFollow_h__
#define PathFollow_h__

#include "PluginSDK.h"
#include "Evade.h"

class CPathFollow
{
public:
	CPathFollow()
	{
		paths.clear();
	}

	bool IsFollowing()
	{
		return (paths.size() > 0);
	}

	void KeepFollowPath()
	{
		if (paths.size() == 0)
			return;

		while (paths.size() > 0 && Evade::PlayerPosition.DistanceTo(paths[0]) < 80)
			paths.erase(paths.begin());

		if (paths.size() > 0)
			Util::Move(paths[0]);
	}

	void Start(std::vector<Vec2> path)
	{
		paths = path;
	}

	void Stop()
	{
		paths.clear();
	}

private:
	std::vector<Vec2> paths;
};

extern CPathFollow* PathFollow;

#endif // PathFollow_h__
