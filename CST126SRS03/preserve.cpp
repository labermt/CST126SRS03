#include "stdafx.h"

#include <cassert>
#include "gps.h"
#include "loxodonta.h"
#include "preserve.h"

Preserve::Preserve(): 
	feature_
	{
		{ Feature::kRock, Feature::kRock,  Feature::kRock,  Feature::kRock,  Feature::kRock, Feature::kRock,  Feature::kRock,  Feature::kRock,  Feature::kRock,  Feature::kRock },
		{ Feature::kRock, Feature::kDirt,  Feature::kDirt,  Feature::kWater, Feature::kDirt, Feature::kDirt,  Feature::kDirt,  Feature::kWater, Feature::kDirt,  Feature::kRock },
		{ Feature::kRock, Feature::kDirt,  Feature::kDirt,  Feature::kDirt,  Feature::kDirt, Feature::kDirt,  Feature::kBrush, Feature::kDirt,  Feature::kDirt,  Feature::kRock },
		{ Feature::kRock, Feature::kDirt,  Feature::kBrush, Feature::kDirt,  Feature::kDirt, Feature::kRock,  Feature::kWater, Feature::kDirt,  Feature::kDirt,  Feature::kRock },
		{ Feature::kRock, Feature::kWater, Feature::kDirt,  Feature::kRock,  Feature::kDirt, Feature::kBrush, Feature::kDirt,  Feature::kDirt,  Feature::kDirt,  Feature::kRock },
		{ Feature::kRock, Feature::kDirt,  Feature::kDirt,  Feature::kWater, Feature::kDirt, Feature::kDirt,  Feature::kDirt,  Feature::kDirt,  Feature::kRock,  Feature::kRock },
		{ Feature::kRock, Feature::kBrush, Feature::kDirt,  Feature::kDirt,  Feature::kDirt, Feature::kDirt,  Feature::kDirt,  Feature::kWater, Feature::kDirt,  Feature::kRock },
		{ Feature::kRock, Feature::kDirt,  Feature::kDirt,  Feature::kDirt,  Feature::kDirt, Feature::kWater, Feature::kDirt,  Feature::kDirt,  Feature::kBrush, Feature::kRock },
		{ Feature::kRock, Feature::kDirt,  Feature::kDirt,  Feature::kDirt,  Feature::kDirt, Feature::kDirt,  Feature::kDirt,  Feature::kDirt,  Feature::kWater, Feature::kRock },
		{ Feature::kRock, Feature::kRock,  Feature::kRock,  Feature::kRock,  Feature::kRock, Feature::kRock,  Feature::kRock,  Feature::kRock,  Feature::kRock,  Feature::kRock }
	}, // TODO: Generate?
	herd_(1, 5)
{
}

Preserve::Feature Preserve::getFeature(const int lat, const int lng) const
{
	auto result = Feature::kRock;

	if (lat >= 0 && lat < latExtent && lng >= 0 && lng < lngExtent)
	{
		result = feature_[lat][lng];

		const auto herdLat{ herd_.getlat() };
		const auto herdLng{ herd_.getlng() };

		if (lat == herdLat && lng == herdLng)
		{
			result = Feature::kHerd;
		}
	}

	return result;
}

Preserve::Feature Preserve::getFeature(const GPS gps) const
{
	const auto lat{ gps.getlat() };
	const auto lng{ gps.getlng() };
	const auto result = getFeature(lat, lng);

	return result;
}

Preserve::Feature Preserve::getFeature(const Loxodonta& loxodonta) const
{
	auto result = Feature::kUnknown;

	const auto gpsPtr{ loxodonta.getGps_() };

	if (gpsPtr != nullptr)
	{
		result = getFeature(*gpsPtr);
	}

	return result;
}

void Preserve::setFeature(const int lat, const int lng, const Feature feature)
{
	if (lat >= 0 && lat < latExtent && lng >= 0 && lng < lngExtent)
	{
		feature_[lat][lng] = feature;
	}
	else
	{
		assert(false);
	}
}

void Preserve::setFeature(const GPS gps, const Feature feature)
{
	const auto lat{ gps.getlat() };
	const auto lng{ gps.getlng() };
	setFeature(lat, lng, feature);
}

int Preserve::getHerdDirection(const Loxodonta& loxodonta) const
{
	auto result = 360;

	const auto gpsPtr{ loxodonta.getGps_() };

	if (gpsPtr != nullptr)
	{
		const auto dx = herd_.getlng() - gpsPtr->getlng();
		const auto dy = -(herd_.getlat() - gpsPtr->getlat());
		const auto angle = 90- GPS::theta(dx, dy);
		result = GPS::cardinal(angle); // TODO: Use cardinal compass coordinates?
	}
	return result;
}

void Preserve::killGrass(const GPS gps)
{
	const auto feature{ getFeature(gps) };
	if (feature == Feature::kGrass)
	{
		setFeature(gps, Feature::kDirt);
	}
}

bool Preserve::isObstacle(const Feature feature)
{
	bool result{ true };

	switch (feature)
	{
	case Feature::kUnknown:
	case Feature::kRock:
	case Feature::kBrush:
		result = true;
		break;

	case Feature::kHerd:
	case Feature::kDirt:
	case Feature::kGrass:
	case Feature::kWater:
		result = false;
		break;

	default:
		assert(false);
		break;
	}

	return result;
}