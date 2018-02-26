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
	auto kUnknown_ = Preserve::Feature::kUnknown;
	auto kHerd_ = Preserve::Feature::kHerd;
	auto kDirt_ = Preserve::Feature::kDirt;
	auto kRock_ = Preserve::Feature::kRock;
	auto kBrush_ = Preserve::Feature::kBrush; 
	auto kGrass_ = Preserve::Feature::kGrass;
	auto kWater_ = Preserve::Feature::kWater;

	auto feature = look();
	auto foundHerd{ feature == kHerd_ };
	while (!foundHerd)
	{
		auto direction = listen();
		turn(direction);
		move();  // want this to be last statement


		if (isSleepy() && look() != kWater_)  
		{
			sleep();
		}
		
	}
	
}