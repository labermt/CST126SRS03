#include "stdafx.h"
#include "elephant.h"
#include "preserve.h"
#include "direction.h"
#include "gps.h"

void dodgeObstacle(Loxodonta::Turn heading);

void Elephant::tag(GPS& gps)
{
	//The GPS tagging method was changed recently. We can work on that problem later.
}

void Elephant::findHerd()
{
	while (look() != Preserve::Feature::kHerd)
	{
		auto destination = Elephant::listen();

		if (destination == 0)
		{
			auto lookingAt = look(Turn::kForward);
			if (lookingAt == Preserve::Feature::kRock || lookingAt == Preserve::Feature::kBrush)
				dodgeObstacle(Loxodonta::Turn::kForward);
			else
				Elephant::move();
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
	}
}

void dodgeObstacle(Loxodonta::Turn heading) //My new working project
{
	switch (heading) {
	case Loxodonta::Turn::kForward : Loxodonta::Preserve::Feature::turn(Turn::kLeft);
	case Loxodonta::Turn::kRight : Loxodonta::move();
		turn(Loxodonta::Turn::kRight);
		while (Loxodonta:: Preserve::Feature:: look(Loxodonta::Turn::kRight) != Preserve::Feature::kRock || look(Loxodonta::Turn::kRight) != Preserve::Feature::kBrush)
			Elephant::move();
		break;
	case Loxodonta::Turn::kLeft : Elephant::move();
		turn(Loxodonta::Turn::kLeft);
		while (look(Loxodonta::Turn::kLeft) != Preserve::Feature::kRock || look(Loxodonta::Turn::kLeft) != Preserve::Feature::kBrush)
			Elephant::move();
		break;
	}
}