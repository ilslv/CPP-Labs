#include "kr_template.h"
#include <iomanip>
#include <sstream>

bool kr_template::compare_name(const std::string& str) const
{
	return name == str;
}

kr_template::kr_template(std::string str)
{
	std::string subs[4];

	for (auto i = 0; i < 3; i++)
	{
		const auto pos = str.find(';');
		subs[i] = str.substr(0, pos);
		str.erase(0, pos + 1);
	}
	subs[3] = str;

	name = subs[0];
	field1 = std::stoi(subs[1]);
	field2 = std::stod(subs[2]);
 
	std::stringstream ss(subs[3]);
	ss >> std::get_time(&date, "%Y-%m-%d %H:%M");
}

bool kr_template::operator<(kr_template const& cmp) const
{
	return name < cmp.name;
}

std::ostream& operator<<(std::ostream& out, kr_template const& temp)
{
	out << temp.name << " " << temp.field1 << " " << temp.field2 << " " << std::put_time(&(temp.date), "%b %d %Y %H:%M") << "\r\n";
	return out;
}


