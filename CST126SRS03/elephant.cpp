#include "stdafx.h"
#include "elephant.h"
#include <cassert>

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

		if(isSleepy())
		{
			sleep();
		}

		auto const herd_dir = listen();
		auto const heading = getHeading(Turn::kForward);
		
		if (heading != herd_dir) //Elephant listens and if not heading herd_dir, turns and moves to herd_dir
		{
			if (getHeading(Turn::kRight) == herd_dir)
			{
				if (movable(look(Turn::kRight)))
				{
					turn(Turn::kRight);
					move();
				}
				else
				{
					turn(Turn::kRight);
					isStuck();
				}
			}
			else if (getHeading(Turn::kLeft) == herd_dir)
			{
				if (movable(look(Turn::kLeft)))
				{
					turn(Turn::kLeft);
					move();
				}
				else
				{
					turn(Turn::kLeft);
					isStuck();
				}
			}
			else //Herd is not left or right. Herd is behind elephant, elephant turns around
			{
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
			else
			{
				isStuck();
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

		auto solving = false;

		if (!movable(look(Turn::kForward))) //confirms initial obstacle and/or post solve obstacle
		{
			if (getHeading(Turn::kForward) != listen()) //checks that obstacle is actually blocking elephants path to the herd
			{
				stuck = false;
			}
			else
			{
				resolve = wall; //moves to resolve case wall

				solving = stuck;
			}
		}
		else
		{
			stuck = false;
		}

		while (solving)
		{
			switch (resolve)
			{
			case wall:
			{
				if (!movable(look(Turn::kLeft)) || !movable(look(Turn::kRight)))
				{
					resolve = diagonal; //might be a diagonal, moves to diagonal case
				}
				else //is a wall
				{
					auto unresolved = true;

					turn(Turn::kRight);
					move();

					while (unresolved) //loop to follow wall until reaching an opening
					{
						if (!movable(look(Turn::kLeft)) && movable(look(Turn::kForward))) //no opening in wall
						{
							move();
						}
						else if (!movable(look(Turn::kLeft)) && !movable(look(Turn::kForward))) //hit a corner
						{
							resolve = diagonal;
							unresolved = false;
						}
						else //opening in wall reached
						{
							turn(Turn::kLeft);
							move();

							if (movable(look(Turn::kForward)))
							{
								move();
								unresolved = false;
								solving = false;
							}
							else
							{
								resolve = diagonal;
								unresolved = false;
							}
						}
					}
				}
				break;
			}
			case diagonal:
			{
				if (!movable(look(Turn::kLeft)) && !movable(look(Turn::kRight)))
				{
					resolve = hall; //not just a diagonal, moves to hall case
				}
				else //is a diagonal
				{
					if (!movable(look(Turn::kLeft))) //can move right
					{
						auto unresolved = true;

						while (unresolved)
						{
							if (!movable(look(Turn::kForward)) && movable(look(Turn::kRight)))
							{
								turn(Turn::kRight);
								move();

								if (!movable(look(Turn::kLeft))) //wall
								{
									turn(Turn::kLeft);
									resolve = wall;
									unresolved = false;
								}
								else
								{
									turn(Turn::kLeft);
									move();
								}
							}
							else
							{
								move();
								unresolved = false;
								solving = false;
							}
						}
					}
					else //can move left
					{
						auto unresolved = true;

						while (unresolved)
						{
							if (!movable(look(Turn::kForward)) && movable(look(Turn::kLeft)))
							{
								turn(Turn::kLeft);
								move();

								if (!movable(look(Turn::kRight))) //wall
								{
									turn(Turn::kRight);
									resolve = wall;
									unresolved = false;
								}
								else
								{
									turn(Turn::kRight);
									move();
								}
							}
							else
							{
								move();
								unresolved = false;
								solving = false;
							}
						}
					}
					break;
				}
			}
			case hall:
			{
				auto unresolved = true;

				turn(Turn::kLeft); //turns around moves out of end of hall dead end
				turn(Turn::kLeft);
				move();

				while (unresolved)
				{
					if (!movable(look(Turn::kLeft)) && !movable(look(Turn::kRight))) //continuous hall on both sides
					{
						if (!movable(look(Turn::kForward)))
						{
							assert(false); //This should NEVER occur. (If it does, somehow the elephant has clipped into an area it should not be in or it started out completely surrounded)
						}
						else
						{
							move();
						}
					}
					else if (movable(look(Turn::kLeft))) //found opening in new left side of hall
					{
						turn(Turn::kLeft);
						move();

						if (!movable(look(Turn::kForward)))
						{
							if(!movable(look(Turn::kRight)))
							{
								turn(Turn::kLeft);
								turn(Turn::kLeft);
								move();

								if (!movable(look(Turn::kForward)))
								{
									turn(Turn::kLeft);
									move();
								}
								else
								{
									move();

									if (!movable(look(Turn::kForward)))
									{
										turn(Turn::kLeft);
										turn(Turn::kLeft);
										move();
										turn(Turn::kRight);
										move();
									}
									else
									{
										move();
										unresolved = false;
										solving = false;
									}
								}
							}
							else
							{
								move();
								unresolved = false;
								solving = false;
							}
						}
						else
						{
							move();
							unresolved = false;
							solving = false;
						}
					}
					else //found opening in new right side of hall
					{
						turn(Turn::kRight);
						move();

						if (!movable(look(Turn::kForward)))
						{
							if (!movable(look(Turn::kLeft)))
							{
								turn(Turn::kLeft);
								turn(Turn::kLeft);
								move();

								if (!movable(look(Turn::kForward)))
								{
									turn(Turn::kRight);
									move();
								}
								else
								{
									move();

									if (!movable(look(Turn::kForward)))
									{
										turn(Turn::kLeft);
										turn(Turn::kLeft);
										move();
										turn(Turn::kLeft);
										move();
									}
									else
									{
										move();
										unresolved = false;
										solving = false;
									}
								}
							}
							else
							{
								move();
								unresolved = false;
								solving = false;
							}
						}
						else
						{
							move();
							unresolved = false;
							solving = false;
						}
					}
				}
				break;
			} 
			default: //elephant has no obstacle in its way
			{
				solving = false;
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