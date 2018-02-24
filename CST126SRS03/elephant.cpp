#include "stdafx.h"

#include "direction.h"
#include "gps.h"
#include "preserve.h"
#include "elephant.h"

void Elephant::tag(GPS& gps)
{
	gps_ = &gps;
}
void Elephant::findHerd() 
{
	bool foundHerd{ false };

	while (!foundHerd)
	{
		//search for herd

		

		//test if herd is found
		if (look() == Preserve::Feature::kHerd) {
			foundHerd = true;
		}
	}
	//use look() function to determine what is under your feet
}