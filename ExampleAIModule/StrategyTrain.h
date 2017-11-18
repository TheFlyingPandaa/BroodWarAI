#ifndef STRATEGYTRAIN_H
#define STRATEGYTRAIN_H
#include <BWAPI/UnitType.h>


const int amount = 2;

class StrategyTrain
{
	
public:
	StrategyTrain();
	~StrategyTrain();
	
	void trainedUnit();

	BWAPI::UnitType getUnitBuildning();
	BWAPI::UnitType getUnitOrder();

	int getUnitCostGoal();
	int getAmountOfUnits();

	int getTrainOrder();

private:

	int trainOrder;

	BWAPI::UnitType unitBuildning[amount];
	BWAPI::UnitType unitOrder[amount];
	int unitCostGoal[amount];
	int amountOfUnits[amount];

};


#endif

