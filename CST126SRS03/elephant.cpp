#include "stdafx.h"
#include "gps.h"
#include "preserve.h"
#include "elephant.h"
#include <iostream>

using std::cout;
using std::endl;

// *************** For diagnostics only - to be removed ***************

struct Brain
{
	int currentLatitude;
	int currentLongitude;
	int directionOfHerd;

	Direction currentDirection;
	int lastDirectionOfHerd{ -1 };
	int secondToLastDirectionOfHerd{ -1 };

	Preserve::Feature featureUnderMe;
	Preserve::Feature featureInFrontOfMe;
	Preserve::Feature featureLeftOfMe;
	Preserve::Feature featureRightOfMe;
	Preserve::Feature featureBehindMe;

	bool isHungry;
	bool isThirsty;
	bool isSleepy;
	unsigned water;
	unsigned weight;

	bool waterNearMe;
	bool brushNearMe;
	bool grassNearMe;

	bool obstacleInFrontOfMe;
	bool obstacleLeftOfMe;
	bool obstacleRightOfMe;
	bool obstacleBehindMe;
};

struct DirectionToHead
{
	bool keepHeadingForward;
	bool turnLeft;
	bool turnRight;
	bool uTurn;
};

char* getStringDirection(int direction);
char* getStringFeature(Preserve::Feature feature);
void displayDiagnostics(Brain& memory);
void determineObstacles(Brain& memory);
bool determineIfCanMoveInTheDirectionWeWantTo(int directionWeWantToHead, Brain &memory, DirectionToHead &directionToHead);
Preserve::Feature lookBehindMe(GPS* gps, Direction heading);
bool isWaterNearMe(Brain& memory);
bool isBrushNearMe(Brain& memory);
bool isGrassNearMe(Brain& memory);

char directionNorth[] = "North";
char directionEast[] = "East";
char directionWest[] = "West";
char directionSouth[] = "South";
char directionNotSet[] = "not set";

char featureHerd[] = "Herd";
char featureDirt[] = "Dirt";
char featureRock[] = "Rock";
char featureBrush[] = "Brush";
char featureGrass[] = "Grass";
char featureWater[] = "Water";

void displayDiagnostics(Brain& memory)
{
	cout << "------- Current Info --------" << endl;
	cout << endl;
	cout << "\t" << "Current Latitude: " << memory.currentLatitude << endl;
	cout << "\t" << "Current Longitude: " << memory.currentLongitude << endl;
	cout << "\t" << "Current Direction: " << getStringDirection(memory.currentDirection) << endl;
	cout << "\t" << "Direction of herd: " << getStringDirection(memory.directionOfHerd) << endl;
	cout << "\t" << "\t" << "Last direction of herd: " << getStringDirection(memory.lastDirectionOfHerd) << endl;
	cout << "\t" << "\t" << "Second to last direction of herd: " << getStringDirection(memory.secondToLastDirectionOfHerd) << endl;
	cout << endl;
	cout << "\t" << "Feature under me: " << getStringFeature(memory.featureUnderMe) << endl;
	cout << "\t" << "Feature in front of me: " << getStringFeature(memory.featureInFrontOfMe) << endl;
	cout << "\t" << "Feature left of me: " << getStringFeature(memory.featureLeftOfMe) << endl;
	cout << "\t" << "Feature right of me: " << getStringFeature(memory.featureRightOfMe) << endl;
	cout << "\t" << "Feature behind me: " << getStringFeature(memory.featureBehindMe) << endl;
	cout << endl;
	cout << "\t" << "is the elephant hungry? " << memory.isHungry << endl;
	cout << "\t" << "\t" << "Weight: " << memory.weight << endl;
	cout << "\t" << "is brush near me? " << memory.brushNearMe << endl;
	cout << "\t" << "is grass near me? " << memory.grassNearMe << endl;
	cout << endl;
	cout << "\t" << "is the elephant thirsty? " << memory.isThirsty << endl;
	cout << "\t" << "\t" << "Water level: " << memory.water << endl;
	cout << "\t" << "is water near me? " << memory.waterNearMe << endl;
	cout << endl;
	cout << "\t" << "is the elephant sleepy? " << memory.isSleepy << endl;
	cout << endl;
	cout << "\t" << "Obstacle in front of me: " << memory.obstacleInFrontOfMe << endl;
	cout << "\t" << "Obstacle left of me: " << memory.obstacleLeftOfMe << endl;
	cout << "\t" << "Obstacle right of me: " << memory.obstacleRightOfMe << endl;
	cout << "\t" << "Obstacle behind me: " << memory.obstacleBehindMe << endl;
}

char* getStringDirection(int direction)
{
	switch (direction)
	{
	case -1: return directionNotSet;
	case 0: return directionNorth;
	case 90: return directionEast;
	case 180: return directionSouth;
	case 270: return directionWest;
	}
	return nullptr;
}

char* getStringFeature(Preserve::Feature feature)
{
	switch (feature)
	{
	case Preserve::Feature::kHerd: return featureHerd;
	case Preserve::Feature::kDirt: return featureDirt;
	case Preserve::Feature::kRock: return featureRock;
	case Preserve::Feature::kBrush: return featureBrush;
	case Preserve::Feature::kGrass: return featureGrass;
	case Preserve::Feature::kWater: return featureWater;
	}
	return nullptr;
}

void determineObstacles(Brain& memory)
{
	memory.obstacleInFrontOfMe = false;
	memory.obstacleRightOfMe = false;
	memory.obstacleLeftOfMe = false;
	memory.obstacleBehindMe = false;

	switch (memory.featureInFrontOfMe)
	{
	case Preserve::Feature::kRock:
	case Preserve::Feature::kBrush:
		memory.obstacleInFrontOfMe = true;
		break;
	}

	switch (memory.featureRightOfMe)
	{
	case Preserve::Feature::kRock:
	case Preserve::Feature::kBrush:
		memory.obstacleRightOfMe = true;
		break;
	}

	switch (memory.featureLeftOfMe)
	{
	case Preserve::Feature::kRock:
	case Preserve::Feature::kBrush:
		memory.obstacleLeftOfMe = true;
		break;
	}

	switch (memory.featureBehindMe)
	{
	case Preserve::Feature::kRock:
	case Preserve::Feature::kBrush:
		memory.obstacleBehindMe = true;
		break;
	}
}

bool isWaterNearMe(Brain& memory)
{
	return memory.featureInFrontOfMe == Preserve::Feature::kWater ||
		memory.featureRightOfMe == Preserve::Feature::kWater ||
		memory.featureLeftOfMe == Preserve::Feature::kWater;
}

bool isBrushNearMe(Brain& memory)
{
	return memory.featureInFrontOfMe == Preserve::Feature::kBrush ||
		memory.featureRightOfMe == Preserve::Feature::kBrush ||
		memory.featureLeftOfMe == Preserve::Feature::kBrush;
}

bool isGrassNearMe(Brain& memory)
{
	return memory.featureInFrontOfMe == Preserve::Feature::kGrass ||
		memory.featureRightOfMe == Preserve::Feature::kGrass ||
		memory.featureLeftOfMe == Preserve::Feature::kGrass;
}
// *************** For diagnostics only - to be removed *************** end

void Elephant::tag(GPS& gps)
{
	gps_ = &gps;
}

void Elephant::findHerd()
{
	Brain memory;
	DirectionToHead directionToHead;
	bool firstTime = true;

	const int maxWater = 200;

	memory.currentDirection = getHeading(Turn::k0);

	do
	{
		// listen for herd to find direction
		memory.directionOfHerd = listen();

		if (firstTime)
		{
			memory.lastDirectionOfHerd = memory.directionOfHerd;
			firstTime = false;
		}
		else
		{
			if (memory.directionOfHerd != memory.lastDirectionOfHerd)
			{
				memory.secondToLastDirectionOfHerd = memory.lastDirectionOfHerd;
				memory.lastDirectionOfHerd = memory.directionOfHerd;
			}
		}

		// look around
		memory.featureUnderMe = look();
		memory.featureInFrontOfMe = look(Turn::kForward);
		memory.featureLeftOfMe = look(Turn::kLeft);
		memory.featureRightOfMe = look(Turn::kRight);
		memory.featureBehindMe = lookBehindMe(getGps_(), memory.currentDirection);

		memory.currentLatitude = getGps_()->getlat();
		memory.currentLongitude = getGps_()->getlng();

		memory.isHungry = isHungry();
		memory.isThirsty = isThirsty();
		memory.isSleepy = isSleepy();

		memory.water = getWater();
		memory.weight = getWeight();

		memory.waterNearMe = isWaterNearMe(memory);
		memory.brushNearMe = isBrushNearMe(memory);
		memory.grassNearMe = isGrassNearMe(memory);

		determineObstacles(memory);

		displayDiagnostics(memory);

		cout << endl;

		if (memory.featureUnderMe == Preserve::Feature::kHerd)
		{
			break;
		}

		// ******* Decisions *******

		auto moveTowardHerd = true;

		if (isSleepy())
		{
			sleep();
		}

		if (isThirsty())
		{
			if (memory.featureUnderMe == Preserve::Feature::kWater)
			{
				do
				{
					drink();
				} while (getWater() < maxWater);
			}
			else if (memory.waterNearMe)
			{
				if (memory.featureRightOfMe == Preserve::Feature::kWater)
					turn(Turn::kRight);
				else if (memory.featureLeftOfMe == Preserve::Feature::kWater)
					turn(Turn::kLeft);

				move();

				do
				{
					drink();
				} while (getWater() < maxWater);

				moveTowardHerd = false;
			}
		}
		else if (isHungry() && (memory.brushNearMe || memory.grassNearMe))
		{
			if (memory.featureRightOfMe == Preserve::Feature::kGrass || memory.featureRightOfMe == Preserve::Feature::kBrush)
				turn(Turn::kRight);
			else if (memory.featureLeftOfMe == Preserve::Feature::kGrass || memory.featureLeftOfMe == Preserve::Feature::kBrush)
				turn(Turn::kLeft);

			if (memory.grassNearMe)
			{
				move();
				eat();
			}
			else
			{
				do
				{
					eat();
				} while (getWeight() < getMaxWeight());
			}

			moveTowardHerd = false;
		}

		// ******* Move Processing *******
		if (moveTowardHerd)
		{
			bool moveInDirectionWeCan = false;

			int directionOfHerd = memory.directionOfHerd;

			if (!determineIfCanMoveInTheDirectionWeWantTo(directionOfHerd, memory, directionToHead))
			{
				// Obstacle in the way
				if (memory.directionOfHerd != memory.lastDirectionOfHerd)
				{
					if (determineIfCanMoveInTheDirectionWeWantTo(memory.lastDirectionOfHerd, memory, directionToHead))
					{
					}
					else if (!determineIfCanMoveInTheDirectionWeWantTo(memory.secondToLastDirectionOfHerd, memory, directionToHead))
					{
						moveInDirectionWeCan = true;
					}
				}
				else
				{
					if (!determineIfCanMoveInTheDirectionWeWantTo(memory.secondToLastDirectionOfHerd, memory, directionToHead))
					{
						moveInDirectionWeCan = true;
					}
				}
			}

			if (moveInDirectionWeCan)
			{
				bool directionFound = false;

				for (int directionToTry = 0; directionToTry <= 270; directionToTry += 90)
				{
					if (determineIfCanMoveInTheDirectionWeWantTo(directionToTry, memory, directionToHead))
					{
						directionFound = true;
						break;
					}
				}

				if (!directionFound)
				{
					cout << "Error: cannot go in direction not yet tried" << endl;
				}
			}

			if (directionToHead.keepHeadingForward) {}
			if (directionToHead.turnLeft)
			{
				turn(Turn::kLeft);
			}
			if (directionToHead.turnRight)
			{
				turn(Turn::kRight);
			}
			if (directionToHead.uTurn)
			{
				turn(Turn::kLeft);
				turn(Turn::kLeft);
			}
			memory.currentDirection = Elephant::getHeading(Elephant::Turn::kForward);

			move();
		}
	} while (true);
}

bool determineIfCanMoveInTheDirectionWeWantTo(int directionWeWantToHead, Brain &memory, DirectionToHead &directionToHead)
{
	if (directionWeWantToHead == -1)
	{
		return false;
	}

	directionToHead.keepHeadingForward = false;
	directionToHead.turnLeft = false;
	directionToHead.uTurn = false;
	directionToHead.turnRight = false;

	switch (memory.currentDirection)
	{
	case kNorth:
		switch (directionWeWantToHead)
		{
		case kNorth:
			directionToHead.keepHeadingForward = true;
			return !memory.obstacleInFrontOfMe;
		case kWest:
			directionToHead.turnLeft = true;
			return !memory.obstacleLeftOfMe;
		case kSouth:
			directionToHead.uTurn = true;
			return !memory.obstacleBehindMe;
		case kEast:
			directionToHead.turnRight = true;
			return !memory.obstacleRightOfMe;
		}
		break;

	case kWest:
		switch (directionWeWantToHead)
		{
		case kNorth:
			directionToHead.turnRight = true;
			return !memory.obstacleRightOfMe;
		case kWest:
			directionToHead.keepHeadingForward = true;
			return !memory.obstacleInFrontOfMe;
		case kSouth:
			directionToHead.turnLeft = true;
			return !memory.obstacleLeftOfMe;
		case kEast:
			directionToHead.uTurn = true;
			return !memory.obstacleBehindMe;
		}
		break;

	case kSouth:
		switch (directionWeWantToHead)
		{
		case kNorth:
			directionToHead.uTurn = true;
			return !memory.obstacleBehindMe;
		case kWest:
			directionToHead.turnLeft = true;
			return !memory.obstacleLeftOfMe;
		case kSouth:
			directionToHead.keepHeadingForward = true;
			return !memory.obstacleInFrontOfMe;
		case kEast:
			directionToHead.turnRight = true;
			return !memory.obstacleRightOfMe;
		}
		break;

	case kEast:
		switch (directionWeWantToHead)
		{
		case kNorth:
			directionToHead.turnLeft = true;
			return !memory.obstacleLeftOfMe;
		case kWest:
			directionToHead.uTurn = true;
			return !memory.obstacleBehindMe;
		case kSouth:
			directionToHead.turnRight = true;
			return !memory.obstacleRightOfMe;
		case kEast:
			directionToHead.keepHeadingForward = true;
			return !memory.obstacleInFrontOfMe;
		}
		break;
	}
	return false;
}

Preserve::Feature lookBehindMe(GPS* gps, Direction heading)
{
	auto result{ Preserve::Feature::kUnknown };

	if (gps != nullptr)
	{
		auto distance{ 1 };

		auto gpsLocation = *gps;
		const auto delta{ GPS::cardinal(heading - 180) };
		gpsLocation.move(heading + delta, distance);
		result = Preserve::getInstance().getFeature(gpsLocation);
	}

	return result;
}