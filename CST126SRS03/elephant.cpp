#include "stdafx.h"
#include "elephant.h"



void Elephant::findHerd()
{
	auto found_heard = false;
	while(!found_heard)
	{
		auto heading = getHeading(Turn::kForward);
		auto heard_dir = listen();
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
