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

Direction Elephant::getHeading(const Turn turn) const
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

	const auto result{ GPS::cardinal(heading_ + theta) };

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
			distance = 1;
			break;

		case Turn::kLeft:
			distance = 1;
			break;

		case Turn::kRight:
			distance = 1;
			break;

		default:
			assert(false);
			break;
		}

		auto gpsLocation = *gps_;
		const auto delta{ getHeading(turn) };
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

void  Elephant::turn(const Turn turn)
{
	incrementTime(1);
	heading_ = getHeading(turn);
}

void  Elephant::move()
{
	incrementTime(60);
	decrementWater(20);
	decrementWeight(20);
	if (gps_ != nullptr)
	{
		gps_->move(heading_, 1);
	}
}
