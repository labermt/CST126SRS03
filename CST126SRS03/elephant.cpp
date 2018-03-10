#include "stdafx.h"
#include "elephant.h"
#include <cassert>

struct Brain
{
	int directionOfHerd;

	Direction currentDirection;
	int lastDirectionOfHerd{ -1 };
	int secondToLastDirectionOfHerd{ -1 };

	Preserve::Feature featureUnderMe;
	Preserve::Feature featureInFrontOfMe;
	Preserve::Feature featureLeftOfMe;
	Preserve::Feature featureRightOfMe;
	Preserve::Feature featureBehindMe;

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

void determineObstacles(Brain& memory);
bool determineIfCanMoveInTheDirectionWeWantTo(int directionWeWantToHead, Brain &memory, DirectionToHead &directionToHead);
Preserve::Feature lookBehindMe(GPS* gps, Direction heading);
bool isWaterNearMe(Brain& memory);
bool isBrushNearMe(Brain& memory);
bool isGrassNearMe(Brain& memory);

void Elephant::tag(GPS& gps)
{
	setGps(gps);
}

void Elephant::findHerd()
{
	Brain memory;
	DirectionToHead directionToHead;
	auto firstTime = true;

	const auto maxWater = 200;

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
		else if (memory.directionOfHerd != memory.lastDirectionOfHerd)
		{
			memory.secondToLastDirectionOfHerd = memory.lastDirectionOfHerd;
			memory.lastDirectionOfHerd = memory.directionOfHerd;
		}

		// look around
		memory.featureUnderMe = look();
		memory.featureInFrontOfMe = look(Turn::kForward);
		memory.featureLeftOfMe = look(Turn::kLeft);
		memory.featureRightOfMe = look(Turn::kRight);
		memory.featureBehindMe = lookBehindMe(getGps_(), memory.currentDirection);

		memory.waterNearMe = isWaterNearMe(memory);
		memory.brushNearMe = isBrushNearMe(memory);
		memory.grassNearMe = isGrassNearMe(memory);

		determineObstacles(memory);

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
			auto moveInDirectionWeCan = false;

			const auto directionOfHerd = memory.directionOfHerd;

			if (!determineIfCanMoveInTheDirectionWeWantTo(directionOfHerd, memory, directionToHead))
			{
				// Obstacle in the way
				if (memory.directionOfHerd != memory.lastDirectionOfHerd)
				{
					if (!determineIfCanMoveInTheDirectionWeWantTo(memory.secondToLastDirectionOfHerd, memory, directionToHead))
					{
						moveInDirectionWeCan = true;
					}
					// else (determineIfCanMoveInTheDirectionWeWantTo(memory.lastDirectionOfHerd, memory, directionToHead))
					// { does nothing}
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
				auto directionFound = false;

				for (auto directionToTry = 0; directionToTry <= 270; directionToTry += 90)
				{
					if (determineIfCanMoveInTheDirectionWeWantTo(directionToTry, memory, directionToHead))
					{
						directionFound = true;
						break;
					}
				}

				if (!directionFound)
				{
					// do something to handle error
					assert(false);
				}
			}

			if (directionToHead.turnLeft)
			{
				turn(Turn::kLeft);
			}
			else if (directionToHead.turnRight)
			{
				turn(Turn::kRight);
			}
			else if (directionToHead.uTurn)
			{
				turn(Turn::kLeft);
				turn(Turn::kLeft);
			}
			// else (directionToHead.keepHeadingForward) {does not turn}

			memory.currentDirection = Elephant::getHeading(Elephant::Turn::kForward);

			move();
		}
	} while (true);
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
	default:
		break;
	}

	switch (memory.featureRightOfMe)
	{
	case Preserve::Feature::kRock:
	case Preserve::Feature::kBrush:
		memory.obstacleRightOfMe = true;
		break;
	default:
		break;
	}

	switch (memory.featureLeftOfMe)
	{
	case Preserve::Feature::kRock:
	case Preserve::Feature::kBrush:
		memory.obstacleLeftOfMe = true;
		break;
	default:
		break;
	}

	switch (memory.featureBehindMe)
	{
	case Preserve::Feature::kRock:
	case Preserve::Feature::kBrush:
		memory.obstacleBehindMe = true;
		break;
	default:
		break;
	}
}

bool determineIfCanMoveInTheDirectionWeWantTo(const int directionWeWantToHead, Brain &memory, DirectionToHead &directionToHead)
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
		default:
			break;
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
		default:
			break;
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
		default:
			break;
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
		default:
			break;
		}
	default:
		break;
	}
	
	return false;
}

Preserve::Feature lookBehindMe(GPS* gps, const Direction heading)
{
	auto result{ Preserve::Feature::kUnknown };

	if (gps != nullptr)
	{
		const auto distance{ 1 };

		auto gpsLocation = *gps;
		
		const auto delta{ GPS::cardinal(heading - 180) };
		
		gpsLocation.move(heading + delta, distance);
		
		result = Preserve::getInstance().getFeature(gpsLocation);
	}

	return result;
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
