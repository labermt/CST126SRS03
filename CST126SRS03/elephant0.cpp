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
	awake_ = 0;
}

void Elephant::drink()
{
	const auto feature{ look() };

	if (feature == Preserve::Feature::kWater)
	{
		water_ = kMaxWater;
	}
}

void Elephant::eat()
{
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
	heading_ = getHeading(turn);
}

void  Elephant::move()
{
	if (gps_ != nullptr)
	{
		// TODO: Add awake time, lose water and weight. 
		// TODO: If awake too long, slow down.
		// TODO: If dehydrated, slow down.
		// TODO: If below weight, slow down.
		// TODO: Move to the next GPS.
		gps_->move(heading_, 1);
	}
}

