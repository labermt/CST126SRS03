#include "stdafx.h"
#include "elephant.h"
#include <cassert>

// Qs for Mitch


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
		auto const herd_dir = getHeading(Turn::kForward) - listen();
		switch(herd_dir)
		{
			case 0:
			if(movable(look(Turn::kForward)))
			{
				move();
			}
			else if(movable(look(Turn::kRight)))
			{
				turn(Turn::kRight);
				move();
			}
			else if(movable(look(Turn::kLeft)))
			{
				turn(Turn::kLeft);
				move();
			}
			else
			{
				turn(Turn::kLeft);
				turn(Turn::kLeft);
				// mark gps spot as immovable
				move();
				turn(Turn::kRight);
			}
			break;

			case 90:
			if(movable(look(Turn::kRight)))
			{
				turn(Turn::kRight);
				move();
			}
			else if(movable(look(Turn::kForward)))
			{
				move();
			}
			//not sure if I want this case to be turn right or turn back
			else if(movable(look(Turn::kLeft)))
			{
				turn(Turn::kLeft);
				move();
			}
			else
			{
				turn(Turn::kLeft);
				turn(Turn::kLeft);
				// mark gps spot as immovable
				move();
				turn(Turn::kRight);
			}
			break;

			case 270:
			if(movable(look(Turn::kLeft)))
			{
				turn(Turn::kLeft);
				move();
			}
			else if(movable(look(Turn::kForward)))
			{
				move();
			}
			//not sure if I want this case to be turn right or turn back
			else if(movable(look(Turn::kRight)))
			{
				turn(Turn::kRight);
				move();
			}
			else
			{
				turn(Turn::kRight);
				turn(Turn::kRight);
				// mark gps spot as immovable
				move();
				turn(Turn::kLeft);
			}
			break;
			
			case 180:
			//not sure what to do if 180
			break;

			default:
			assert(false);
			break;
		}

		if(isThirsty() && look() == Preserve::Feature::kWater )
		{
			drink();
		}
		
		if(getWeight() <= (getMaxWeight() / 2) && look() == Preserve::Feature::kGrass || look(Turn::kForward) == Preserve::Feature::kBrush )
		{
			eat();
		}
		if(look()== Preserve::Feature::kHerd)
		{
			found_heard = true;
		}
	}
}

void Elephant::tag(class GPS& gps)
{
	setGps(gps);
}