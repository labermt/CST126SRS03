#include "stdafx.h"

//#include <iostream> // Unused.
//#include "direction.h" // Unused.
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

	while (!foundHerd)
	{
		// Look in all directions for herd
		const Turn turns[] = { Turn::k0, Turn::kForward, Turn::kLeft , Turn::kRight }; // minor suggestion. Might be easier to maintain if you don't hardcode the size in case it changes.
		for (Turn turn : turns) 
		{ // consistent bracing style.
			const Preserve::Feature feature{ look(turn) };

			if (feature == Preserve::Feature::kHerd)
			{
				foundHerd = true;
				break;
			}
		}
		if (foundHerd)
		{
			break;
		}
		
		// survive
		const Preserve::Feature feature = look();
		survive(feature);

		// move!
		moveTowardsHerd();
	}
}

void Elephant::survive(const Preserve::Feature feature) // parameter isn't changing and a reference is more expensive. 
{
	// Deal with hunger // Note: Spaces after comment like so: // helps the human reader. //This is harder to read.
	if (isHungry() && feature == Preserve::Feature::kGrass)
	{
		eat(); // eat grass // Comments like this a little obvious. Consider using comments more sparingly. 
	}
	if (isHungry() && look(Turn::kForward) == Preserve::Feature::kBrush)
	{
		eat(); // eat brush
	}

	// Deal with thirst
	if (isThirsty() && feature == Preserve::Feature::kWater)
	{
		drink();
	}

	// Sleep if sleepy
	if (isSleepy())
	{
		sleep();
	}
}

void Elephant::moveTowardsHerd()
{
	// Face towards herd
	faceHerd();

	// detect if obstacles are in front of elephant
	if (look(Turn::kForward) == Preserve::Feature::kRock ||
		look(Turn::kForward) == Preserve::Feature::kBrush)
	{
		// Check obstacle left
		if (look(Turn::kLeft) == Preserve::Feature::kRock ||
			look(Turn::kLeft) == Preserve::Feature::kBrush)
		{
			// check obstacle right
			if (look(Turn::kRight) == Preserve::Feature::kRock ||
				look(Turn::kRight) == Preserve::Feature::kBrush)
			{
				// elephant is stuck in a U
				// move out of U to the right
				// may not work in all circumstances

				turn(Turn::kRight);
				turn(Turn::kRight);
				move();
				turn(Turn::kRight);
				move();
				move();
				turn(Turn::kLeft);
				move();
			}
			else
			{
				turn(Turn::kRight);
				move();
				turn(Turn::kLeft);
				move();
				move();
				turn(Turn::kLeft);
				move();
			}
		}
		else
		{
			// move around object in front
			turn(Turn::kLeft);
			move();
			turn(Turn::kRight);
			move();
			move();
			turn(Turn::kRight);
			move();
		}
	}
	else// no obstacles
	{
		// just move forward 
		move();
	}
}