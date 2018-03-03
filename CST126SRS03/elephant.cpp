#include "stdafx.h"
#include <cassert>
#include "elephant.h"

using namespace std;

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

		if (getWeight() - getMinWeight() < 80)
		{
			if (feature == Preserve::Feature::kGrass)
			{
				eat();
			}
			else
			{
				const auto possibleDirections = { Turn::kLeft, Turn::kForward, Turn::kRight };
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
		if (getAwake() > 15 * 60)
		{
			sleep();
		}

		const auto focusDirection = listen();
		const auto heading = getHeading(Turn::kForward);

		const auto lookForward = look(Turn::kForward);
		const auto forwardClear = lookForward == Preserve::Feature::kGrass || lookForward == Preserve::Feature::kDirt || lookForward == Preserve::Feature::kWater || lookForward == Preserve::Feature::kHerd;

		auto lookRight = look(Turn::kRight);
		auto rightClear = lookRight == Preserve::Feature::kGrass || lookRight == Preserve::Feature::kDirt || lookRight == Preserve::Feature::kWater || lookRight == Preserve::Feature::kHerd;

		auto lookLeft = look(Turn::kLeft);
		auto leftClear = lookLeft == Preserve::Feature::kGrass || lookLeft == Preserve::Feature::kDirt || lookLeft == Preserve::Feature::kWater || lookLeft == Preserve::Feature::kHerd;

		const auto directionDelta = getHeading(Turn::kForward) - focusDirection;
		switch ((directionDelta + 360) % 360)
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
			assert(false);
			break;
		}
		move();
		feature = look();
	}
}