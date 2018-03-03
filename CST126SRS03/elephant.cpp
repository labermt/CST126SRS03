#include "stdafx.h"
#include "elephant.h"
#include <cassert>

// Methods to implement in heirarchal order: isStuck()

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
				//can be replaced with isStuck() \/
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
				//can be replaced with isStuck() \/
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
				//Herd is behind Elephant 

					turn(Turn::kLeft);
					turn(Turn::kLeft);
			}
		}
		else //Moving towards herd already
		{
			if(movable(look(Turn::kForward)))
			{
				move();
			}
			//else isStuck() can replace this \/
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

		if (!movable(look(Turn::kForward)))//confirms initial obstacle
		{
			resolve = 1; //moves to resolve case wall
		}
		while (stuck)
		{
			switch (resolve)
			{
			case wall:
			{
				if (!movable(look(Turn::kLeft)) || !movable(look(Turn::kRight)))
				{
					resolve = 2; //might be a diagonal, moves to diagonal case
					break;
				}
				else //is a wall
				{
					turn(Turn::kLeft);
					move();

					auto unresolved = true;

					while (unresolved) //loop to follow wall until reaching an opening
					{
						if (!movable(look(Turn::kRight)))
						{
							move();
						}
						else
						{
							turn(Turn::kRight);
							move();
							unresolved = false;
						}
					}
					break;
				}
			}
			case diagonal:
			{
				if (!movable(look(Turn::kLeft)) && !movable(look(Turn::kRight)))
				{
					assert(false); //not just a diagonal, moves to hall case
					break;
				}
				else //is a diagonal
				{
					if (!movable(look(Turn::kLeft))) //can move right
					{
						turn(Turn::kRight);
						move();

						if(!movable(look(Turn::kLeft)))
						{
							turn(Turn::kRight);
							move();
						}
						else
						{
							turn(Turn::kLeft);
							move();
						}
					}
					else //can move left
					{
						turn(Turn::kLeft);
						move();

						if (!movable(look(Turn::kRight)))
						{
							turn(Turn::kLeft);
							move();
						}
						else
						{
							turn(Turn::kRight);
							move();
						}
					}
					break;
				}
			}
			/*case hall:
			{
				turn(Turn::kLeft); // turns around moves out of end of hall 
				turn(Turn::kLeft);
				move();

				if (!movable(look(Turn::kLeft))) //check for continuous hall
				{
					
				}
			} */
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