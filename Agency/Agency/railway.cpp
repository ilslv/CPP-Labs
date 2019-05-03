#include "railway.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

std::istream& operator>>(std::istream& in, railway& station)
{
	std::string line;
	std::getline(in, line);

	std::string fields[6];

	for (auto i = 0; i < 5; i++)
	{
		const auto pos = line.find(';');
		fields[i] = line.substr(0, pos);
		line.erase(0, pos + 1);
	}
	fields[5] = line;

	train t;
	t.start_city = fields[0];
	t.end_city = fields[2];
	t.tickets_left = std::stoi(fields[5]);
	t.capacity = std::stoi(fields[4]);
	std::stringstream start_date_stream(fields[1]);
	std::stringstream end_date_stream(fields[3]);
	start_date_stream >> std::get_time(&(t.start_time), "%Y-%m-%d %H:%M");
	end_date_stream >> std::get_time(&(t.end_time), "%Y-%m-%d %H:%M");

	station.trains_.push_back(t);

	return in;
}

std::ostream& operator<<(std::ostream& out, railway const& station)
{
	for (const auto& t : station.trains_)
	{
		out << t.start_city << "\t" <<
			std::put_time(&(t.start_time), "%b %d %Y %H:%M") << "\t" <<
			t.end_city << "\t" <<
			std::put_time(&(t.end_time), "%b %d %Y %H:%M") <<
			t.capacity << "\t" <<
			t.tickets_left << "\t" << "\n";
	}
	return out;
}

bool railway::book_ticket(const int id, const int number_of_tickets)
{
	if (trains_[id].tickets_left >= number_of_tickets)
	{
		trains_[id].tickets_left--;
		return true;
	}
	return false;
}

train railway::operator[](const int id) const
{
	return trains_[id];
}

void railway::sort_by_date()
{
	std::sort(trains_.begin(), trains_.end(), [](train t1, train t2)
	{
		return mktime(&(t1.start_time)) < mktime(&(t2.end_time));
	});
}

std::vector<train>::iterator railway::begin()
{
	return trains_.begin();
}

std::vector<train>::iterator railway::end()
{
	return trains_.end();
}



