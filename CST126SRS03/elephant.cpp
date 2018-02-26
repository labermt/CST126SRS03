#include "stdafx.h"
#include <iostream>
#include "gps.h"
#include "preserve.h"
#include "elephant.h"

void Elephant::tag( GPS& gps)
{
	//getGps_() = gps;
	//gps_ = &gps;
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

		if (isSleepy() || look() != kWater_)  // so the elephant doesn't sleep in water
		{
			sleep();
		}
		
	}
}