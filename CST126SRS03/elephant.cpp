#include "stdafx.h"
#include "elephant.h"
#include <cassert>

// Qs for Mitch
<<<<<<< HEAD
// Nested function? I want to add a function here to return true if I can pass through a space
// Would there be a scenario where our elephant would be trapped and unable to reach the heard?
// Can we set different thresholds for isThirsty / hungry?
void movement(const int heard_dir)
{
	
}

bool movable(Elephant::Turn direction)
{
	auto result = false;

=======


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
>>>>>>> 11efb092ad56d1a95c718214b0f5983bb2435fce
}

void Elephant::findHerd()
{
	auto found_heard = false;
<<<<<<< HEAD

=======
>>>>>>> 11efb092ad56d1a95c718214b0f5983bb2435fce
	if(look() == Preserve::Feature::kHerd)
	{
		found_heard = true;
	}
<<<<<<< HEAD

	while(!found_heard)
	{
		auto heading = getHeading(Turn::k0);
		const auto heard_dir = listen();
		const auto look_f = look(Turn::kForward);
		const auto look_l = look(Turn::kLeft);
		const auto look_r = look(Turn::kRight);

		switch (heard_dir)
		{
		case 0: //heard = forward placeholder
			//look forward
			//if clear move forward break;
			
			//look right
			//if clear turn right, go forward break;

			//look left
			//if clear, turn right go forward break;

			//last resort
			//if front, right, left not clear
			// turn left twice,
			//break; (will be facing backwards and on the next iteration forward should be clear, else your trapped 
			break;

		case 1: //heard = left placeholder

			//basically all these scenarios could be handled by a function
			//if heard is left then turn left and follow case 0
			//if heard is right, then turn right and follow case 0

			//look left
			//if clear turn left move forward break;

			//look forward
			//if clear move forward break;
			turn(Turn::kLeft);
			move();
			break;

		case 2: //heard = right placeholder
			
			turn(Turn::kRight);
			move();
			break;

		default: //placeholder might not want this to be the default case but 
			if (look() == Preserve::Feature::kHerd)
			{
				found_heard = true;
				break;
			}
=======
	
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
>>>>>>> 11efb092ad56d1a95c718214b0f5983bb2435fce
			assert(false);
			break;
		}

		if(isThirsty() && look() == Preserve::Feature::kWater )
		{
			drink();
		}
		
<<<<<<< HEAD
		if(isHungry() && look() == Preserve::Feature::kGrass || look() == Preserve::Feature::kBrush )
		{
			eat();
		}
=======
		if(getWeight() <= (getMaxWeight() / 2) && look() == Preserve::Feature::kGrass || look(Turn::kForward) == Preserve::Feature::kBrush )
		{
			eat();
		}
		if(look()== Preserve::Feature::kHerd)
		{
			found_heard = true;
		}
>>>>>>> 11efb092ad56d1a95c718214b0f5983bb2435fce
	}
}

void Elephant::tag(class GPS& gps)
{
<<<<<<< HEAD
	gps_ = &gps;
}


//while elephant hasnt found heard repeat these following steps
// attach and update gps position if needed
//
// setup following as case statements
//
// check for food / water
// if food / water == true eat / drink
// 
// listen for heard
// turn to closest direction of heard
// 
// look forward
// if blocked && heard  < 90
// turn right else turn left
//
=======
	setGps(gps);
}
>>>>>>> 11efb092ad56d1a95c718214b0f5983bb2435fce
