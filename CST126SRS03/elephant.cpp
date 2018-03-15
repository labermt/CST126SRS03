#include "stdafx.h"
#include "elephant.h"

void Elephant::tag(GPS& gps)
{
	setGps(gps);
}

void Elephant::findHerd()
{
	bool herdfound = false;

	while (!herdfound)
	{
		const int hortonHearsaWho = listen(); // Learn to use const. 

		switch (hortonHearsaWho)
		{
		case (0):
			turn(Turn::kForward);
			break;

		case (270):
			turn(Turn::kLeft); // I don't see why to turn left here. You don't know the elephant is facing kNorth.
			break;

		case (90):
			turn(Turn::kRight);
			break;

		default: // Or assert. Let the reader know you didn't forget the default case. 
			break;
		}

		
		auto toHortonsLeft = look(Turn::kLeft);
		auto toHortonsFront = look(Turn::kForward);
		auto toHortonsRight = look(Turn::kRight);

		// look based on 270,0, 90 and if there is no ob
		
		if (toHortonsLeft != Preserve::Feature::kUnknown)
		{
			switch (toHortonsLeft)
			{
			case Preserve::Feature::kUnknown:
				break;

			case Preserve::Feature::kHerd:
				herdfound = true;//??? break ???
			case Preserve::Feature::kDirt:
				move();
				// findHerd(); // Put a space after comment. Comments are for humans, help them read it. 
				break;

			case Preserve::Feature::kRock:
				break;

			case Preserve::Feature::kBrush:
				break;

			case Preserve::Feature::kGrass:
				move();
				eat();
				break;

			case Preserve::Feature::kWater:
				move();
				drink();
				break;
			}
		}
		
		switch (toHortonsFront)
		{
		case Preserve::Feature::kUnknown:
			break;

		case Preserve::Feature::kHerd:
			herdfound = true;
			break;

		case Preserve::Feature::kDirt:
			move();
			findHerd();
			break;

		case Preserve::Feature::kRock:
			break;

		case Preserve::Feature::kBrush:
			break;

		case Preserve::Feature::kGrass:
			move();
			eat();
			findHerd();
			break;

		case Preserve::Feature::kWater:
			move();
			drink();
			findHerd();
			break;
		}

		switch (toHortonsRight)
		{
		case Preserve::Feature::kUnknown:
			break;

		case Preserve::Feature::kHerd:
			herdfound = true;
			break;

		case Preserve::Feature::kDirt:
			move();
			findHerd();
			break;

		case Preserve::Feature::kRock:
			break;

		case Preserve::Feature::kBrush:
			break;

		case Preserve::Feature::kGrass:
			move();
			eat();
			findHerd();
			break;

		case Preserve::Feature::kWater:
			move();
			drink();
			findHerd();
			break;
		}
	}
}
