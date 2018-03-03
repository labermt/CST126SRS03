#pragma once

#include "preserve.h"

class Loxodonta // Genus Elephant
{
public:
	enum class Turn { k0, kForward, kLeft, kRight };

private:
	GPS * gps_{};

private:
	static constexpr unsigned kMaxAwake{ 22 }; // hours/day
	static constexpr unsigned kMaxWater{ 200 }; // liters/day
	static constexpr unsigned kMaxFood{ 200 }; // kg/day

	unsigned elapsedTime_{}; // minutes
	unsigned awake_{}; // minutes
	unsigned water_{}; // [0, kMaxWater)

	unsigned weight_; // [minWeight_,maxWeight_)

	const unsigned minWeight_;
	const unsigned maxWeight_;

	Direction heading_;

protected:
	Loxodonta(const unsigned weight, const Direction heading);

private:
	static Preserve::Feature look(const Loxodonta& loxodonta);
	static Direction getDirection(const Turn turn);

	void incrementTime(const unsigned minutes);
	void decrementWater(const unsigned liters);
	void decrementWeight(const unsigned kg);

protected:
	void setGps(GPS& gps);

public:
	GPS * getGps_() const;
	unsigned getElapsedTime() const;

protected: // Student available methods.
	unsigned getAwake() const;
	unsigned getWater() const;
	unsigned getWeight() const;
	unsigned getMinWeight() const;
	unsigned getMaxWeight() const;

	bool isSleepy() const;
	bool isThirsty() const;
	bool isHungry() const;

	void faceHerd();

	Direction getHeading(const Turn turn) const;
	Direction getHeading() const;
	Preserve::Feature look() const;
	Preserve::Feature look(const Turn turn) const;
	int listen() const; // Elephants can feel vibrations with their feet. 

	void sleep();
	void drink();
	void eat();
	void turn(const Turn turn);
	void move();
};
