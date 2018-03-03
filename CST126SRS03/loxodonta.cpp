#include "stdafx.h"
#include <cassert>
#include "gps.h"
#include "preserve.h"
#include "loxodonta.h"

Loxodonta::Loxodonta(const unsigned weight, const Direction heading):
	weight_{ weight }, heading_{ heading }, minWeight_{ weight - 200 }, maxWeight_{ weight + 100 }
{
}

Preserve::Feature Loxodonta::look(const Loxodonta& loxodonta)
{
	const auto result{ Preserve::getInstance().getFeature(loxodonta) };
	return result;
}

void Loxodonta::setGps(GPS & gps)
{
	gps_ = &gps;
}

GPS* Loxodonta::getGps_() const
{
	return gps_;
}

unsigned Loxodonta::getElapsedTime() const
{
	return elapsedTime_;
}

unsigned Loxodonta::getAwake() const
{
	const unsigned result{ awake_ };
	return result;
}

unsigned Loxodonta::getWater() const
{
	const unsigned result{ water_ };
	return result;
}

unsigned Loxodonta::getWeight() const
{
	const unsigned result{ weight_ };
	return result;
}

unsigned Loxodonta::getMinWeight() const
{
	const unsigned result{ minWeight_ };
	return result;
}

unsigned Loxodonta::getMaxWeight() const
{
	const unsigned result{ maxWeight_ };
	return result;
}

void Loxodonta::incrementTime(const unsigned minutes)
{
	auto min = minutes;

	if (isSleepy())
	{
		min *= 3;
	}

	if (isThirsty())
	{
		min *= 4;
	}

	if (isHungry())
	{
		min *= 2;
	}

	elapsedTime_ += min;
	awake_ += min;
}

void Loxodonta::decrementWater(const unsigned liters)
{
	int water = water_ - liters;
	if (water < 0)
	{
		water = 0;
	}
	water_ -= water;
}

void Loxodonta::decrementWeight(const unsigned kg)
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

bool Loxodonta::isSleepy() const
{
	const auto result{ awake_ >= 60*kMaxAwake };
	return result;
}

bool Loxodonta::isThirsty() const
{
	const auto result{ water_ == 0 };
	return result;
}

bool Loxodonta::isHungry() const
{
	const auto result{ weight_ < minWeight_ };
	return result;
}

Preserve::Feature Loxodonta::look() const
{
	const auto result{ look(*this) };
	return result;
}

Direction Loxodonta::getDirection(const Turn turn)
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

Direction Loxodonta::getHeading(const Turn turn) const
{
	const auto result{ GPS::cardinal(heading_ + getDirection(turn)) };
	return result;
}

Direction Loxodonta::getHeading() const
{
	const auto result{ getHeading(Turn::kForward) };
	return result;
}

Preserve::Feature Loxodonta::look(const Turn turn) const
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

int Loxodonta::listen() const
{
	const auto result{ Preserve::getInstance().getHerdDirection(*this) };
	return result;
}

void Loxodonta::sleep()
{
	incrementTime(120);
	awake_ = 0;
}

void Loxodonta::drink()
{
	incrementTime(5);

	const auto feature{ look() };
	if (feature == Preserve::Feature::kWater)
	{
		water_ = kMaxWater;
	}
}

void Loxodonta::eat()
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

void Loxodonta::turn(const Turn turn)
{
	incrementTime(1);
	heading_ = getHeading(turn);
}

void Loxodonta::move()
{
	incrementTime(60);
	decrementWater(10);
	decrementWeight(20);
	if (gps_ != nullptr)
	{
		const Preserve::Feature feature{ look(Turn::kForward) };
		if (!Preserve::isObstacle(feature))
		{
			gps_->move(heading_, 1);
		}
	}
}

void Loxodonta::faceHerd()
{
	const Direction heading{ getHeading() };
	const int herdDirection{ listen() };
	const auto deltaAngle{ herdDirection - heading };

	const auto deltaTheta = GPS::rangeTheta(deltaAngle);

	if (deltaTheta >= 315 || deltaTheta < 45)
	{
		turn(Turn::kForward);
	}
	else if (deltaTheta >= 45 && deltaTheta < 135)
	{
		turn(Turn::kRight);
	}
	else if (deltaTheta >= 135 && deltaTheta < 225)
	{
		turn(Turn::kRight);
		turn(Turn::kRight);
	}
	else if (deltaTheta >= 225 && deltaTheta < 315)
	{
		turn(Turn::kLeft);
	}
	else
	{
		assert(false);
	}
}
