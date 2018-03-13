#include "stdafx.h"
#include <cassert>
#include "gps.h"
#include "preserve.h"
#include "elephant.h"
#include "loxodonta.h"

// Genius code from Colin (Seadra) https://github.com/SeadraCST126/CST126SRS03/blob/master/CST126SRS03/elephant.cpp
bool canMove(Preserve::Feature const terrain)
{
	auto result = false;

	switch (terrain)
	{
	case Preserve::Feature::kUnknown:
	case Preserve::Feature::kRock:
	case Preserve::Feature::kBrush:
		result = false;
		break;

	case Preserve::Feature::kDirt:
	case Preserve::Feature::kGrass:
	case Preserve::Feature::kWater:
	case Preserve::Feature::kHerd:
		result = true;
		break;

	default:
		assert(false);
		result = false;
		break;
	}

	return result;
}
void Elephant::tag( GPS& gps)
{
	setGps(gps);
}

void Elephant::findHerd()
{
	while (look() != Preserve::Feature::kHerd)
	{
		// Listen and turn towrard the herd
		auto const herdHeading = GPS::cardinal(listen());
		while (herdHeading != getHeading(Turn::kForward))
		{
			turn(Turn::kRight);
		}

		// Check for brush in front of elephant
		if (look(Turn::kForward) == Preserve::Feature::kBrush)
		{
			eat();
		}
	
		// Look at what elephant is standing on
		if (look() == Preserve::Feature::kGrass)
		{
			eat();
		}
		else if (look() == Preserve::Feature::kWater)
		{
			drink();
		}

		// Sleep if tired, and not in water (no killing elephant)
		if (isSleepy() && look() != Preserve::Feature::kWater)
		{
			sleep();
		}
			
		// Maneuver around obstacles
		while (!canMove(look(Turn::kForward)))
		{
			if (canMove(look(Turn::kRight)))
			{
				turn(Turn::kRight);
				move();
				turn(Turn::kLeft);
				
				if (herdHeading != GPS::cardinal(listen()))
				{
					turn(Turn::kLeft);
					move();
					move();
					turn(Turn::kRight);
				}
			}
			else if (canMove(look(Turn::kLeft)))
			{
				turn(Turn::kLeft);
				move();
				while (!canMove(look(Turn::kRight))) // {} suggested on 
				{
					move();
				}
				turn(Turn::kRight);
			}
		}
		move();
	}
}
// blank line added to end. Not required, some old utilties prefer it. Remember the assignement parsing cin when the last character isn't a '\n'?
