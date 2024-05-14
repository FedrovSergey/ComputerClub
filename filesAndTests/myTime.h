#pragma once
#include <iostream>
#include <string>

class Time {
public:
	Time();
	Time(int h, int m);
	// take string XX:XX
	Time(std::string XX);
	// for time addition
	void addTime(const Time& other);
	// for time subtraction
	void subtractTime(const Time& other);
	//get summary price for the using time
	int countPrice(const int price);

	Time operator=(const Time& other);
	bool operator<(const Time& other);
	bool operator<=(const Time& other);
	friend std::ostream& operator<<(std::ostream& os, Time& ts);

private:
	int hours;
	int minutes;
};

std::ostream& operator<<(std::ostream& os, Time& ts);

bool isValidTime(std::string time);