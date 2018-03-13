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

void Elephant::findHerd()
{
	auto found_heard = false;
	if (look() == Preserve::Feature::kHerd)
	{
		found_heard = true;
	}

	while (!found_heard)
	{
		if (isThirsty() && look() == Preserve::Feature::kWater)
		{
			drink();
		}

		if (getWeight() <= (getMaxWeight() / 2) && look() == Preserve::Feature::kGrass || look(Turn::kForward) == Preserve::Feature::kBrush)
		{
			eat();
		}

		if (isSleepy())
		{
			sleep();
		}

		auto const herd_dir = listen();
		auto const heading = getHeading(Turn::kForward);

		if (heading != herd_dir)
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
			else
			{
				turn(Turn::kLeft);
				turn(Turn::kLeft);
			}
		}
		else
		{
			if (movable(look(Turn::kForward)))
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

		if (!movable(look(Turn::kForward)))
		{
			if (getHeading(Turn::kForward) != listen())
			{
				stuck = false;
			}
			else
			{
				resolve = wall;

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
					resolve = diagonal;
				}
				else
				{
					auto unresolved = true;

					turn(Turn::kRight);
					move();

					while (unresolved)
					{
						if (!movable(look(Turn::kLeft)) && movable(look(Turn::kForward)))
						{
							move();
						}
						else if (!movable(look(Turn::kLeft)) && !movable(look(Turn::kForward)))
						{
							resolve = diagonal;
							unresolved = false;
						}
						else
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
					resolve = hall;
					// Keep on going to the next case?
				}
				else
				{
					if (!movable(look(Turn::kLeft)))
					{
						auto unresolved = true;

						while (unresolved)
						{
							if (!movable(look(Turn::kForward)) && movable(look(Turn::kRight)))
							{
								turn(Turn::kRight);
								move();

								if (!movable(look(Turn::kLeft)))
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
					else
					{
						auto unresolved = true;

						while (unresolved)
						{
							if (!movable(look(Turn::kForward)) && movable(look(Turn::kLeft)))
							{
								turn(Turn::kLeft);
								move();

								if (!movable(look(Turn::kRight)))
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
					break; // ??? 
				}
				// continue to the hall? Seems a bit complex. Try to the the logic flow normally when possible. 
			}

			case hall:
			{
				auto unresolved = true;

				turn(Turn::kLeft);
				turn(Turn::kLeft);
				move();

				while (unresolved)
				{
					if (!movable(look(Turn::kLeft)) && !movable(look(Turn::kRight)))
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
					else if (movable(look(Turn::kLeft)))
					{
						turn(Turn::kLeft);
						move();

						if (!movable(look(Turn::kForward)))
						{
							if (!movable(look(Turn::kRight)))
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
					else
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
			}
			break;

			default:
			{
				solving = false;
			}
			break; // Keep a break outside any nesting for readablity. 
			}
		}
	}
}

void Elephant::tag(class GPS& gps)
{
	setGps(gps);
}
