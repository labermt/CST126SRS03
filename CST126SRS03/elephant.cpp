#include "stdafx.h"
<<<<<<< HEAD
#include <iostream>
#include <cassert>
#include "gps.h"
#include "preserve.h"
=======
>>>>>>> pr/4
#include "elephant.h"

void Elephant::tag( GPS& gps)
{
	gps_ = &gps;
}

void Elephant::findHerd()
{
	auto feature = look();
	auto foundHerd{ feature == Preserve::Feature::kHerd };
	while (!foundHerd)
	{
		const auto direction = listen();

	}
}