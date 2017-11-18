#include "StrategyBuild.h"

StrategyBuild::StrategyBuild()
{
	this->buildStage = 0;

	this->buildOrder[0] = BWAPI::UnitTypes::Enum::Terran_Supply_Depot;
	this->buildOrder[1] = BWAPI::UnitTypes::Enum::Terran_Barracks;
	this->buildOrder[2] = BWAPI::UnitTypes::Enum::Terran_Command_Center;

	this->miniralGoal[0] = 100;
	this->miniralGoal[1] = 150;
	this->miniralGoal[2] = 400;
	this->miniralGoal[3] = 2000;

	this->isCommandCenter = false;
}

StrategyBuild::~StrategyBuild()
{
	
}

bool StrategyBuild::getIsCommandCenter()
{
	return isCommandCenter;
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
	isCommandCenter = false;

	if (buildStage == 2)
	{
		isCommandCenter = true;
	}

}

int StrategyBuild::getBuildStage()
{
	return this->buildStage;
}


