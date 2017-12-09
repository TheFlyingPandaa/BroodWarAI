
#ifndef STRATEGYBUILD_H
#define STRATEGYBUILD_H
#define INT_MAX 2147483647
#include <BWAPI/UnitType.h>
#include <BWAPI.h>

const int buildings = 7;



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


	void buildingBuilt();

	bool exists(BWAPI::Unit u) const;
	void add(BWAPI::Unit u);
private:

	//Pallar inte g�ra en Glass f�r detta
	BWAPI::UnitType buildOrder[buildings];
	int miniralGoal[buildings];
	int gasGoal[buildings];
	BWAPI::Unit uBuild[buildings];

	int buildStage;

	bool isCommandCenter;

	bool isRefinary;
	//int miniralGoal;

};

#endif

