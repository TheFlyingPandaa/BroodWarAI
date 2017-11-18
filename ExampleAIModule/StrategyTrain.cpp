#include "StrategyTrain.h"

StrategyTrain::StrategyTrain()
{
	this->trainOrder = 0;

	unitBuildning[0] = BWAPI::UnitTypes::Enum::Terran_Command_Center;
	unitBuildning[1] = BWAPI::UnitTypes::Enum::Terran_Barracks;

	unitOrder[0] = BWAPI::UnitTypes::Enum::Terran_SCV;
	unitOrder[1] = BWAPI::UnitTypes::Enum::Terran_Marine;

	unitCostGoal[0] = 50;
	unitCostGoal[1] = 50;

	amountOfUnits[0] = 3;
	amountOfUnits[1] = 10;
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
	return trainOrder;
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

int StrategyTrain::getAmountOfUnits()
{
	return amountOfUnits[trainOrder];
}


