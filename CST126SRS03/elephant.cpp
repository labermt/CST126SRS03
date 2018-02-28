#include "stdafx.h"
#include "elephant.h"
#include "preserve.h"
#include "direction.h"
#include "gps.h"

void dodgeObstacle();

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
				dodgeObstacle();
			else
				Elephant::move();
		}
		else if (destination == 90)
		{
			auto lookingAt = look(Turn::kRight);
			if (lookingAt == Preserve::Feature::kRock || lookingAt == Preserve::Feature::kBrush)
				dodgeObstacle();
			else
				turn(Turn::kRight);
		}
		else if (destination == 270)
		{
			auto lookingAt = look(Turn::kLeft);
			if (lookingAt == Preserve::Feature::kRock || lookingAt == Preserve::Feature::kBrush)
				dodgeObstacle();
			else
				turn(Turn::kLeft);
		}
		else
		{
			turn(Turn::kLeft);
		}
	}
}

void dodgeObstacle() //My new working project
{
	if (lookingAt == Preserve::Feature::kRock || lookingAt == Preserve::Feature::kBrush)
	{
		if (lookLeft == Preserve::Feature::kRock || lookingAt == Preserve::Feature::kBrush)
		{
			turn(Turn::kRight);
			turn(Turn::kRight);
			lookingAt = look(Turn::kForward);
			if (lookingAt == Preserve::Feature::kRock || lookingAt == Preserve::Feature::kBrush)
				bool iMightBeStuck = true;
			else
		}
	}
}