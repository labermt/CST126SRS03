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

		//if on top of herd
		if (look() == Preserve::Feature::kHerd)
		{
			herdFound = true;
		}

		//Faces in the direction of the herd
		faceHerd();

		//Thirsty Elephant
		if (isThirsty() || getWater() <= 70 && look() == Preserve::Feature::kWater)
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

		//If forward is not blocked
		if (look(Turn::kForward) != Preserve::Feature::kRock &&  look(Turn::kForward) != Preserve::Feature::kBrush &&  look(Turn::kForward) != Preserve::Feature::kUnknown)
		{
			move();
		}
		//If forward is blocked but right is not
		else if (look(Turn::kRight) != Preserve::Feature::kRock &&  look(Turn::kRight) != Preserve::Feature::kBrush &&  look(Turn::kRight) != Preserve::Feature::kUnknown) {
			turn(Turn::kRight);
			move();
		}
		//If forward and right are blocked but left is not
		else if (look(Turn::kLeft) != Preserve::Feature::kRock &&  look(Turn::kLeft) != Preserve::Feature::kBrush &&  look(Turn::kLeft) != Preserve::Feature::kUnknown) {
			turn(Turn::kLeft);
			move();
		}
		//All blocked, turn around
		else {
			turn(Turn::kLeft);
			turn(Turn::kLeft);
			move();
			turn(Turn::kRight);
			move();
		}
	}
}

