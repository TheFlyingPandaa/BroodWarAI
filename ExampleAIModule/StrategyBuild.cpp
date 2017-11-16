#include "StrategyBuild.h"

StrategyBuild::StrategyBuild()
{
	this->buildStage = 0;

	this->buildOrder[0] = BWAPI::UnitTypes::Enum::Terran_Supply_Depot;
	this->buildOrder[1] = BWAPI::UnitTypes::Enum::Terran_Barracks;

	this->miniralGoal[0] = 100;
	this->miniralGoal[1] = 150;


}

StrategyBuild::~StrategyBuild()
{
	
}

BWAPI::UnitType StrategyBuild::getCurrentBuild()
{
	return buildOrder[buildStage];
}

int StrategyBuild::getMiniralGoal()
{
	return miniralGoal[buildStage];
}


void StrategyBuild::buildingBuilt()
{
	this->buildStage++;
}

int StrategyBuild::getBuildStage()
{
	return this->buildStage;
}


