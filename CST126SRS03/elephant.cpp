#include "stdafx.h"
#include "elephant.h"
#include <cassert>

// Qs for Mitch
// Nested function? I want to add a function here to return true if I can pass through a space
// Would there be a scenario where our elephant would be trapped and unable to reach the heard?
// Can we set different thresholds for isThirsty / hungry?
void movement(const int heard_dir)
{
	
}

bool movable(Elephant::Turn direction)
{
	auto result = false;

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
			assert(false);
			break;
		}

		if(isThirsty() && look() == Preserve::Feature::kWater )
		{
			drink();
		}
		
		if(isHungry() && look() == Preserve::Feature::kGrass || look() == Preserve::Feature::kBrush )
		{
			eat();
		}
	}
}
//Sets Elephant GPS Tag to initial coordinates.
void Elephant::tag(class GPS& gps)
{
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
