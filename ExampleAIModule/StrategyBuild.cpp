#include "StrategyBuild.h"

StrategyBuild::StrategyBuild()
{
	this->buildStage = 0;

	this->buildOrder[0] = BWAPI::UnitTypes::Enum::Terran_Supply_Depot;
	this->buildOrder[1] = BWAPI::UnitTypes::Enum::Terran_Supply_Depot;
	this->buildOrder[2] = BWAPI::UnitTypes::Enum::Terran_Barracks;
	this->buildOrder[3] = BWAPI::UnitTypes::Enum::Terran_Refinery;
	this->buildOrder[4] = BWAPI::UnitTypes::Enum::Terran_Academy;
	this->buildOrder[5] = BWAPI::UnitTypes::Enum::Terran_Command_Center;
	this->buildOrder[6] = BWAPI::UnitTypes::Enum::Terran_Factory;
	this->buildOrder[7] = BWAPI::UnitTypes::Enum::Terran_Machine_Shop;
	this->buildOrder[8] = BWAPI::UnitTypes::Enum::Terran_Supply_Depot;
	this->buildOrder[9] = BWAPI::UnitTypes::Enum::Terran_Factory; //Som sagt Ha alltid 1 extra

	this->miniralGoal[0] = 100;	//Supply
	this->miniralGoal[1] = 100;	//Supply
	this->miniralGoal[2] = 150;	//Barrack
	this->miniralGoal[3] = 100; //Refinary
	this->miniralGoal[4] = 150; //Academy
	this->miniralGoal[5] = 400;	//Command Center
	this->miniralGoal[6] = 200; //Factory
	this->miniralGoal[7] = 50; //Factory
	this->miniralGoal[8] = 100; //Factory
	this->miniralGoal[9] = 20000; //Detta är något lösligt bara så att den inte ska crascha

	this->gasGoal[0] = -1;
	this->gasGoal[1] = -1;
	this->gasGoal[2] = -1;
	this->gasGoal[3] = -1;
	this->gasGoal[4] = -1;
	this->gasGoal[5] = -1;
	this->gasGoal[6] = 100;
	this->gasGoal[7] = 50;
	this->gasGoal[8] = -1;
	this->gasGoal[9] = 1000;

	for (int i = 0; i < buildings; i++)
		this->uBuild[i] = NULL;

	this->isCommandCenter = false;
	this->isRefinary = false;
	this->isBuildAddon = false;
}

StrategyBuild::~StrategyBuild()
{
	
}

bool StrategyBuild::canBuildSiege()
{
	if (buildStage >= 9)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool StrategyBuild::exists(BWAPI::Unit u) const{
	
	for (int i = 0; i < buildings; i++){
		if (this->uBuild[i] == u)
			return true;
	}
	return false;
}

void StrategyBuild::add(BWAPI::Unit u){

	this->uBuild[buildStage] = u;
}

bool StrategyBuild::getIsCommandCenter()
{
	return isCommandCenter;
}

bool StrategyBuild::getIsRefinary() const
{
	return isRefinary;
}

bool StrategyBuild::getIsBuildAddon() const
{
	return isBuildAddon;
}

BWAPI::UnitType StrategyBuild::getCurrentBuild()
{
	if (buildStage < buildings)
		return buildOrder[buildStage];
	else
		return NULL;
}

BWAPI::UnitType StrategyBuild::getBuildStage(int num)
{
	return buildOrder[num];
}

int StrategyBuild::getMiniralGoal()
{
	if (buildStage < buildings)
		return miniralGoal[buildStage];
	else
		return -1;
}

int StrategyBuild::getGasGoal(){
	return gasGoal[buildStage];
}


void StrategyBuild::buildingBuilt()
{
	this->buildStage++;
	isCommandCenter = false;
	isRefinary = false;
	isBuildAddon = false;

	if (buildOrder[buildStage] == BWAPI::UnitTypes::Enum::Terran_Command_Center)
	{
		isCommandCenter = true;
	}
	if (buildOrder[buildStage] == BWAPI::UnitTypes::Enum::Terran_Refinery)
	{
		this->isRefinary = true;
	}
	if (buildOrder[buildStage] == BWAPI::UnitTypes::Enum::Terran_Machine_Shop)
	{
		this->isBuildAddon = true;
	}
}

int StrategyBuild::getBuildStage()
{
	if (buildStage < buildings)
		return this->buildStage;
	else
		return -1;
}


