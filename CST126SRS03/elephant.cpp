#include "stdafx.h"
#include "elephant.h"

void Elephant::tag(GPS& gps)
{
	Loxodonta::setGps(gps); 
}

void Elephant::findHerd()
{
	while (look() != Preserve::Feature::kHerd)
	{
		const auto featureFound{ look() };
		if (featureFound == Preserve::Feature::kGrass && getWeight() < 6010)
		{
			eat();
		}

		if (featureFound == Preserve::Feature::kWater && getWater() <= 20)
		{
			drink();
		}

		if (featureFound != Preserve::Feature::kWater && isSleepy())
		{
			sleep();
		}

		faceHerd();

		const auto lookingAt = look(Turn::kForward);
		if (lookingAt == Preserve::Feature::kRock || lookingAt == Preserve::Feature::kBrush)
		{
			while (lookingAt == Preserve::Feature::kBrush && getWeight() < 5900)
			{
				eat();
			}
			dodgeObstacle();
		}
		else
		{
			Loxodonta::move();
		}
	}
}

void Elephant::dodgeObstacle()
{
	turn(Turn::kLeft);
	if (look(Turn::kForward) == Preserve::Feature::kRock || look(Turn::kForward) == Preserve::Feature::kBrush)
	{
		turn(Turn::kLeft);
	}
	Loxodonta::move();
	while (look(Turn::kRight) == Preserve::Feature::kRock || look(Turn::kRight) == Preserve::Feature::kBrush)
	{
		Loxodonta::move();
	}
	turn(Turn::kRight);
	Loxodonta::move();

	// return; // void returns are optional.
}