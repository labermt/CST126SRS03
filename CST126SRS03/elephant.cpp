#include "stdafx.h"
#include "elephant.h"
#include "preserve.h"
#include "direction.h"
#include "gps.h"

void Elephant::tag(GPS& gps)
{
	Loxodonta::setGps(gps); 
}

void Elephant::findHerd()
{
	while (look() != Preserve::Feature::kHerd)
	{
		auto destination = Loxodonta::listen();

		if (destination == 0)
		{
			auto lookingAt = look(Turn::kForward);
			if (lookingAt == Preserve::Feature::kRock || lookingAt == Preserve::Feature::kBrush)
				dodgeObstacle(Loxodonta::Turn::kForward);
			else
				Loxodonta::move();
		}
		else if (destination == 90)
		{
			auto lookingAt = look(Turn::kRight);
			if (lookingAt == Preserve::Feature::kRock || lookingAt == Preserve::Feature::kBrush)
				dodgeObstacle(Loxodonta::Turn::kRight);
			else
				turn(Turn::kRight);
		}
		else if (destination == 270)
		{
			auto lookingAt = look(Turn::kLeft);
			if (lookingAt == Preserve::Feature::kRock || lookingAt == Preserve::Feature::kBrush)
				dodgeObstacle(Loxodonta::Turn::kLeft);
			else
				turn(Turn::kLeft);
		}
		else
		{
			turn(Turn::kLeft);
		}
		Loxodonta::move();
	}
}

void Elephant::dodgeObstacle(Loxodonta::Turn heading) //I've got it set up, but it doesn't like it for some reasons, going to check on that later.
{
	switch (heading) 
	{
		case Loxodonta::Turn::kForward : turn(Loxodonta::Turn::kLeft);
		case Loxodonta::Turn::kRight : Loxodonta::move();
			turn(Loxodonta::Turn::kRight);
			while (Loxodonta::look(Loxodonta::Turn::kRight) != Preserve::Feature::kRock || Elephant::Loxodonta::look(Loxodonta::Turn::kRight) != Preserve::Feature::kBrush)
				Loxodonta::move();
			break;
		case Loxodonta::Turn::kLeft : Elephant::move();
			turn(Loxodonta::Turn::kLeft);
			while (look(Loxodonta::Turn::kLeft) != Preserve::Feature::kRock || look(Loxodonta::Turn::kLeft) != Preserve::Feature::kBrush)
				Loxodonta::move();
			break;
	}
}