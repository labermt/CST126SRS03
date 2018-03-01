#include "stdafx.h"
#include "elephant.h"
#include <cassert>

// Qs for Mitch


bool movable(Elephant::Turn dir)
{
	auto result = false;
	/*switch (Elephant::look(dir))
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
	}*/
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
		int heading{};
		switch (getHeading(Turn::kForward))
		{
		case kNorth:
			heading = 0;
			break;

		case kEast:
			heading = 270;
			break;
			
		case kSouth:
			heading = 180;
			break;
			
		case kWest:
			heading = 90;
			break;

		default:
			assert(false);
			break;
		}
	
		auto const herd_dir = heading - listen();
		if(herd_dir == 0)
		{
			if(movable(Turn::kForward))
			{
				move();
			}
			else if(movable(Turn::kRight))
			{
				turn(Turn::kRight);
				move();
			}
			else if(movable(Turn::kLeft))
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
		}
		else if(herd_dir < 0)
		{
			if(movable(Turn::kLeft))
			{
				turn(Turn::kLeft);
				move();
			}
			else if(movable(Turn::kForward))
			{
				move();
			}
			//not sure if I want this case to be turn right or turn back
			else if(movable(Turn::kRight))
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
		}
		else if(herd_dir > 0)
		{
			if(movable(Turn::kRight))
			{
				turn(Turn::kRight);
				move();
			}
			else if(movable(Turn::kForward))
			{
				move();
			}
			//not sure if I want this case to be turn right or turn back
			else if(movable(Turn::kLeft))
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
		}
		else
		{
			assert(false);
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