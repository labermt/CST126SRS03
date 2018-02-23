#include "stdafx.h"
#include "elephant.h"



void Elephant::findHerd()
{
	auto found_heard = false;
	while(!found_heard)
	{
		auto heading = getHeading(Turn::kForward);
		auto heard_dir = listen();
		if(water_ <= kMaxWater / 2 )
		{
			drink();
		}
		// adding comment for push test
		// food isnt implemented yet apparently 
		// if(food_ <= kMaxFood / 2 )
		// {
		//	eat();
		// }
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
