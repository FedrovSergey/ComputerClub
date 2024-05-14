#include "myTime.h"

Time::Time() : hours(0), minutes(0) {}

Time::Time(int h, int m) : hours(h), minutes(m) {}

Time::Time(std::string XX) {
	hours = std::stoi(XX.substr(0, 2));
	minutes = std::stoi(XX.substr(3));
}
// for time addition
void Time::addTime(const Time& other) {
	hours += other.hours;
	minutes += other.minutes;
	if (minutes >= 60) {
		minutes -= 60;
		hours++;
	}
	if (hours > 23) {
		hours -= 24;
	}
}
// for time subtraction
void Time::subtractTime(const Time& other) {
	hours -= other.hours;
	minutes -= other.minutes;
	if (minutes < 0) {
		minutes += 60;
		hours--;
	}
	if (hours < 0) {
		hours += 24;
	}
}
//get price for the using time
int Time::countPrice(const int price) {
	return price * (hours + (minutes / 60) + 1);
}

Time Time::operator=(const Time& other) {
	hours = other.hours;
	minutes = other.minutes;
	return *this;
}
bool Time::operator<(const Time& other) {
	if (hours < other.hours)return true;
	else if (hours == other.hours && minutes < other.minutes)return true;
	return false;
}
bool Time::operator<=(const Time& other) {
	if (hours < other.hours)return true;
	else if (hours == other.hours && minutes <= other.minutes)return true;
	return false;
}

std::ostream& operator<<(std::ostream& os, Time& ts) {
	if (ts.hours < 10)
		os << "0" << ts.hours << ":";
	else
		os << ts.hours << ":";
	if (ts.minutes < 10)
		os << "0" << ts.minutes;
	else
		os << ts.minutes;
	return os;
}

bool isValidTime(std::string time) {
	int hours = std::stoi(time.substr(0, 2));
	int minutes = std::stoi(time.substr(3));
	if (hours > 23) return false;
	if (minutes > 59) return false;
	return true;
}