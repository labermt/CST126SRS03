// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "elephant.h"
#include "preserve.h"

int main()
{
	// static Preserve preserve; // TODO: Dynamic preserve. 

	Elephant elephant(6'000, Direction::kNorth);
	GPS gps(8, 8);
	elephant.tag(gps);
	elephant.findHerd();
	const auto time{ elephant.getElapsedTime() };
	std::cout << "Herd found in: " << time << std::endl;

	return 0;
}