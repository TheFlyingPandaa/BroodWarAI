#include "StrategyBuild.h"

StrategyBuild::StrategyBuild()
{
	this->buildStage = 0;

	this->buildOrder[0] = BWAPI::UnitTypes::Enum::Terran_Supply_Depot;
	this->buildOrder[1] = BWAPI::UnitTypes::Enum::Terran_Supply_Depot;
	this->buildOrder[2] = BWAPI::UnitTypes::Enum::Terran_Barracks;
	this->buildOrder[3] = BWAPI::UnitTypes::Enum::Terran_Command_Center;
	this->buildOrder[4] = BWAPI::UnitTypes::Enum::Terran_Academy;
	this->buildOrder[5] = BWAPI::UnitTypes::Enum::Terran_Refinery;

	this->miniralGoal[0] = 100;	//Supply
	this->miniralGoal[1] = 100;	//Supply
	this->miniralGoal[2] = 150;	//Barrack
	this->miniralGoal[3] = 400;	//Command Center
	this->miniralGoal[4] = 150; //Academy
	this->miniralGoal[4] = 100; //Refinary

	this->isCommandCenter = false;
	this->isRefinary = false;
}

StrategyBuild::~StrategyBuild()
{
	
}

bool StrategyBuild::getIsCommandCenter()
{
	return isCommandCenter;
}

bool StrategyBuild::getIsRefinary() const
{
	return isRefinary;
}

BWAPI::UnitType StrategyBuild::getCurrentBuild()
{
	if (buildStage < buildings)
		return buildOrder[buildStage];
	else
		return NULL;
}

int StrategyBuild::getMiniralGoal()
{
	if (buildStage < buildings)
		return miniralGoal[buildStage];
	else
		return -1;
}


void StrategyBuild::buildingBuilt()
{
	this->buildStage++;
	isCommandCenter = false;
	isRefinary = false;

	if (buildStage == 3)
	{
		isCommandCenter = true;
	}
	if (buildStage == 5)
		this->isRefinary = true;

}

int StrategyBuild::getBuildStage()
{
	if (buildStage < buildings)
		return this->buildStage;
	else
		return -1;
}


