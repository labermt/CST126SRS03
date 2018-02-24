#include "stdafx.h"
#include "elephant.h"
#include "preserve.h"
#include "direction.h"
#include "gps.h"

void Elephant::tag(GPS& gps)
{
<<<<<<< HEAD
	   GPS* getGps_();
=======
	GPS* getGps_();
>>>>>>> 4087efdf83d5cb2836685e045600cfcb20c2f274
}


void Elephant::findHerd()
{
	while (GPS* != herd)
	{
		auto destination = Elephant::listen();
		auto lookingAt = look(Turn::kForward);

		if (lookingAt == Preserve::Feature::kRock || lookingAt == Preserve::Feature::kBrush)
		{
			auto lookLeft = look(Turn::kLeft);
			if (lookLeft == Preserve::Feature::kRock || lookingAt == Preserve::Feature::kBrush)
			{
				Bool ImightBeStuck;
				turn(Turn::kRight);
			}
			else
			{
				turn(Turn::kLeft);
				Elephant::move();
			}
		}
		else
		{
			while (look != obstical)
			{
				if (destination == 0)
				{
					Elephant::move();
				}
				else if (destination == 90)
				{
					turn(Turn::kRight);
					Elephant::move();
				}
				else if (destination == 270)
				{
					turn(Turn::kLeft);
					Elephant::move();
				}
				else
				{
					turn(Turn::kLeft);
				}
			}
		}
	}
}