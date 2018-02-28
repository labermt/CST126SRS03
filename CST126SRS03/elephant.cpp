#include "stdafx.h"
#include "gps.h"
#include "preserve.h"
#include "elephant.h"
#include "loxodonta.h"

void Elephant::tag( GPS& gps)
{
	setGps(gps);
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
		// TODO: Not stopping here, will fix tomorrow
		if (look() == kHerd_)
		{
			foundHerd = true;
		}

		//Listen and turn towrard the herd
		while (GPS::cardinal(listen()) != getHeading(Turn::kForward))
		{
			turn(Turn::kRight);
		}

		//Check for brush in front of elephant
		if (look(Turn::kForward) == kBrush_)
		{
			eat();
		}
	
		//Look at what elephant is standing on
		if (look() == kGrass_)
		{
			eat();
		}
		else if (look() == kWater_)
		{
			drink();
		}

		//Sleep if tired, and not in water (no killing elephant)
		if (isSleepy() && look() != kWater_)  
		{
			sleep();
		}

		//Maneuver around obstacles
		while (look(Turn::kForward) == kRock_ || look(Turn::kForward) == kBrush_) // While there's an obstacle
		{
			if (look(Turn::kRight) != kRock_ && look(Turn::kRight) != kBrush_)	
			{
				turn(Turn::kRight);
				move();
				turn(Turn::kLeft);
			}
			//If there is obstacle to Right
			else if (look(Turn::kLeft) != kRock_ && look(Turn::kLeft) != kBrush_)	
			{
				turn(Turn::kLeft);
				move();
				turn(Turn::kRight);
			}
		}


		move(); 
	}
}