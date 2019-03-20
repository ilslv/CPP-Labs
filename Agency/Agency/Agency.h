#pragma once
#include <ctime>
#include <string>
#include <vector>

struct event
{
	std::string name;
	int tickets_left, tickets_amount;
	tm date;
};

class Agency
{
	std::vector<event> events_;
public:
	Agency() = default;
	Agency(Agency const&) = delete;
	Agency(Agency&&) = delete;
	event operator[](int) const;
	int get_tickets_amount(int) const;
	void book_ticket(int);
	void sort_by_name();
	void sort_by_date();
	std::vector<event>::iterator begin();
	std::vector<event>::iterator end();
	friend std::ostream& operator<<(std::ostream&, Agency const &);
	friend std::istream& operator>>(std::istream&, Agency &);
};