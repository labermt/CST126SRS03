#include "stdafx.h"
#include "elephant.h"
#include "preserve.h"
//listen look move 
void Elephant::tag(GPS& gps)
{
	gps_ = &gps; 
}
void Elephant::findHerd()
{
	const auto feature{ look() };
	//const auto feature = look(); this is another way to rewrite line 13 
	while (feature != Preserve::Feature::kHerd);
	{
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

