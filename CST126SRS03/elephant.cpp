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
	auto feature{ look() };
	while (feature != Preserve::Feature::kHerd)
	{
		if (feature == Preserve::Feature::kUnknown)
		{
			assert(false);
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
		constexpr auto sleepThreshhold{ 15 * 60 };
		if (getAwake() > sleepThreshhold)
		{
			sleep();
		}

		const auto focusDirection{ listen() };
		const auto heading{ getHeading(Turn::kForward) };

		const auto lookForward{ look(Turn::kForward) };
		const auto forwardClear{ canWalkOn(lookForward) };

		auto lookRight{ look(Turn::kRight) };
		auto rightClear{ canWalkOn(lookRight) };

		auto lookLeft{ look(Turn::kLeft) };
		auto leftClear{ canWalkOn(lookLeft) };

		const auto directionDelta{ getHeading(Turn::kForward) - focusDirection };
		switch ((directionDelta + 360) % 360)
		{
			case 0:
			{
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
			}
			break;

			case 90:
			{
				if (leftClear)
				{
					turn(Turn::kLeft);
				}
				else if (!forwardClear)
				{
					turn(Turn::kLeft);
					lookLeft = look(Turn::kLeft);
					leftClear = canWalkOn(lookLeft);
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
			}
			break;

			case 180:
			{
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
			}
			break;
			
			case 270:
			{
				if (rightClear)
				{
					turn(Turn::kRight);
				}
				else if (!forwardClear)
				{
					turn(Turn::kRight);
					lookRight = look(Turn::kRight);
					rightClear = canWalkOn(lookRight);
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
			}
			break;
			
			default:
			{
				assert(false);
			}
			break;
		}
		move();
		feature = look();
	}
}

bool Elephant::canWalkOn(const Preserve::Feature feature) const
{
	bool result = feature == Preserve::Feature::kGrass || feature == Preserve::Feature::kDirt || feature == Preserve::Feature::kWater || feature == Preserve::Feature::kHerd;
	return result;
}