#include "stdafx.h"
#include "elephant.h"

void Elephant::tag(GPS& gps)
{
	gps_ = &gps;
}

void Elephant::findHerd()
{
	auto feature = look();
	while (feature != Preserve::Feature::kHerd)
	{
		const auto focusDirection = listen();
		if (weight_ - minWeight_ < 80)
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

					}
				}
			}
		}

		const auto directionDifference = getHeading(Turn::kForward) - focusDirection;
		if (directionDifference % 360 == 90)
		{
			turn(Turn::kLeft);
		}
		if (directionDifference % 360 == 270)
		{
			turn(Turn::kRight);
		}

	}
}