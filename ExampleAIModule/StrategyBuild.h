
#ifndef STRATEGYBUILD_H
#define STRATEGYBUILD_H
#define INT_MAX 2147483647
#include <BWAPI/UnitType.h>
#include <BWAPI.h>

const int buildings = 9;



class StrategyBuild
{
public:
	StrategyBuild();
	~StrategyBuild();

	BWAPI::UnitType getCurrentBuild();
	int getMiniralGoal();
	int getGasGoal();

	int getBuildStage();
	bool getIsCommandCenter();
	bool getIsRefinary() const;
	bool getIsBuildAddon() const;


	void buildingBuilt();

	bool exists(BWAPI::Unit u) const;
	void add(BWAPI::Unit u);
private:

	//Pallar inte göra en Glass för detta
	BWAPI::UnitType buildOrder[buildings];
	int miniralGoal[buildings];
	int gasGoal[buildings];
	BWAPI::Unit uBuild[buildings];

	int buildStage;

	bool isCommandCenter;
	bool isRefinary;
	bool isBuildAddon;
	//int miniralGoal;

};

#endif

