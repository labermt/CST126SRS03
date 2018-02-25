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
	// I don't know if this is necesary but it limits the typing below I think.
	auto const kUnknown_ = Preserve::Feature::kUnknown;
	auto const kHerd_ = Preserve::Feature::kHerd;
	auto const kDirt_ = Preserve::Feature::kDirt;
	auto const kRock_ = Preserve::Feature::kRock;
	auto const kBrush_ = Preserve::Feature::kBrush; 
	auto const kGrass_ = Preserve::Feature::kGrass;
	auto const kWater_ = Preserve::Feature::kWater;

	auto feature = look();
	auto foundHerd{ feature == kHerd_ };

	while (!foundHerd)
	{
		const auto direction = listen();
		auto turn(direction);
		move();

		if (isHungry())	 // no case for no grass or brush bordering.
		{
			while (look() != kGrass_)
			{
				if (look(Turn::kForward) == kBrush_)
				{
					eat();
				}
				else if (look(Turn::kForward) == kGrass_)
				{
					move();
					eat();
				}
				else if (look(Turn::kRight) == kBrush_)
				{
					auto turn(Turn::kRight);
					eat();
			
				}
				else if (look(Turn::kRight) == kGrass_)
				{
					auto turn(Turn::kRight);
					move();
					eat();
				}
				else if (look(Turn::kLeft) == kBrush_)
				{
					auto turn(Turn::kLeft);
					eat();
				}
				else if (look(Turn::kLeft) == kGrass_)
				{
					auto turn(Turn::kLeft);
					move();
					eat();
				}
			}
		}


		if (isThirsty())
		{
			while (look() != kWater_)
			{
				//turn until elephant is facing water. 
				//case for no water bordering
			}
		}


		if (isSleepy() || look() != kWater_)  // so the elephant doesn't sleep in water
		{
			sleep();
		}
		
	}
	
}