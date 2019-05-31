#pragma once
#include <string>
#include <ctime>

class kr_template
{
public:
	std::string name;
	int field1;
	double field2;
	tm date{};
	kr_template(std::string str);
	bool operator<(kr_template const&) const;
	friend std::ostream& operator<<(std::ostream&, kr_template const &);
	bool compare_name(const std::string& str) const;
};
