#include "ManageUnits.h"


ManageUnits::ManageUnits()
{
	
}


ManageUnits::~ManageUnits()
{
	
}

void ManageUnits::add(BWAPI::Unit u)
{
	units.insert(u);
}

void ManageUnits::remove(BWAPI::Unit u)
{
	//units.erase(std::remove(units.cbegin(), units.cend(), u), units.cend());
}

bool ManageUnits::exist(BWAPI::Unit u)
{
	return units.contains(u);
}

void ManageUnits::attack(BWAPI::Unit u)
{
	units.attack(u);
}
