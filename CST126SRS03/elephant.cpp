#include "stdafx.h"

#include <iostream>
#include "direction.h"
#include "gps.h"
#include "preserve.h"
#include "elephant.h"

void Elephant::tag(GPS& gps)
{
	setGps(gps);
}
void Elephant::findHerd()
{
	bool foundHerd{ false };
	int herdDirection;
	//Direction direction;

	while (!foundHerd)
	{
		//search for herd

		Preserve::Feature feature;
		int heading;

		//FIRST, SURVIVE
		//Look in all directions for herd
		Turn turns[4] = { Turn::k0, Turn::kForward, Turn::kLeft , Turn::kRight };
		for (Turn turn : turns) {
			feature = look(turn);

			if (feature == Preserve::Feature::kHerd)
			{
				foundHerd = true;
				break;
			}
		}
		if (foundHerd) break;
		feature = look();

		//Deal with hunger
		if (isHungry() && feature == Preserve::Feature::kGrass) eat(); //eat grass
		if (isHungry() && look(Turn::kForward) == Preserve::Feature::kBrush) eat(); //eat brush

		//Deal with thirst
		if (isThirsty() && feature == Preserve::Feature::kWater) drink();

		//Sleep if sleepy
		if (isSleepy()) sleep();


		//SECOND, LISTEN FOR HERD
		herdDirection = listen();

		if (herdDirection == 0) {
			turn(Turn::kForward);
		}
		else if (herdDirection == 90) {
			turn(Turn::kRight);
		}
		else if (herdDirection == 180) {
			turn(Turn::kRight);
			turn(Turn::kRight);
		}
		else if (herdDirection == 270) {
			turn(Turn::kLeft);
		}


		//THIRD, detect if obstacles are in front of elephant
		if (look(Turn::kForward) == Preserve::Feature::kRock ||
			look(Turn::kForward) == Preserve::Feature::kBrush)
		{
			

			//Check obstacle left
			if (look(Turn::kLeft) == Preserve::Feature::kRock ||
				look(Turn::kLeft) == Preserve::Feature::kBrush) 
			{
				//check obstacle right
				if (look(Turn::kRight) == Preserve::Feature::kRock ||
					look(Turn::kRight) == Preserve::Feature::kBrush)
				{
					turn(Turn::kRight);
					turn(Turn::kRight);
					move();

					//elephant is lost at this point
					throw;

					/* 
					//This code may be implemented once I can figure out a reliable solution
					if (look(Turn::kRight) == Preserve::Feature::kRock ||
						look(Turn::kRight) == Preserve::Feature::kBrush) {

						
					}
					else {
						turn(Turn::kRight);
						move();
						turn(Turn::kRight);
					}
					*/

				}
				else { //ERROR HERE FOR SOME REASON WHEN TESTING
					turn(Turn::kRight);
					move();
					turn(Turn::kLeft);
				}
			}
			else {
				turn(Turn::kLeft);
				move();
				turn(Turn::kRight);
			}

		}
		else {
			move();
		}
	}
}