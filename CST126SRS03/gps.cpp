#include "stdafx.h"

#include <cassert>
#include <cmath>

#include "gps.h"

GPS::GPS(const int lat, const int lng): lat_(lat), lng_(lng)
{
}

void GPS::set(const int lat, const int lng)
{
	lat_ = lat;
	lng_ = lng;
}

int GPS::getlat() const
{
	return lat_;
}

int GPS::getlng() const
{
	return lng_;
}

void GPS::move(const int direction, const unsigned distance)
{
	switch (distance)
	{
	case 0:
	break;

	case 1:
	{
		const auto cardinal = GPS::cardinal(direction);

		switch (cardinal)
		{
		case kNorth:
			--lat_;
			break;

		case kEast:
			++lng_;
			break;

		case kSouth:
			++lat_;
			break;

		case kWest:
			--lng_;
			break;

		default:
			assert(false);
			break;
		}
	}
	break;

	default:
		assert(false);
	break;
	}
}

int GPS::theta(const int x, const int y)
{
	int result = 0;
	if (x == 0)
	{
		if (y < 0)
		{
			result = 270;
		}
		else if (y > 0)
		{
			result = 90;
		}
	}
	else
	{
		constexpr auto kPi{ 3.14159265358979323846 };

		result = std::lround(atan(1.0 * y / x) * 180.0 / kPi);
		if (x < 0)
		{
			result += 180;
		}
		else if (y < 0)
		{
			result += 360;
		}
	}

	return result;
}

Direction GPS::cardinal(const int direction)
{
	auto result{ kNorth };

	const auto theta = rangeTheta(direction);

	if (theta >= 315 || theta < 45)
	{
		result = kNorth;
	}
	else if (theta >= 45 && theta < 135)
	{
		result = kEast;
	}
	else if (theta >= 135 && theta < 225)
	{
		result = kSouth;
	}
	else if (theta >= 225 && theta < 315)
	{
		result = kWest;
	}
	else
	{
		assert(false);
	}

	return result;
}

int GPS::rangeTheta(const int theta)
{
	const auto result{ (360 * (1 - theta / 360) + theta) % 360 };
	return result;
}
