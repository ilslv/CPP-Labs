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


/*
Agency::Agency()
{
	events_ = nullptr;
	n_ = 0;
	cur_ = 0;
}

Agency::~Agency()
{
	for (auto i = 0; i < cur_; i++)
	{
		delete[] events_[i].name;
	}
	delete[] events_;
}

std::ostream& operator<<(std::ostream& out, Agency const & a)
{
	for (auto i = 0; i < a.cur_; i++)
	{
		char date[100];
		strftime(date, 100, "%b %d %Y %H:%M", &(a.events_[i].date));
		out << a.events_[i].name << "\t" << a.events_[i].tickets_amount << "\t" << a.events_[i].tickets_left << "\t" << date << std::endl;
	}
	return out;
}

event Agency::operator[](const int i) const
{
	return events_[i];
}

int Agency::get_tickets_amount(const int i) const
{
	return events_[i].tickets_amount;
}

void Agency::sort_by_name() const
{
	qsort(events_, n_, sizeof(event), [](const void *s1, const void *s2) -> int
	{
		return strcmp(static_cast<char const*>(s1), static_cast<char const*>(s2));
	});
}

void Agency::sort_by_date() const
{
	qsort(events_, n_, sizeof(events_[0]), [](const void* d1, const void* d2) -> int
	{
		auto date_1 = *static_cast<const struct tm *>(d1);
		auto date_2 = *static_cast<const struct tm *>(d2);
		const auto d = difftime(mktime(&date_1), mktime(&date_2));
		return (d < 0) - (d > 0);
	});
}

void Agency::book_ticket(const int i) const
{
	if (events_[i].tickets_amount == 0)
	{
		throw std::length_error("No tickets left!");
	}
	events_[i].tickets_amount--;
}

std::istream& operator>>(std::istream& in, Agency &a)
{
	if (a.cur_ == a.n_)
	{
		const auto tmp = new event[(a.n_ * 2) + 1];
		for (auto i = 0; i < a.n_; i++)
		{
			tmp[i] = a.events_[i];
		}
		a.events_ = tmp;
		a.n_ = (a.n_ * 2) + 1;
	}
	char line[300];
	in.getline(line, 300);
	char* context = nullptr;
	a.events_[a.cur_].name = new char[100];
	strcpy_s(a.events_[a.cur_].name, 100, strtok_s(line, ";", &context));
	a.events_[a.cur_].tickets_amount = strtol(strtok_s(nullptr, ";", &context), nullptr, 10);
	a.events_[a.cur_].tickets_left = strtol(strtok_s(nullptr, ";", &context), nullptr, 10);
	std::stringstream ss(strtok_s(nullptr, "\n", &context));
	ss >> std::get_time(&(a.events_[a.cur_].date), "%Y-%m-%d %H:%M");
	a.cur_++;
	return in;
}
*/