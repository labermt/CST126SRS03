#include "stdafx.h"

#include <cassert>
#include "gps.h"
#include "preserve.h"
#include "elephant.h"

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
	} // TODO: Generate?
{
}

Preserve::Feature Preserve::getFeature(const int lat, const int lng) const
{
	auto result = Feature::kRock;
	if (lat >= 0 && lat < latExtent && lng >= 0 && lng < lngExtent)
	{
		result = feature_[lat][lng];
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

Preserve::Feature Preserve::getFeature(const Elephant& elephant) const
{
	auto result = Feature::kUnknown;

	const auto gpsPtr{ elephant.getGps_() };

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

int Preserve::getHerdDirection(const Elephant& elephant) const
{
	auto result = 360;

	const auto gpsPtr{ elephant.getGps_() };

	if (gpsPtr != nullptr)
	{
		const auto latDelta = herd_.getlat() - gpsPtr->getlat();
		const auto lngDelta = herd_.getlng() - gpsPtr->getlng();
		result = GPS::theta(lngDelta, latDelta);
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
