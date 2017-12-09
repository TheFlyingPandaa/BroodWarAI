#ifndef STRATEGYTRAIN_H
#define STRATEGYTRAIN_H
#include <BWAPI/UnitType.h>


const int amount = 4;

class StrategyTrain
{
	
public:
	StrategyTrain();
	~StrategyTrain();
	
	void trainedUnit();

	BWAPI::UnitType getUnitBuildning();
	BWAPI::UnitType getUnitOrder();

	int getUnitCostGoal();
	int getGasGoal();
	int getAmountOfUnits();

	int getTrainOrder();

private:

	int trainOrder;

	BWAPI::UnitType unitBuildning[amount];
	BWAPI::UnitType unitOrder[amount];
	int unitCostGoal[amount];
	int unitGasGoal[amount];
	int amountOfUnits[amount];

};


#endif

