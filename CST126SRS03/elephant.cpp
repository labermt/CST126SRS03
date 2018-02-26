#include "stdafx.h"

#include <iostream>
#include "direction.h"
#include "gps.h"
#include "preserve.h"
#include "elephant.h"

void Elephant::tag(GPS& gps)
{
	gps_ = &gps; //THIS DOESN'T WORK NOW WITH MITCH"S NEW INHERITANCE COMMIT??
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

		//detect if obstacles are in front of elephant
		if (look(Turn::kForward) == Preserve::Feature::kRock ||
			look(Turn::kForward) == Preserve::Feature::kBrush)
		{
			
			
			//TODO: FIGURE OUT HOW TO GO AROUND OBSTACLES
			
			
			//move around obstacle
			if (herdDirection == 0) {
				turn(Turn::kRight);
				turn(Turn::kRight);
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
		}
		else {
			//normal move
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
		}

		//std::cout << direction << std::endl;

		//LASTLY, MOVE TOWARDS HERD
		move();
	}
}