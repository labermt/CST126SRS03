#include "stdafx.h"
#include "preserve.h"
#include "elephant.h"

void Elephant::tag(GPS& gps)
{
	setGps(gps);
}

void Elephant::findHerd()
{
	while (look() != Preserve::Feature::kHerd)
	{
		healthCheck();

		// Dead code: const auto hearDir = listen();
		faceHerd();
		auto see{ look(Turn::kForward) };
		while (Preserve::isObstacle(see))
		{
			turn(Turn::kRight); // Always taking a right caused an infinite loop. 
			see = look(Turn::kForward);
			if (!Preserve::isObstacle(see))
			{
				move();
				turn(Turn::kLeft);
				see = look(Turn::kForward);
			}
		}
		move();
	}
}

void Elephant::healthCheck()
{
	if (isHungry())
	{
		const auto see = look();
		const auto grassFound{ see == Preserve::Feature::kGrass };
		if (grassFound)
		{
			eat();
		};
	}

	if (isThirsty())
	{
		const auto see = look();
		const auto waterFound{ see == Preserve::Feature::kWater };
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
