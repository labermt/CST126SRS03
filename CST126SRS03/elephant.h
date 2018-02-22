#pragma once

#include "direction.h"
#include "gps.h"
#include "preserve.h"
#include "elephant.h"

class Elephant
{
public:
	enum class Turn { k0, kForward, kLeft, kRight};

private:
	GPS* gps_{};

private:
	static constexpr unsigned kMaxAwake{ 22 }; // hours/day
	static constexpr unsigned kMaxWater{ 200 }; // liters/day
	static constexpr unsigned kMaxFood{ 200 }; // kg/day

	unsigned clock_{}; // TODO: Make the clock run. 
	unsigned awake_{};
	unsigned water_{}; // [0, kMaxWater)

	unsigned weight_; // [minWeight_,maxWeight_)

	const unsigned minWeight_;
	const unsigned maxWeight_;

	Direction heading_;

public:
	Elephant(const unsigned weight, const Direction heading);

private:
	static Preserve::Feature look(const Elephant& elephant);

public:
	GPS * getGps_() const;

private: // Available methods.
	Direction getHeading(const Turn turn) const;
	Preserve::Feature look() const;
	Preserve::Feature look(const Turn turn) const;
	int listen() const; // Elephants can feel vibrations with their feet. 
	void sleep();
	void drink();
	void eat();
	void turn(const Turn turn);
	void move();

public: // To be implemented methods. 
	void tag(GPS& gps);
	void findHerd();
};
