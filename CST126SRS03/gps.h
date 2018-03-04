#pragma once

#include "direction.h"


class GPS
{
public:
	enum Coordinate { kLng = 0, kLat = 1 };

public:
	GPS(const int lat, const int lng);

private:
	int lat_{-1};
	int lng_{-1};

public:
	static int rangeTheta(const int theta);

public:
	static int theta(const int x, const int y);
	static Direction cardinal(const int direction);

private:
	void set(int lat, int lng);

public:
	int getlat() const;
	int getlng() const;

public:
	void move(const int direction, const unsigned distance);
};
