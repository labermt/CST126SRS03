#include "stdafx.h"
#include "elephant.h"
#include <cassert>

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
				else
				{
					assert(false);
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