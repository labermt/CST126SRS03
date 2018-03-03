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
		//listen for the heard 
		int hortonHearsaWho = listen();

		switch (hortonHearsaWho)
		{
		case (0):
			turn(Turn::kForward);
			break;
		case (270):
			turn(Turn::kLeft);
			break;
		case (90):
			turn(Turn::kRight);
			break;
		}

		
		auto toHortonsLeft = look(Turn::kLeft);
		auto toHortonsFront = look(Turn::kForward);
		auto toHortonsRight = look(Turn::kRight);

		//look based on 270,0, 90 and if there is no ob
		
		if (toHortonsLeft != Preserve::Feature::kUnknown)
		{
			switch (toHortonsLeft)
			{
			case Preserve::Feature::kUnknown:
				break;
			case Preserve::Feature::kHerd:
				herdfound = true;
			case Preserve::Feature::kDirt:
				move();
				//findHerd();
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





		
	

