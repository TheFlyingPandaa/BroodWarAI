#include "ExampleAIModule.h" 
using namespace BWAPI;

bool analyzed;
bool analysis_just_finished;
BWTA::Region* home;
BWTA::Region* enemy_base;

BWAPI::Unit builderUnit;

TilePosition tempDraw;
UnitType tempUnit;
bool willDraw = false;
bool done = false;
bool donee = true;
int tempInt = 0;

bool jumpResearch = false;

bool building = NULL;

const double maxDistanceFormBase = 1024;
Position mainBase;

Position chokeHold;

bool attackEnemy = false;
Unit badGuy = NULL;

Unit supreamLeader = NULL; //KIM JONG UNNN YEAHHHH
int amountOfUnits = 0;
bool doOnce = true;
Position romePosition = Position(-1,-1);
Position visitedRoam[10];
int amountOfVisit = 0;

Unit secondBase = NULL;

int spawnUnits = 0;


//This is the startup method. It is called once
//when a new game has been started with the bot.
void ExampleAIModule::onStart()
{

	strategyBuild = StrategyBuild();
	strategyTrain = StrategyTrain();
	


	Broodwar->setLocalSpeed(0);	

	Broodwar->sendText("Hello world!");
	//Enable flags
	Broodwar->enableFlag(Flag::UserInput);
	//Uncomment to enable complete map information
	//Broodwar->enableFlag(Flag::CompleteMapInformation);
	
	
	
	this->amountOfWorkes = 5;

	//Start analyzing map data
	BWTA::readMap();
	analyzed = false;
	analysis_just_finished = false;
	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL); //Threaded version
	
	AnalyzeThread();

	//Send each worker to the mineral field that is closest to it
	sendWorkersToMinirals();

	mainBase = Broodwar->self()->getUnits().getPosition();

	chokeHold = findGuardPoint();
}

//Called when a game is ended.
//No need to change this.
void ExampleAIModule::onEnd(bool isWinner)
{
	if (isWinner)
	{
		Broodwar->sendText("I won!");
	}
}

//Finds a guard point around the home base.
//A guard point is the center of a chokepoint surrounding
//the region containing the home base.
Position ExampleAIModule::findGuardPoint()
{
	//Get the chokepoints linked to our home region
	std::set<BWTA::Chokepoint*> chokepoints = home->getChokepoints();
	double min_length = 10000;
	BWTA::Chokepoint* choke = NULL;
	Position co;
	//Iterate through all chokepoints and look for the one with the smallest gap (least width)
	for (std::set<BWTA::Chokepoint*>::iterator c = chokepoints.begin(); c != chokepoints.end(); c++)
	{
		double length = (*c)->getWidth();
		if (length < min_length || choke == NULL)
		{
			min_length = length;
			choke = *c;
		}
	}
	co = choke->getCenter();
	co.x += 60; //Why this. Man kan spawna på 2 pos. #tttrrraiilllleleleeeeeand errroorororor
	co.y += 60;
	return co;
}

void ExampleAIModule::sendWorkersToMinirals()
{
	for (auto u : Broodwar->self()->getUnits())
	{
		if (u->getType().isWorker()) //&& u->getType() != builderUnit)
		{
			Unit closestMineral = NULL;
			for (auto m : Broodwar->getMinerals())
			{
				if (closestMineral == NULL || u->getDistance(m) < u->getDistance(closestMineral))
				{
					closestMineral = m;
				}
			}
			if (closestMineral != NULL)
			{
				u->rightClick(closestMineral);
				Broodwar->printf("Send worker %d to mineral %d", u->getID(), closestMineral->getID());
			}
		}
	}
}

void ExampleAIModule::buildCommandCenter()
{

	TilePosition startPos;
	for (Unit c : Broodwar->self()->getUnits())
	{
		if (c->getType() == BWAPI::UnitTypes::Enum::Terran_Command_Center)
		{
			startPos = c->getTilePosition();
		}
	}

	TilePosition cPos;
	for (auto center : BWTA::getBaseLocations())
	{
		if (center->isStartLocation())
		{
			startPos = center->getTilePosition();
			cPos.x = 2000000;
			cPos.y = 2000000;
		}
	}

	for (auto center : BWTA::getBaseLocations())
	{
		if (startPos.getDistance(center->getTilePosition()) < startPos.getDistance(cPos))
		{
			if (startPos.getDistance(startPos) != startPos.getDistance(center->getTilePosition()))
			{
				cPos = center->getTilePosition();
			}
		}
	}

	for (auto JIM : Broodwar->self()->getUnits() )
	{
		if (JIM->getType().isWorker() && (JIM->isGatheringMinerals() || JIM->isIdle()))
		{
			UnitType type = BWAPI::UnitTypes::Enum::Terran_Command_Center;

			Broodwar->drawBox(CoordinateType::Map, cPos.x * 32, cPos.y * 32, cPos.x * 32 + 4 * 32, cPos.y * 32 + 3 * 32, Colors::Red, false);

			builderUnit = JIM;

			tempUnit = type;
			tempDraw = cPos;
			building = true;

			Broodwar->printf("Build stage %d", strategyBuild.getBuildStage());
						
			break;
		}
	}

}

void ExampleAIModule::buildAddonBuilding()
{
	for (auto unit : Broodwar->self()->getUnits())
	{
		if (strategyBuild.getBuildStage() == 7)
		{
			if (unit->getType() == UnitTypes::Enum::Terran_Factory)
			{
				unit->buildAddon(strategyBuild.getCurrentBuild());
				
				tempUnit = strategyBuild.getCurrentBuild();
				building = true;

				//strategyBuild.buildingBuilt();
			}
		}
	}
}

void ExampleAIModule::researchSiegeMode()
{

	for (auto u : Broodwar->self()->getUnits())
	{
		if (u->getType() == UnitTypes::Enum::Terran_Machine_Shop)
		{
			Broodwar->printf("%d", u->getRemainingBuildTime());
			if (u->getRemainingBuildTime() == 0)
			{
				if (Broodwar->self()->gas() >= 150)
				{
					u->research(TechTypes::Enum::Tank_Siege_Mode);
					jumpResearch = true;
				}

			}
		}
	}
}

void ExampleAIModule::setRallyPoint()
{
	//if (strategyBuild.getBuildStage(strategyBuild.getBuildStage() - 1) == UnitTypes::Enum::Terran_Barracks)
	//{
		for (auto unit : Broodwar->self()->getUnits())
		{
			if (unit->getType() == UnitTypes::Enum::Terran_Barracks)
			{
				unit->setRallyPoint(chokeHold);
			}
			if (unit->getType() == UnitTypes::Enum::Terran_Factory)
			{
				unit->setRallyPoint(chokeHold);
			}
		}
	//}
	
}

void ExampleAIModule::unitAction()
{
	Unit tempUni = NULL;
	if (badGuy != NULL)
	{
		if (badGuy->getHitPoints() < 1)
		{	
			for (auto unit : Broodwar->getAllUnits())
			{
				if (unit->getPlayer() != Broodwar->self())
				{
					if (unit->getResources() < 1)
					{
						tempUni = unit;
						badGuy = unit;
						attackEnemy = true;
						break;
					}
					else
					{
						attackEnemy = false;
					}

				}
			}
		}
	}
	else
	{
			for (auto unit : Broodwar->getAllUnits())
			{
				if (unit->getPlayer() != Broodwar->self())
				{
					if (unit->getResources() < 1)
					{
		
						Broodwar->setLocalSpeed(1);
						tempUni = unit;
						attackEnemy = true;
						if (badGuy != NULL)
						{
						
						}
						else
						{
							badGuy = unit;
						}
					}
					else
					{
						attackEnemy = false;
					}

				}
			}
		
	}

	if (attackEnemy == true)
	{
		
			for (auto unit : Broodwar->self()->getUnits())
			{
				if (unit->getResources() < 1)
				{
					if (unit->getType().isWorker() == false && unit->getType().isBuilding() == false)
					{
						if (unit->getDistance(badGuy->getPosition()) > 500)
						{
							Broodwar->drawCircle(CoordinateType::Map, unit->getPosition().x, unit->getPosition().y, 15, Colors::Purple, false);
							
							if (unit->canSiege())
							{
								if (unit->isSieged() == true)
								{
									unit->siege();
								}
								else
								{
									if (unit->isAttackFrame() == false)
									{
										unit->move(badGuy->getPosition());

									}
								}

							}
							else
							{
								if (unit->isAttackFrame() == false)
								{
									unit->move(badGuy->getPosition());
								}
							}
							doOnce = true;
						}
						else
						{
							if (doOnce == true)
							{
								if (unit->canSiege())
								{
									if (unit->isSieged() == false)
									{
										unit->siege();
									}
									else
									{
										unit->move(unit->getPosition());
										//unit->holdPosition();
										unit->attack(badGuy);
										doOnce = false;
									}

								}
								else
								{
									Broodwar->printf("FUCKING");
									//unit->attack(badGuy);
									unit->move(unit->getPosition());
									//unit->holdPosition();
									if (unit->isAttackFrame() == false)
									{
										unit->attack(badGuy);
									}
									//unit->attack(badGuy);
									doOnce = false;
								}
							}
						}
					}
				}
			}
		
	}
	else
	{
		for (auto unit : Broodwar->self()->getUnits())
		{
			if (unit->isAttackFrame() == false)
			{
				if (unit->getResources() < 1)
				{
					if (unit->getType().isWorker() == false && unit->getType().isBuilding() == false)
					{
						Broodwar->drawCircle(CoordinateType::Map, unit->getPosition().x, unit->getPosition().y, 15, Colors::Blue, false);

						if (amountOfUnits >= 9)
						{
							unitRoaming();
							unit->move(romePosition);
						}

						//unit->move(Position(unit->getPosition().x + 1, unit->getPosition().y));
						//unit->stop();
					}
				}
			}
		}
	}

}

void ExampleAIModule::checkUnits()
{
	amountOfUnits = 0;
	for (auto unit : Broodwar->self()->getUnits())
	{
		if (unit->getPlayer() == Broodwar->self())
		{
			if (unit->getResources() < 1)
			{
				if (unit->getType().isWorker() == false)
				{
					if (unit->getType().isBuilding() == false)
					{
						amountOfUnits++;
					}
				}
			}
		}
	}

}

void ExampleAIModule::unitRoaming()
{
	if (romePosition.x == -1 && romePosition.y == -1)
	{
		for (auto base : BWTA::getBaseLocations())
		{
			if (mainBase.getDistance(base->getPosition()) > 100) 
			{
				romePosition = base->getPosition();
				break;
			}
		}
	}

	bool tempBool = true;

	if (supreamLeader->getDistance(romePosition) < 100)
	{
		visitedRoam[amountOfVisit] = romePosition;
		amountOfVisit++;
		if (amountOfVisit >= 10)
		{
			for (size_t i = 0; i < 10; i++) {
				visitedRoam[i] = Position(1, 1);
			}
			amountOfVisit = 0;
		}

		for (auto base : BWTA::getBaseLocations())
		{
			if (mainBase.getDistance(base->getPosition()) > 100)
			{

				for (size_t i = 0; i < 10; i++)
				{
					if (visitedRoam[i] == base->getPosition())
					{
						tempBool = false;
					}
				}
				if (tempBool == true)
				{
					romePosition = base->getPosition();
				}
				else
				{
					tempBool = true;
				}
			}
		}

	}


}



//This is the method called each frame. This is where the bot's logic
//shall be called.
void ExampleAIModule::onFrame()
{

	for (auto unit : Broodwar->self()->getUnits())
	{
		if (unit->getPlayer() == Broodwar->self())
		{
			if (unit->getResources() < 1)
			{
				if (unit->getType().isWorker() == false)
				{
					if (unit->getType().isBuilding() == false)
					{
						supreamLeader = unit;
						break;
					}
				}
			}
		}
	}

	
	//buildCommandCenter();
	//Call every 100:th frame

	//draws the main base position (ish)
	Broodwar->drawCircle(CoordinateType::Map, mainBase.x, mainBase.y, 10, Colors::Green, true);
	drawTerrainData();

	if (Broodwar->getFrameCount() % 5 == 0)
	{
		unitAction();
	}
	

	if (Broodwar->getFrameCount() % 100 == 0)
	{

		if (secondBase != NULL)
		{
			if (spawnUnits <= 4)
			{
				secondBase->train(UnitTypes::Enum::Terran_SCV);
				spawnUnits++;
			}
		}

		checkUnits();
		Broodwar->printf("How meny units %d", amountOfUnits);

		//For the SigeTanks
		if (jumpResearch == false)
		{
			researchSiegeMode();
		}
		


		donee = strategyBuild.canBuildSiege();
		//Broodwar->printf("%d", donee);
		//Broodwar->printf("%d", strategyTrain.getTrainOrder());
		//_____________________________________________________________________________________________________________
		if (Broodwar->self()->minerals() >= strategyTrain.getUnitCostGoal() &&
			Broodwar->self()->gas() >= strategyTrain.getGasGoal() &&
			strategyTrain.getTrainOrder() != -1)
		{
			
			if (strategyTrain.getTrainOrder() <= 1 || strategyTrain.getTrainOrder() >= 3 || donee)
			{
				Unit ubuild = NULL;
				for (auto b : Broodwar->self()->getUnits())
				{
					//WHY BE LIKE THE REST. NAHH PUT 0 AS FINNISHED EVER HEARD OF -1
					if (b->getType() == strategyTrain.getUnitBuildning() &&
						b->getRemainingBuildTime() < 1) {
						if (ubuild == NULL)
							ubuild = b;
						/*else if (b->getPosition().getDistance(mainBase) < ubuild->getPosition().getDistance(mainBase))
							ubuild = b;*/
					}
				}
				if (ubuild != NULL) {
					UnitType::list queueCheck = ubuild->getTrainingQueue();
					if (queueCheck.size() <= 4)
					{
						ubuild->train(strategyTrain.getUnitOrder());
						Broodwar->printf("Amount Of units left %d", strategyTrain.getAmountOfUnits());
						Broodwar->printf("current Stage %d", strategyTrain.getTrainOrder());
						strategyTrain.trainedUnit();
					}
				}
			}
		}
		
	
		//_____________________________________________________________________________________________________________
		if (Broodwar->self()->minerals() >= strategyBuild.getMiniralGoal() &&
			strategyBuild.getMiniralGoal() != -1 /*&& strategyBuild.getBuildStage() != NULL*/ &&
			Broodwar->self()->gas() >= strategyBuild.getGasGoal() &&
			(!building || building == NULL) &&
			strategyBuild.getIsBuildAddon()  != true)
		{
			
			
			
			UnitType type = strategyBuild.getCurrentBuild();
			TilePosition destPos;
			if (strategyBuild.getIsCommandCenter())
			{
				buildCommandCenter();
			}
			else {
				Unit JIM = NULL;
				for (auto unit : Broodwar->self()->getUnits())
				{
					if (unit->getType().isWorker())
						JIM = unit;
					if (unit->getType().isWorker() && 
						unit->getPosition().getDistance(mainBase) < JIM->getPosition().getDistance(mainBase) &&
						(unit->isGatheringMinerals() || unit->isIdle()) &&
						!unit->isGatheringGas())
						JIM = unit;
				}
				destPos = JIM->getTilePosition();
				TilePosition buildPos = Broodwar->getBuildLocation(type, destPos, 63, false);

				Broodwar->drawBox(CoordinateType::Map, buildPos.x * 32, buildPos.y * 32, buildPos.x * 32 + 4 * 32, buildPos.y * 32 + 3 * 32, Colors::Red, false);

				Broodwar->printf("Build stage %d", strategyBuild.getBuildStage());

				builderUnit = JIM;
				tempUnit = strategyBuild.getCurrentBuild();

				tempDraw = buildPos;
				//willDraw = true;

				building = true;
			}
		}
		else
		{
			if (Broodwar->self()->minerals() >= strategyBuild.getMiniralGoal() &&
				strategyBuild.getMiniralGoal() != -1 &&
				Broodwar->self()->gas() >= strategyBuild.getGasGoal())
			{
				if (strategyBuild.getIsBuildAddon())
				{
					buildAddonBuilding();
				}
			}
		}
		//Fucking katter går på skrivbordet and shit
		//oiiiijvgf vfvfgw32w23 Death to all humans aa3sd6hgfnvbm  fghoiifgh
		int refWorkers = 0;
		for (Unit ref : Broodwar->self()->getUnits()) {
			if (ref->getType() == UnitTypes::Enum::Terran_Refinery) {
				//____________________________________________________________
				for (Unit w : Broodwar->self()->getUnits()) {
					if (w->getType().isWorker() && w->isGatheringGas()) {
						refWorkers++;
						if (refWorkers >= 2)
							break;
					}
				}
				//____________________________________________________________
				if (refWorkers < 2) {
					for (Unit w : Broodwar->self()->getUnits()) {
						if (w->getType().isWorker() && !w->isGatheringGas()) {
							w->rightClick(ref);
							refWorkers++;
							if (refWorkers >= 2)
								break;
						}
					}
				}
			}
		}
	}

	if (strategyBuild.getBuildStage() >= 8)
	{
		Broodwar->setLocalSpeed(1);
	}

	for (auto unit : Broodwar->getAllUnits())
	{
		if (unit->getPlayer() != Broodwar->self())
		{
			if (unit->getResources() < 1)
			{
				Broodwar->drawCircle(CoordinateType::Map, unit->getPosition().x, unit->getPosition().y, 15, Colors::Red, false);
			}
			
		}
	}

	if (building && builderUnit != NULL && tempUnit != NULL){
		if (buildBuilding(builderUnit, tempUnit, tempDraw)){
			building = false;


			if (strategyBuild.getIsRefinary())
				this->sendWorkerToTheRefinery(builderUnit);
			else
				this->sendWorkerToMinirals(builderUnit);

			for (auto b : Broodwar->self()->getUnits()){
				if (b->getType() == strategyBuild.getCurrentBuild()){
					if (!strategyBuild.exists(b) && b->getPosition().getDistance(mainBase) > 64){
						strategyBuild.add(b);
						strategyBuild.buildingBuilt();
						Broodwar->printf("Build stage %i Complete", strategyBuild.getBuildStage());
						if (b->getType() == UnitTypes::Enum::Terran_Command_Center)
						{
							secondBase = b;
						}
						setRallyPoint();
					}
				}
			}
			//strategyBuild.buildingBuilt();
		}
	}

}

bool ExampleAIModule::buildBuilding(Unit worker, UnitType building, TilePosition position){

	bool dest = false;
	
	Broodwar->drawBox(CoordinateType::Map, position.x * 32, position.y * 32, position.x * 32 + 4 * 32, position.y * 32 + 3 * 32, Colors::Red, false);
	
	if (!dest)
	{
		if (worker->getTilePosition().getDistance(position) < 10)		
			dest = true;
		if (!dest){
			worker->move(Position(position.x * 32, position.y * 32));
			return false;
		}
	}

	if (dest)
	{	
		if (worker->build(building, position))						
			return false;
		if (worker->isIdle() || worker->isGatheringGas() || worker == NULL)
			return true;	
	}	
	return false;
	
}

void ExampleAIModule::sendWorkerToMinirals(Unit worker){
	Unit closestMineral = NULL;
	for (auto m : Broodwar->getMinerals())
	{
		if (closestMineral == NULL || builderUnit->getDistance(m) < builderUnit->getDistance(closestMineral))
		{
			closestMineral = m;
		}
	}
	if (closestMineral != NULL)
	{
		worker->rightClick(closestMineral);
	}
}

void ExampleAIModule::sendWorkerToTheRefinery(Unit worker){
	Unit ref = NULL;
	for (auto b : Broodwar->self()->getUnits())
	{
		if (b->getType() == UnitTypes::Enum::Terran_Refinery)
			if (ref == NULL || (b->getPosition().getDistance(worker->getPosition()) < ref->getPosition().getDistance(worker->getPosition())))
				ref = b;
	}
	worker->rightClick(ref);
}

//Is called when text is written in the console window.
//Can be used to toggle stuff on and off.
void ExampleAIModule::onSendText(std::string text)
{
	if (text == "/show players")
	{
		showPlayers();
	}
	else if (text == "/show forces")
	{
		showForces();
	}
	else
	{
		Broodwar->printf("You typed '%s'!", text.c_str());
		Broodwar->sendText("%s", text.c_str());
	}
}

//Called when the opponent sends text messages.
//No need to change this.
void ExampleAIModule::onReceiveText(BWAPI::Player player, std::string text)
{
	Broodwar->printf("%s said '%s'", player->getName().c_str(), text.c_str());
	
}

//Called when a player leaves the game.
//No need to change this.
void ExampleAIModule::onPlayerLeft(BWAPI::Player player)
{
	Broodwar->sendText("%s left the game.", player->getName().c_str());
}

//Called when a nuclear launch is detected.
//No need to change this.
void ExampleAIModule::onNukeDetect(BWAPI::Position target)
{
	if (target != Positions::Unknown)
	{
		Broodwar->printf("Nuclear Launch Detected at (%d,%d)", target.x, target.y);
	}
	else
	{
		Broodwar->printf("Nuclear Launch Detected");
	}
}

//No need to change this.
void ExampleAIModule::onUnitDiscover(BWAPI::Unit unit)
{
	//Broodwar->sendText("A %s [%x] has been discovered at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x,unit->getPosition().y);
}

//No need to change this.
void ExampleAIModule::onUnitEvade(BWAPI::Unit unit)
{
	//Broodwar->sendText("A %s [%x] was last accessible at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x,unit->getPosition().y);
}

//No need to change this.
void ExampleAIModule::onUnitShow(BWAPI::Unit unit)
{
	//Broodwar->sendText("A %s [%x] has been spotted at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x,unit->getPosition().y);
}

//No need to change this.
void ExampleAIModule::onUnitHide(BWAPI::Unit unit)
{
	//Broodwar->sendText("A %s [%x] was last seen at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x,unit->getPosition().y);
}

//Called when a new unit has been created.
//Note: The event is called when the new unit is built, not when it
//has been finished.
void ExampleAIModule::onUnitCreate(BWAPI::Unit unit)
{
	if (unit->getPlayer() == Broodwar->self())
	{
		Broodwar->sendText("A %s [%x] has been created at (%d,%d)", unit->getType().getName().c_str(), unit, unit->getPosition().x, unit->getPosition().y);
	}
	/*
	for (auto u : Broodwar->self()->getUnits())
	{
		//Check if unit is a Marine
		if (u->getType() == UnitTypes::Enum::Terran_Marine)
		{
			//Find guard point
			Position guardPoint = findGuardPoint();
			//Order the worker to move to the guard point
			u->rightClick(guardPoint);
		}
	}
	*/
	for (auto unit : Broodwar->self()->getUnits())
	{
		if (unit->isIdle())
		{
			if (unit->getType().isWorker()) //&& unit->getType() != builderUnit)
			{
				Unit closestMineral = NULL;
				for (auto m : Broodwar->getMinerals())
				{
					if (closestMineral == NULL || unit->getDistance(m) < unit->getDistance(closestMineral))
					{
						closestMineral = m;
					}
				}
				if (closestMineral != NULL)
				{
					unit->rightClick(closestMineral);
					//Broodwar->printf("Send worker %d to mineral %d", unit->getID(), closestMineral->getID());
					break;
				}
			}
		}
	}
}

//Called when a unit has been destroyed.
void ExampleAIModule::onUnitDestroy(BWAPI::Unit unit)
{
	if (unit->getPlayer() == Broodwar->self())
	{
		Broodwar->sendText("My unit %s [%x] has been destroyed at (%d,%d)", unit->getType().getName().c_str(), unit, unit->getPosition().x, unit->getPosition().y);
	}
	else
	{
		Broodwar->sendText("Enemy unit %s [%x] has been destroyed at (%d,%d)", unit->getType().getName().c_str(), unit, unit->getPosition().x, unit->getPosition().y);
	}
}

//Only needed for Zerg units.
//No need to change this.
void ExampleAIModule::onUnitMorph(BWAPI::Unit unit)
{
	//Broodwar->sendText("A %s [%x] has been morphed at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x,unit->getPosition().y);
}

//No need to change this.
void ExampleAIModule::onUnitRenegade(BWAPI::Unit unit)
{
	//Broodwar->sendText("A %s [%x] is now owned by %s",unit->getType().getName().c_str(),unit,unit->getPlayer()->getName().c_str());
}

//No need to change this.
void ExampleAIModule::onSaveGame(std::string gameName)
{
	Broodwar->printf("The game was saved to \"%s\".", gameName.c_str());
}

//Analyzes the map.
//No need to change this.
DWORD WINAPI AnalyzeThread()
{
	BWTA::analyze();

	//Self start location only available if the map has base locations
	if (BWTA::getStartLocation(BWAPI::Broodwar->self()) != NULL)
	{
		home = BWTA::getStartLocation(BWAPI::Broodwar->self())->getRegion();
	}
	//Enemy start location only available if Complete Map Information is enabled.
	if (BWTA::getStartLocation(BWAPI::Broodwar->enemy()) != NULL)
	{
		enemy_base = BWTA::getStartLocation(BWAPI::Broodwar->enemy())->getRegion();
	}
	analyzed = true;
	analysis_just_finished = true;
	return 0;
}

//Prints some stats about the units the player has.
//No need to change this.
void ExampleAIModule::drawStats()
{
	BWAPI::Unitset myUnits = Broodwar->self()->getUnits();
	Broodwar->drawTextScreen(5, 0, "I have %d units:", myUnits.size());
	std::map<UnitType, int> unitTypeCounts;
	for (auto u : myUnits)
	{
		if (unitTypeCounts.find(u->getType()) == unitTypeCounts.end())
		{
			unitTypeCounts.insert(std::make_pair(u->getType(), 0));
		}
		unitTypeCounts.find(u->getType())->second++;
	}
	int line = 1;
	for (std::map<UnitType, int>::iterator i = unitTypeCounts.begin(); i != unitTypeCounts.end(); i++)
	{
		Broodwar->drawTextScreen(5, 16 * line, "- %d %ss", i->second, i->first.getName().c_str());
		line++;
	}
}

//Draws terrain data aroung regions and chokepoints.
//No need to change this.
void ExampleAIModule::drawTerrainData()
{
	//Iterate through all the base locations, and draw their outlines.
	for (auto bl : BWTA::getBaseLocations())
	{
		TilePosition p = bl->getTilePosition();
		Position c = bl->getPosition();
		//Draw outline of center location
		//Broodwar->drawBox(CoordinateType::Map, p.x * 32, p.y * 32, p.x * 32 + 4 * 32, p.y * 32 + 3 * 32, Colors::Blue, false);
		//Draw a circle at each mineral patch
		for (auto m : bl->getStaticMinerals())
		{
			Position q = m->getInitialPosition();
			Broodwar->drawCircle(CoordinateType::Map, q.x, q.y, 30, Colors::Cyan, false);
		}
		//Draw the outlines of vespene geysers
		for (auto v : bl->getGeysers())
		{
			TilePosition q = v->getInitialTilePosition();
			Broodwar->drawBox(CoordinateType::Map, q.x * 32, q.y * 32, q.x * 32 + 4 * 32, q.y * 32 + 2 * 32, Colors::Orange, false);
		}
		//If this is an island expansion, draw a yellow circle around the base location
		if (bl->isIsland())
		{
			Broodwar->drawCircle(CoordinateType::Map, c.x, c.y, 80, Colors::Yellow, false);
		}
	}
	//Iterate through all the regions and draw the polygon outline of it in green.
	for (auto r : BWTA::getRegions())
	{
		BWTA::Polygon p = r->getPolygon();
		for (int j = 0; j<(int)p.size(); j++)
		{
			Position point1 = p[j];
			Position point2 = p[(j + 1) % p.size()];
			Broodwar->drawLine(CoordinateType::Map, point1.x, point1.y, point2.x, point2.y, Colors::Green);
		}
	}
	//Visualize the chokepoints with red lines
	for (auto r : BWTA::getRegions())
	{
		for (auto c : r->getChokepoints())
		{
			Position point1 = c->getSides().first;
			Position point2 = c->getSides().second;
			Broodwar->drawLine(CoordinateType::Map, point1.x, point1.y, point2.x, point2.y, Colors::Red);
		}
	}
}

//Show player information.
//No need to change this.
void ExampleAIModule::showPlayers()
{
	for (auto p : Broodwar->getPlayers())
	{
		Broodwar->printf("Player [%d]: %s is in force: %s", p->getID(), p->getName().c_str(), p->getForce()->getName().c_str());
	}
}

//Show forces information.
//No need to change this.
void ExampleAIModule::showForces()
{
	for (auto f : Broodwar->getForces())
	{
		BWAPI::Playerset players = f->getPlayers();
		Broodwar->printf("Force %s has the following players:", f->getName().c_str());
		for (auto p : players)
		{
			Broodwar->printf("  - Player [%d]: %s", p->getID(), p->getName().c_str());
		}
	}
}

//Called when a unit has been completed, i.e. finished built.
void ExampleAIModule::onUnitComplete(BWAPI::Unit unit)
{
	Broodwar->sendText("A %s [%x] has been completed at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x,unit->getPosition().y);

	

	for (auto unit : Broodwar->self()->getUnits())
	{
		if (unit->isIdle())
		{
			if (unit->getType().isWorker())// && unit->getType() != builderUnit)
			{
				Unit closestMineral = NULL;
				for (auto m : Broodwar->getMinerals())
				{
					if (closestMineral == NULL || unit->getDistance(m) < unit->getDistance(closestMineral))
					{
						closestMineral = m;
					}
				}
				if (closestMineral != NULL)
				{
					unit->rightClick(closestMineral);
					//Broodwar->printf("Send worker %d to mineral %d", unit->getID(), closestMineral->getID());
					break;
				}
			}
		}
	}
}
