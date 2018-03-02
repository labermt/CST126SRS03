#include "stdafx.h"
#include "elephant.h"
#include"preserve.h"
#include "direction.h"

	void Elephant::tag(GPS& gps)
{
	//gps_ = &gps;
};

void Elephant::findHerd()
{
	//Preserve::Feature look();
	//Preserve::Feature look(const Turn turn);
	const auto feature{ look() };


	while (feature != Preserve::Feature::kHerd)
	{
		//int hearDir = listen();
		//int curDir = 0;
		//auto turnDir = hearDir;
		/*
		if (45 < hearDir && hearDir<134 && curDir!= 1%4)
		{
		turn(Turn::kRight);
		}
		if (135 < hearDir && hearDir < 224 && curDir!=2%4)
		{
		turn(Turn::kRight);
		}
		if (225 < hearDir && hearDir < 314 && curDir=3%4)
		{
		turn(Turn::kLeft);
		}
		*/
		while (GPS::cardinal(listen()) != getHeading(Turn::kForward))
		{
			turn(Turn::kRight);
		}
		auto see = look();

		if (see == Preserve::Feature::kRock || see == Preserve::Feature::kBrush)
		{
			turn(Turn::kRight);
			move();
		}
		move();
		if (isSleepy())
		{
			sleep();
		}
		if (isHungry())//refine peramiters
		{
			look();
			if (see == Preserve::Feature::kBrush)
			{
				eat();
			}
			if (see == Preserve::Feature::kGrass)
			{
				//move();
				eat();
			}
		}
		if (isThirsty())
		{
			look();
			if (see == Preserve::Feature::kWater)
			{
				//move();
				drink();
			}
		};
	};
};
