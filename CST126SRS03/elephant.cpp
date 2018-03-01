#include "stdafx.h"
#include "elephant.h"
#include <cassert>

// Qs for Mitch


bool movable(Elephant::Turn dir)
{
	auto result = false;
	const auto look = Elephant::look(dir);
	
	switch (look)
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
		auto heading = getHeading(Turn::k0);
		const auto heard_dir = listen();

		switch (heard_dir)
		{
		case 0: //heard = forward placeholder
			if(movable(Turn::kForward))
			{
				move();
				break;
			}
			if(movable(Turn::kRight))
			{
				turn(Turn::kRight);
				move();
				break;
			}
			if(movable(Turn::kLeft))
			{
				turn(Turn::kLeft);
				move();
				break;
			}
			else
			{
				turn(Turn::kLeft);
				turn(Turn::kLeft);
				// mark gps spot as immovable
				move();
				turn(Turn::kRight);
				break;
			}

		case 1: //heard = left placeholder
			if(movable(Turn::kLeft))
			{
				turn(Turn::kLeft);
				move();
				break;
			}
			if(movable(Turn::kForward))
			{
				move();
				break;
			}
			//not sure if I want this case to be turn right or turn back
			if(movable(Turn::kRight))
			{
				turn(Turn::kRight);
				move();
				break;
			}
			else
			{
				turn(Turn::kRight);
				turn(Turn::kRight);
				// mark gps spot as immovable
				move();
				turn(Turn::kLeft);
				break;
			}

		case 2: //heard = right placeholder
			
			if(movable(Turn::kRight))
			{
				turn(Turn::kRight);
				move();
				break;
			}
			if(movable(Turn::kForward))
			{
				move();
				break;
			}
			//not sure if I want this case to be turn right or turn back
			if(movable(Turn::kLeft))
			{
				turn(Turn::kLeft);
				move();
				break;
			}
			else
			{
				turn(Turn::kLeft);
				turn(Turn::kLeft);
				// mark gps spot as immovable
				move();
				turn(Turn::kRight);
				break;
			}

		default: //placeholder might not want this to be the default case but 
			if (look() == Preserve::Feature::kHerd)
			{
				found_heard = true;
				break;
			}
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