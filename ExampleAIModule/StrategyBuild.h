
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

	void buildingBuilt();
private:

	//Pallar inte göra en Glass för detta
	BWAPI::UnitType buildOrder[2];
	int miniralGoal[2];

	int buildStage;
	//int miniralGoal;

};

#endif

