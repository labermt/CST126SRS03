#include "stdafx.h"
#include <iostream>
#include "gps.h"
#include "preserve.h"
#include "elephant.h"

void Elephant::tag( GPS& gps)
{
	gps_ = &gps;
}

void Elephant::findHerd()
{
	auto feature = look();
	auto foundHerd{ feature == Preserve::Feature::kHerd };
	while (!foundHerd)
	{
		const auto direction = listen();
		auto turn(direction);
		move();


		if (isHungry())
		{
			if (look(Turn::kForward) == Preserve::Feature::kBrush) // or kGrass
				move();
				eat();
			else if (look(Turn::kForward) == Preserve::Feature::kBrush)

			}
		}

		if (isThirsty())
		{
			while (look() != Preserve::Feature::kWater)
			{
				//turn until elephant is facing water. 
				//case for no water bordering
			}
		}

		if (isSleepy())
		{
			sleep();
		}

	}
	
}