#pragma once
#include <string>
#include <vector>
#include <ctime>

struct train
{
	std::string start_city, end_city;
	tm start_time{}, end_time{};
	int tickets_left{}, capacity{};
};

class railway
{
	std::vector<train> trains_;
public:
	railway() = default;
	train operator[](int) const;
	void sort_by_date();
	bool book_ticket(int id, int number_of_tickets = 1);
	std::vector<train>::iterator begin();
	std::vector<train>::iterator end();
	friend std::ostream& operator<<(std::ostream&, railway const &);
	friend std::istream& operator>>(std::istream&, railway &);
};