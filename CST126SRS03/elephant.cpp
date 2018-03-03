#include "stdafx.h"
#include "elephant.h"
#include <cassert>

// Methods to implement in heirarchal order: isStuck(), eleTracks()
bool movable(Preserve::Feature const terrain);
 /* void elephantTracks()
{



} */

/* void isStuck()
 {
	
//check for first stuck instance (wall)

		loop, follow wall and look while following wall until movable terrain is found then move into that coordinate

//check for second stuck instance (hallway)

		check by looking in all directions til movable is found, then turn til heading movable terrain, then move out and around the U

//check for third stuck instance (diagonal wall)

		loop, follow wall and look while following wall until movable terrain is found then move into that coordinate
		
 } */

bool movable(Preserve::Feature const terrain)
{
	auto result = false;
	switch (terrain)
	{
	case Preserve::Feature::kUnknown:
	case Preserve::Feature::kRock:
	case Preserve::Feature::kBrush:
		break;

	case Preserve::Feature::kDirt:
	case Preserve::Feature::kGrass:
	case Preserve::Feature::kWater:
	case Preserve::Feature::kHerd:
		result = true;
		break;

	default:
		assert(false);
		break;
	}
	return result;
}

void Elephant::findHerd()
{
	auto found_heard = false;
	if(look() == Preserve::Feature::kHerd)
	{
		found_heard = true;
	}
	
	while(!found_heard)
	{
		if (isThirsty() && look() == Preserve::Feature::kWater)
		{
			drink();
		}

		if (getWeight() <= (getMaxWeight() / 2) && look() == Preserve::Feature::kGrass || look(Turn::kForward) == Preserve::Feature::kBrush)
		{
			eat();
		}

		auto const herd_dir = listen();
		auto const heading = getHeading(Turn::kForward);
		
		if (heading != herd_dir) //Elephant listens and if not heading herd_dir, turns and moves to herd_dir
		{
			if (getHeading(Turn::kRight) == herd_dir)
			{
				if(movable(look(Turn::kRight)))
				{
					turn(Turn::kRight);
					move();
				}
				else if(movable(look(Turn::kForward)))
				{
					move();
				}
				else
				{
					turn(Turn::kLeft);
					move();
				}
			}
			else if (getHeading(Turn::kLeft) == herd_dir)
			{
				if(movable(look(Turn::kLeft)))
				{
					turn(Turn::kLeft);
					move();
				}
				else if (movable(look(Turn::kForward)))
				{
					move();
				}
				else
				{
					turn(Turn::kRight);
					move();
				}
			}
			else //Herd is not left or right
			{
				if (movable(look(Turn::kForward))) //herd is forward
				{
					move();
				}
				else //Herd is behind Elephant and or if there is an immovable obj
				{
					//assert(false);

					turn(Turn::kLeft);
					turn(Turn::kLeft);

					//Redetermine herds' location.
					if (heading == herd_dir)
					{
						if (movable(look(Turn::kForward)))
						{
							move();
						}
						else
						{
							assert(false);
						}
					}
					else
					{
						assert(false);

						//isStuck();
					}
				}
			}
		}
		else //Moving towards herd already
		{
			if(movable(look(Turn::kForward)))
			{
				move();
			}
			else if(movable(look(Turn::kRight)))
			{
				turn(Turn::kRight);
				move();
			}
			else if(movable(look(Turn::kLeft)))
			{
				turn(Turn::kLeft);
				move();
			}
			else
			{
				assert(false);

				//isStuck();
			}
		}
		if (look() == Preserve::Feature::kHerd)
		{
			found_heard = true;
		}

	}
}

void Elephant::isStuck()
{
	auto stuck = true;

	while (stuck)
	{
		enum obstacle { none, wall, diagonal, hall };

		auto resolve = 0;

		if (!movable(look(Turn::kForward)))//check for initial obstacle
		{
			resolve = 1;
		}
		while (stuck)
		{
			switch (resolve)
			{
			case wall:
			{
				if (!movable(look(Turn::kLeft)))
				{
					resolve = 2;
					break;
				}
				else
				{
					//is a wall
				}
			}
			case diagonal:
			{
				if (!movable(look(Turn::kLeft)) || !movable(look(Turn::kRight)))
				{
					resolve = 3;
					break;
				}
				else
				{
					//is a diagonal
				}
			}
			case hall:
			{
				turn(Turn::kLeft);
				turn(Turn::kLeft);
				move();
			}
			default:
			{
				stuck = false;
				break;
			}
			}
		}
	}
}

void Elephant::tag(class GPS& gps)
{
	setGps(gps);
}