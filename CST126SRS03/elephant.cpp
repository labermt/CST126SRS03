#include "stdafx.h"
#include "preserve.h"
#include "elephant.h"

//listen look move 
void Elephant::tag(GPS& gps)
{
	setGps(gps);
}

void Elephant::findHerd()
{
	const auto feature{ look() };
	auto currentHeading{getHeading(forward)}; 
	while (feature != Preserve::Feature::kHerd)
	{
		healthCheck(); 

		auto hearDir = listen(); 
		turn(Turn::kLeft); 
		auto see = look();
		Elephant::look(); 
		auto herdDirection = listen();
		look();
		turn(Turn::kLeft);
		if (see == Preserve::Feature::kRock || see == Preserve::Feature::kBrush)
		{

		}
		auto herdFound{ see == Preserve::Feature::kHerd };
		while (!herdFound)
		{

		}
		if (isHungry())
		{

		}
		if (isThirsty())
		{

		}
		if (isSleepy())
		{

		}
	}
}

void Elephant::healthCheck()
{
	if (isHungry()) 
	{
		auto see = look();
		auto grassFound{ see == Preserve::Feature::kGrass };
		if (grassFound) 
		{
			eat();
		};
	}

	if (isThirsty())
	{
		auto see = look(); 
		auto waterFound{ see == Preserve::Feature::kWater }; 
		if (waterFound)
		{
			drink();
		}
	}

	if (isSleepy())
	{
		sleep();
	}
}
