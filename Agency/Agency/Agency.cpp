#include "Agency.h"
#include <iostream>
#include <string>

Agency::Agency()
{
	events_ = nullptr;
	n_ = 0;
}

Agency::~Agency()
{
	delete[] events_;
}

void Agency::read(const char* filename)
{
	std::ifstream file("concerts.csv");
	n_ = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n') + 1;
	file.close();
	file.open("concerts.csv");
	events_ = new event[n_];
	for (auto i = 0; i < n_; i++)
	{
		char line[300];
		file.getline(line, 300);
		auto s = strtok(line, ";");
		events_[i].name = new char[100];
		strcpy(events_[i].name, s);
		s = strtok(nullptr, ";");
		events_[i].tickets_amount = strtol(s, nullptr, 10);
		s = strtok(nullptr, ";");
		events_[i].tickets_left = strtol(s, nullptr, 10);
		s = strtok(nullptr, "-");
		events_[i].date.tm_year = strtol(s, nullptr, 10) - 1900;
		s = strtok(nullptr, "-");
		events_[i].date.tm_mon = strtol(s, nullptr, 10) - 1;
		s = strtok(nullptr, " ");
		events_[i].date.tm_mday = strtol(s, nullptr, 10);
		s = strtok(nullptr, ":");
		events_[i].date.tm_hour = strtol(s, nullptr, 10);
		s = strtok(nullptr, "\n");
		events_[i].date.tm_min = strtol(s, nullptr, 10);
	}
	file.close();
}

event Agency::operator>>(const event e)
{
	const auto res = new event[n_ + 1];
	for(auto i=0; i<n_; i++)
	{
		res[i] = events_[i];
	}
	res[n_] = e;
	n_++;
	events_ = res;
	return  e;
}

std::ostream& operator<<(std::ostream& out, Agency const & a)
{
	for(auto i=0; i<a.n_; i++)
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
	qsort(events_, n_, sizeof(event), [](const void* d1, const void* d2) -> int
	{
		return mktime((tm*)d1) > mktime((tm*)d2);
	});
}

void Agency::book_ticket(const int i) const
{
	events_[i].tickets_amount--;
}
