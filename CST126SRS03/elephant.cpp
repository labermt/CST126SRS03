#include "stdafx.h"
#include "elephant.h"
#include <cassert>

void Elephant::tag(class GPS& gps)
{
	setGps(gps);
}

void Elephant::findHerd()
{
	auto herdFound = false;
	
	while (!herdFound)
	{
		faceHerd();
		if (look() == Preserve::Feature::kHerd)
		{
			herdFound = true;
		}

		//Thirsty Elephant
		if (isThirsty() && look() == Preserve::Feature::kWater)
		{
			drink();
		}
		//Hungry Elephant
		if (isHungry() && look() == Preserve::Feature::kGrass || look() == Preserve::Feature::kBrush)
		{
			eat();
		}

		//Sleepy Elephant
		if (isSleepy())
		{
			sleep();
		}

		//Moving Forward
		if (look(Turn::kForward) != Preserve::Feature::kRock || look(Turn::kForward) != Preserve::Feature::kUnknown)
		{
			move();
		}
		//Moving Left if Forward is blocked
		else if (look(Turn::kForward) == Preserve::Feature::kRock || look(Turn::kForward) == Preserve::Feature::kUnknown &&
			     look(Turn::kLeft) != Preserve::Feature::kRock || look(Turn::kLeft) != Preserve::Feature::kUnknown)
		{
			turn(Turn::kLeft);
			move();
		}
		//Moving Right if Forward and Left is blocked
		else if (look(Turn::kForward) == Preserve::Feature::kRock || look(Turn::kForward) == Preserve::Feature::kUnknown &&
			look(Turn::kLeft) == Preserve::Feature::kRock || look(Turn::kLeft) == Preserve::Feature::kUnknown &&
			look(Turn::kLeft) != Preserve::Feature::kRock || look(Turn::kLeft) != Preserve::Feature::kUnknown)
		{
			turn(Turn::kRight);
			move();
		}
		else
		{
			turn(Turn::kLeft);
			turn(Turn::kLeft);
			move();
			turn(Turn::kRight);
		}
	}
}

