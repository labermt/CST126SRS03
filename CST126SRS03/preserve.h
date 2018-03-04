#pragma once

#include <type_traits>
#include "gps.h"

class Loxodonta;

class Preserve
{
public:
	enum class Feature
	{
		kUnknown, // Unable to determine
		kHerd,    // The destination
		kDirt,    // Empty land
		kRock,    // Obstacle
		kBrush,   // Obstacle and Food edible if ajacent and facing
		kGrass,   // Transient Food
		kWater    // Swim or drink
	};

private:
	Feature feature_[10][10];

public:
	static constexpr auto latExtent{ std::extent<decltype(Preserve::feature_), GPS::Coordinate::kLat>::value };
	static constexpr auto lngExtent{ std::extent<decltype(Preserve::feature_), GPS::Coordinate::kLng>::value };

private:
	Preserve();

private:
	GPS herd_;

public:
	static Preserve& getInstance() // TODO: Dynamic creation in main?
	{
		static Preserve instance;
		return instance;
	}

public:
	static bool isObstacle(const Feature feature);

private:
	Feature getFeature(int lat, int lng) const;
	void setFeature(const int lat, const int lng, const Feature feature);
	void setFeature(const GPS gps, const Feature feature);

public:
	void killGrass(const GPS gps);
	Feature getFeature(const GPS gps) const;
	Feature getFeature(const Loxodonta& loxodonta) const;
	int getHerdDirection(const Loxodonta& loxodonta) const;
};
