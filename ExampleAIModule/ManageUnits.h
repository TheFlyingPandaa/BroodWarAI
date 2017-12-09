#pragma once
#include <BWAPI.h>
#include <algorithm>
class ManageUnits
{
public:
	ManageUnits();
	~ManageUnits();

	void add(BWAPI::Unit u);
	void remove(BWAPI::Unit u);

	bool exist(BWAPI::Unit u);

	void attack(BWAPI::Unit u);

private:
	BWAPI::Unitset units;

};

