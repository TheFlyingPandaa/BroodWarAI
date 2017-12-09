#include "StrategyTrain.h"

StrategyTrain::StrategyTrain()
{
	this->trainOrder = 0;

	unitBuildning[0] = BWAPI::UnitTypes::Enum::Terran_Command_Center;
	unitBuildning[1] = BWAPI::UnitTypes::Enum::Terran_Barracks;	
	unitBuildning[2] = BWAPI::UnitTypes::Enum::Terran_Barracks;
	unitBuildning[3] = BWAPI::UnitTypes::Enum::Terran_Command_Center;

	unitOrder[0] = BWAPI::UnitTypes::Enum::Terran_SCV;
	unitOrder[1] = BWAPI::UnitTypes::Enum::Terran_Marine;
	unitOrder[2] = BWAPI::UnitTypes::Enum::Terran_Medic;
	unitOrder[3] = BWAPI::UnitTypes::Enum::Terran_SCV;

	unitCostGoal[0] = 50;
	unitCostGoal[1] = 50;
	unitCostGoal[2] = 50;
	unitCostGoal[3] = 50;

	amountOfUnits[0] = 3;
	amountOfUnits[1] = 10;
	amountOfUnits[2] = 3;
	amountOfUnits[3] = 5;

	unitGasGoal[0] = -1;
	unitGasGoal[1] = -1;
	unitGasGoal[2] = 25;
	unitGasGoal[3] = -1;
}

StrategyTrain::~StrategyTrain()
{
	
}

void StrategyTrain::trainedUnit()
{

	amountOfUnits[trainOrder]--;

	if (amountOfUnits[trainOrder] < 0)
	{
		trainOrder++;
	}

}

int StrategyTrain::getTrainOrder()
{
	if (this->trainOrder < amount)
		return trainOrder;
	else
		return -1;
}


BWAPI::UnitType StrategyTrain::getUnitBuildning()
{
	return unitBuildning[trainOrder];
}

BWAPI::UnitType StrategyTrain::getUnitOrder()
{
	return unitOrder[trainOrder];
}

int StrategyTrain::getUnitCostGoal()
{
	return unitCostGoal[trainOrder];
}

int StrategyTrain::getGasGoal(){
	return unitGasGoal[trainOrder];
}

int StrategyTrain::getAmountOfUnits()
{
	return amountOfUnits[trainOrder];
}


