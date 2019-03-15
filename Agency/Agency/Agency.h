#pragma once
#include <ctime>
#include <fstream>

struct event
{
	char* name;
	int tickets_left, tickets_amount;
	tm date;
};

class Agency
{
	event* events_;
	int n_, cur_;
public:
	Agency();
	Agency(Agency const&) = delete;
	Agency(Agency&&) = delete;
	virtual ~Agency();
	event operator[](int) const;
	int get_tickets_amount(int) const;
	void book_ticket(int) const;
	void sort_by_name() const;
	void sort_by_date() const;
	friend std::ostream& operator<<(std::ostream&, Agency const &);
	friend std::istream& operator>>(std::istream&, Agency &);
};