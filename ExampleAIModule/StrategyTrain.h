#ifndef STRATEGYTRAIN_H
#define STRATEGYTRAIN_H
#include <BWAPI/UnitType.h>


const int amount = 5;

class StrategyTrain
{
	
public:
	StrategyTrain();
	~StrategyTrain();
	
	void trainedUnit();

	BWAPI::UnitType getUnitBuildning();
	BWAPI::UnitType getUnitOrder();

	bool canTankBuild();

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

	bool siegeTank;
};


#endif

