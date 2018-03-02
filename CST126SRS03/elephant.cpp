#include "stdafx.h"
#include "elephant.h"
#include <cassert>

<<<<<<< HEAD
// Methods to implement in heirarchal order: eleTrail, isStuck, isNecesity, retraceTrail

/* void eleTrail()
{
	
} */

=======
>>>>>>> 790d887f001f5121b70e7a2fc2ef6354e408b727
bool movable(Preserve::Feature const terrain)
{
	auto result = false;
	switch (terrain)
	{
	case Preserve::Feature::kUnknown:
	case Preserve::Feature::kRock:
	case Preserve::Feature::kBrush:
		break;

	case Preserve::Feature::kDirt:
	case Preserve::Feature::kGrass:
	case Preserve::Feature::kWater:
	case Preserve::Feature::kHerd:
		result = true;
		break;

	default:
		assert(false);
		break;
	}
	return result;
}

void Elephant::findHerd()
{
	auto found_heard = false;
	if(look() == Preserve::Feature::kHerd)
	{
		found_heard = true;
	}
	
	while(!found_heard)
	{
		if (isThirsty() && look() == Preserve::Feature::kWater)
		{
			drink();
		}

		if (getWeight() <= (getMaxWeight() / 2) && look() == Preserve::Feature::kGrass || look(Turn::kForward) == Preserve::Feature::kBrush)
		{
			eat();
		}

		auto const herd_dir = listen();
		auto const heading = getHeading(Turn::kForward);
		
		if (heading != herd_dir)
		{
			if (getHeading(Turn::kRight) == herd_dir)
			{
				if(movable(look(Turn::kRight)))
				{
					turn(Turn::kRight);
					move();
				}
				else if(movable(look(Turn::kForward)))
				{
					move();
				}
				else
				{
					turn(Turn::kLeft);
					move();
				}
			}
			else if (getHeading(Turn::kLeft) == herd_dir)
			{
				if(movable(look(Turn::kLeft)))
				{
					turn(Turn::kLeft);
					move();
				}
				else if (movable(look(Turn::kForward)))
				{
					move();
				}
				else
				{
					turn(Turn::kRight);
					move();
				}
			}
			else
			{
				if (movable(look(Turn::kForward)))
				{
					move();
				}
				else //Herd is behind Elephant 
				{
					//assert(false);

					turn(Turn::kLeft);
					turn(Turn::kLeft);

					//Redetermine herds' location.
				}
			}
		}
		else
		{
			if(movable(look(Turn::kForward)))
			{
				move();
			}
			else if(movable(look(Turn::kRight)))
			{
				turn(Turn::kRight);
				move();
			}
			else
			{
				turn(Turn::kLeft);
				move();
			}
		}
		if (look() == Preserve::Feature::kHerd)
		{
			found_heard = true;
		}

	}
}

void Elephant::tag(class GPS& gps)
{
	setGps(gps);
}