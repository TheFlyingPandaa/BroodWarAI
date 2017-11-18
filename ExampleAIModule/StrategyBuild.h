
#ifndef STRATEGYBUILD_H
#define STRATEGYBUILD_H
#define INT_MAX 2147483647
#include <BWAPI/UnitType.h>

class StrategyBuild
{
public:
	StrategyBuild();
	~StrategyBuild();

	BWAPI::UnitType getCurrentBuild();
	int getMiniralGoal();

	int getBuildStage();
	bool getIsCommandCenter();


	void buildingBuilt();
private:

	//Pallar inte g�ra en Glass f�r detta
	BWAPI::UnitType buildOrder[3];
	int miniralGoal[4];

	int buildStage;

	bool isCommandCenter;
	//int miniralGoal;

};

#endif

