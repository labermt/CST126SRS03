#pragma once

#include "gps.h"
#include "loxodonta.h"
#include "elephant.h"

class Elephant: public Loxodonta
{
public:
	Elephant(const unsigned weight, const Direction heading) : Loxodonta(weight, heading)
	{
	}

public: // Student to implement methods: 
	void tag(GPS& gps);
	void findHerd();

	//Student added 
	void moveTowardsHerd();
	void survive(Preserve::Feature& feature);
	bool check(Turn);
	void safeMove();
};
