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

public: // To be implemented methods. 
	void tag(GPS& gps);
	void findHerd();
};
