#include "stdafx.h"
#include <cassert>
#include "gps.h"
#include "preserve.h"
#include "elephant.h"

Elephant::Elephant(const unsigned weight, const Direction heading):
	weight_{ weight }, heading_{ heading }, minWeight_{ weight - 200 }, maxWeight_{ weight + 100 }
{
}

Preserve::Feature Elephant::look(const Elephant& elephant)
{
	const auto result{ Preserve::getInstance().getFeature(elephant) };
	return result;
}

GPS* Elephant::getGps_() const
{
	return gps_;
}

unsigned Elephant::getElapsedTime() const
{
	return elapsedTime_;
}

unsigned Elephant::getAwake() const
{
	const unsigned result{ awake_ };
	return result;
}

unsigned Elephant::getWater() const
{
	const unsigned result{ water_ };
	return result;
}

unsigned Elephant::getWeight() const
{
	const unsigned result{ weight_ };
	return result;
}

unsigned Elephant::getMinWeight() const
{
	const unsigned result{ minWeight_ };
	return result;
}

unsigned Elephant::getMaxWeight() const
{
	const unsigned result{ maxWeight_ };
	return result;
}

void Elephant::incrementTime(const unsigned minutes)
{
	auto min = minutes;

	if (isSleepy())
	{
		min *= 2;
	}

	if (isThirsty())
	{
		min *= 2;
	}

	if (isHungry())
	{
		min *= 2;
	}

	elapsedTime_ += minutes;
	awake_ += minutes;
}

void Elephant::decrementWater(const unsigned liters)
{
	int water = water_ - liters;
	if (water < 0)
	{
		water = 0;
	}
	water_ -= water;
}

void Elephant::decrementWeight(const unsigned kg)
{
	if (weight_ >= minWeight_)
	{
		int weight = weight_ - kg;
		if (weight < 0)
		{
			weight = 0;
		}
		weight_ = weight;
	}
}

bool Elephant::isSleepy() const
{
	const auto result{ awake_ >= kMaxAwake };
	return result;
}

bool Elephant::isThirsty() const
{
	const auto result{ water_ == 0 };
	return result;
}

bool Elephant::isHungry() const
{
	const auto result{ weight_ < minWeight_ };
	return result;
}

Preserve::Feature Elephant::look() const
{
	const auto result{ look(*this) };
	return result;
}

Direction Elephant::getDirection(const Turn turn)
{
	auto theta{ 0 };

	switch (turn)
	{
	case Turn::k0:
	case Turn::kForward:
		theta = 0;
		break;

	case Turn::kLeft:
		theta = -90;
		break;

	case Turn::kRight:
		theta = +90;
		break;

	default:
		assert(false);
		break;
	}

	const auto result{ GPS::cardinal(theta) };

	return result;
}

Direction Elephant::getHeading(const Turn turn) const
{
	const auto result{ GPS::cardinal(heading_ + getDirection(turn)) };
	return result;
}

Preserve::Feature Elephant::look(const Turn turn) const
{
	auto result{ Preserve::Feature::kUnknown };

	if (gps_ != nullptr)
	{
		auto distance{ 0 };

		switch (turn)
		{
		case Turn::k0:
			distance = 0;
			break;

		case Turn::kForward:
		case Turn::kLeft:
		case Turn::kRight:
			distance = 1;
			break;

		default:
			assert(false);
			break;
		}

		auto gpsLocation = *gps_;
		const int delta{ getDirection(turn) };
		gpsLocation.move(heading_ + delta, distance);
		result = Preserve::getInstance().getFeature(gpsLocation);
	}

	return result;
}

int Elephant::listen() const
{
	const auto result{ Preserve::getInstance().getHerdDirection(*this) };
	return result;
}

void Elephant::sleep()
{
	incrementTime(120);
	awake_ = 0;
}

void Elephant::drink()
{
	const auto feature{ look() };

	incrementTime(5);
	if (feature == Preserve::Feature::kWater)
	{
		water_ = kMaxWater;
	}
}

void Elephant::eat()
{
	incrementTime(15);
	if (gps_ != nullptr)
	{
		const auto feature{ look() };

		if (feature == Preserve::Feature::kGrass)
		{
			weight_ += 100;
			Preserve::getInstance().killGrass(*gps_);
		}
		else if (look(Turn::kForward) == Preserve::Feature::kBrush)
		{
			weight_ += 10;
		}

		weight_ = weight_ > maxWeight_ ? maxWeight_ : weight_;
	}
}

void Elephant::turn(const Turn turn)
{
	incrementTime(1);
	heading_ = getHeading(turn);
}

void Elephant::move()
{
	incrementTime(60);
	decrementWater(20);
	decrementWeight(20);
	if (gps_ != nullptr)
	{
		gps_->move(heading_, 1);
	}
}

void Elephant::tag(GPS& gps)
{
	gps_ = &gps;
}

void Elephant::findHerd()
{
	auto feature = look();
	while (feature != Preserve::Feature::kHerd)
	{
		if (feature == Preserve::Feature::kUnknown)
		{
			assert(false); // Let Mitch know
		}

		const auto focusDirection = listen();
		/*if (weight_ - minWeight_ < 80)
		{
		if (feature == Preserve::Feature::kGrass)
		{
		eat();
		}
		else
		{
		auto possibleDirections = { Turn::kLeft, Turn::kForward, Turn::kRight };
		for (auto direction : possibleDirections)
		{
		if (look(direction) == Preserve::Feature::kBrush)
		{
		turn(direction);

		}
		}
		}
		}*/

		auto heading = getHeading(Turn::kForward);
		auto lookForward = look(Turn::kForward);
		auto lookRight = look(Turn::kRight);
		auto lookLeft = look(Turn::kLeft);
		auto forwardClear = lookForward == Preserve::Feature::kGrass || lookForward == Preserve::Feature::kDirt || lookForward == Preserve::Feature::kWater || lookForward == Preserve::Feature::kHerd;
		auto rightClear = lookRight == Preserve::Feature::kGrass || lookRight == Preserve::Feature::kDirt || lookRight == Preserve::Feature::kWater || lookRight == Preserve::Feature::kHerd;
		auto leftClear = lookLeft == Preserve::Feature::kGrass || lookLeft == Preserve::Feature::kDirt || lookLeft == Preserve::Feature::kWater || lookLeft == Preserve::Feature::kHerd;

		const auto directionDifference = getHeading(Turn::kForward) - focusDirection;
		switch ((directionDifference + 360) % 360)
		{
		case 0:
			if (!forwardClear)
			{
				if (rightClear)
				{
					turn(Turn::kRight);
				}
				else if (leftClear)
				{
					turn(Turn::kLeft);
				}
				else
				{
					turn(Turn::kRight);
					turn(Turn::kRight);
				}
			}
			break;
		case 90:
			if (leftClear)
			{
				turn(Turn::kLeft);
			}
			else if (!forwardClear)
			{
				turn(Turn::kLeft);
				lookLeft = look(Turn::kLeft);
				leftClear = lookLeft == Preserve::Feature::kGrass || lookLeft == Preserve::Feature::kDirt || lookLeft == Preserve::Feature::kWater || lookLeft == Preserve::Feature::kHerd;
				if (leftClear)
				{
					turn(Turn::kLeft);
				}
				else
				{
					turn(Turn::kRight);
					turn(Turn::kRight);
				}
			}
			break;
		case 180:
			turn(Turn::kRight);
			turn(Turn::kRight);
			if (!forwardClear)
			{
				if (rightClear)
				{
					turn(Turn::kRight);
				}
				else if (leftClear)
				{
					turn(Turn::kLeft);
				}
				else
				{
					turn(Turn::kRight);
					turn(Turn::kRight);
				}
			}
			break;
		case 270:
			if (rightClear)
			{
				turn(Turn::kRight);
			}
			else if (!forwardClear)
			{
				turn(Turn::kRight);
				lookRight = look(Turn::kRight);
				rightClear = rightClear = lookRight == Preserve::Feature::kGrass || lookRight == Preserve::Feature::kDirt || lookRight == Preserve::Feature::kWater || lookRight == Preserve::Feature::kHerd;
				if (rightClear)
				{
					turn(Turn::kRight);
				}
				else
				{
					turn(Turn::kRight);
					turn(Turn::kRight);
				}
			}
			break;
		default:
			break;
		}
		move();
		feature = look();
	}
}
