#include "Agency.h"
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>

std::istream& operator>>(std::istream& in, Agency& a)
{
	std::string line;
	std::getline(in, line);

	size_t pos = 0;
	std::string subs[4];

	for (auto i = 0; i < 3; i++)
	{
		pos = line.find(';');
		subs[i] = line.substr(0, pos);
		line.erase(0, pos + 1);
	}
	subs[3] = line;

	event e;
	e.name = subs[0];
	e.tickets_amount = std::stoi(subs[1]);
	e.tickets_left = std::stoi(subs[2]);
	std::stringstream ss(subs[3]);
	ss >> std::get_time(&(e.date), "%Y-%m-%d %H:%M");

	a.events_.push_back(e);

	return in;
}

std::ostream& operator<<(std::ostream& out, Agency const& a)
{
	for(const auto& e : a.events_)
	{
		out << e.name << "\t" << e.tickets_amount << "\t" << e.tickets_left << "\t" << std::put_time(&(e.date), "%b %d %Y %H:%M") << "\n";
	}
	return out;
}

void Agency::book_ticket(const int i)
{
	if(events_[i].tickets_left != 0)
	{
		events_[i].tickets_left--;
	}
	else
	{
		throw std::length_error("No tickets left!");
	}
}

int Agency::get_tickets_amount(const int i) const
{
	return events_[i].tickets_amount;
}

event Agency::operator[](const int i) const
{
	return events_[i];
}

void Agency::sort_by_name()
{
	std::sort(events_.begin(), events_.end(), [](const event e1, const event e2) -> bool
	{
		return e1.name < e2.name;
	});
}

void Agency::sort_by_date()
{
	std::sort(events_.begin(), events_.end(), [](event e1, event e2) -> bool
	{
		return mktime(&(e1.date)) < mktime(&(e2.date));
	});
}

std::vector<event>::iterator Agency::begin()
{
	return events_.begin();
}

std::vector<event>::iterator Agency::end()
{
	return events_.end();
}

