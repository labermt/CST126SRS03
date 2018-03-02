#include "stdafx.h"
#include <cassert>
#include "elephant.h"

using namespace std;

// Radical new comment!

void Elephant::tag(GPS& gps)
{
	setGps(gps);
}

void Elephant::findHerd()
{
	auto feature = look();
	while (feature != Preserve::Feature::kHerd)
	{
		if (feature == Preserve::Feature::kUnknown)
		{
			assert(false); // Let Mitch know
		}

		const auto focusDirection = listen();
		auto heading = getHeading(Turn::kForward);
		auto lookForward = look(Turn::kForward);
		auto lookRight = look(Turn::kRight);
		auto lookLeft = look(Turn::kLeft);
		auto forwardClear = lookForward == Preserve::Feature::kGrass || lookForward == Preserve::Feature::kDirt || lookForward == Preserve::Feature::kWater || lookForward == Preserve::Feature::kHerd;
		auto rightClear = lookRight == Preserve::Feature::kGrass || lookRight == Preserve::Feature::kDirt || lookRight == Preserve::Feature::kWater || lookRight == Preserve::Feature::kHerd;
		auto leftClear = lookLeft == Preserve::Feature::kGrass || lookLeft == Preserve::Feature::kDirt || lookLeft == Preserve::Feature::kWater || lookLeft == Preserve::Feature::kHerd;

		if (getWeight() - getMinWeight() < 80)
		{
			if (feature == Preserve::Feature::kGrass)
			{
				eat();
			}
			else
			{
				auto possibleDirections = { Turn::kLeft, Turn::kForward, Turn::kRight };
				for (auto direction : possibleDirections)
				{
					if (look(direction) == Preserve::Feature::kBrush)
					{
						turn(direction);
						eat();
						eat();
						eat();
						
					}
				}
			}
		}
		if (feature == Preserve::Feature::kWater)
		{
			drink();
		}
		/*if (getWater() <= 20)
		{
			if (lookLeft == Preserve::Feature::kWater)
			{
				turn(Turn::kLeft);
				move();
				drink();
			}
			if (lookRight == Preserve::Feature::kWater)
			{
				turn(Turn::kRight);
				move();
				drink();
			}
			if (lookForward == Preserve::Feature::kWater)
			{
				turn(Turn::kForward);
				move();
				drink();
			}
		}*/
		if (getAwake() > 15 * 60)
		{
			sleep();
		}

		const auto directionDifference = getHeading(Turn::kForward) - focusDirection;
		switch ((directionDifference + 360) % 360)
		{
		case 0:
			if (!forwardClear)
			{
				if (rightClear)
				{
					turn(Turn::kRight);
				}
				else if (leftClear)
				{
					turn(Turn::kLeft);
				}
				else
				{
					turn(Turn::kRight);
					turn(Turn::kRight);
				}
			}
			break;
		case 90:
			if (leftClear)
			{
				turn(Turn::kLeft);
			}
			else if (!forwardClear)
			{
				turn(Turn::kLeft);
				lookLeft = look(Turn::kLeft);
				leftClear = lookLeft == Preserve::Feature::kGrass || lookLeft == Preserve::Feature::kDirt || lookLeft == Preserve::Feature::kWater || lookLeft == Preserve::Feature::kHerd;
				if (leftClear)
				{
					turn(Turn::kLeft);
				}
				else
				{
					turn(Turn::kRight);
					turn(Turn::kRight);
				}
			}
			break;
		case 180:
			turn(Turn::kRight);
			turn(Turn::kRight);
			if (!forwardClear)
			{
				if (rightClear)
				{
					turn(Turn::kRight);
				}
				else if (leftClear)
				{
					turn(Turn::kLeft);
				}
				else
				{
					turn(Turn::kRight);
					turn(Turn::kRight);
				}
			}
			break;
		case 270:
			if (rightClear)
			{
				turn(Turn::kRight);
			}
			else if (!forwardClear)
			{
				turn(Turn::kRight);
				lookRight = look(Turn::kRight);
				rightClear = rightClear = lookRight == Preserve::Feature::kGrass || lookRight == Preserve::Feature::kDirt || lookRight == Preserve::Feature::kWater || lookRight == Preserve::Feature::kHerd;
				if (rightClear)
				{
					turn(Turn::kRight);
				}
				else
				{
					turn(Turn::kRight);
					turn(Turn::kRight);
				}
			}
			break;
		default:
			break;
		}
		move();
		feature = look();
	}
}