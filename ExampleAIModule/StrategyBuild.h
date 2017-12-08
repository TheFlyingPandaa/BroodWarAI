
#ifndef STRATEGYBUILD_H
#define STRATEGYBUILD_H
#define INT_MAX 2147483647
#include <BWAPI/UnitType.h>

const int buildings = 6;

class StrategyBuild
{
public:
	StrategyBuild();
	~StrategyBuild();

	BWAPI::UnitType getCurrentBuild();
	int getMiniralGoal();

	int getBuildStage();
	bool getIsCommandCenter();
	bool getIsRefinary() const;


	void buildingBuilt();
private:

	//Pallar inte göra en Glass för detta
	BWAPI::UnitType buildOrder[buildings];
	int miniralGoal[buildings];

	int buildStage;

	bool isCommandCenter;

	bool isRefinary;
	//int miniralGoal;

};

#endif

