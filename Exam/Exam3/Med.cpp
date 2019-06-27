#include "Med.h"

bool Items::operator==(Items& cmp) const
{
	return strcmp(Name, cmp.Name) == 0;
}

bool Price::operator==(Price& cmp) const
{
	return strcmp(Name, cmp.Name) == 0;
}

bool Med::operator==(Med& cmp) const
{
	return strcmp(Name, cmp.Name) == 0;
}

std::ostream& operator<<(std::ostream& out, Items& item)
{
	out << item.Name << "\t" << item.MyDate << "\t" << item.Section << "\t" << item.Count << "\n";
	return out;
}

std::ostream& operator<<(std::ostream& out, Price& item)
{
	out << item.Name << "\t" << item.MyDate << "\t" << item.pr << "\n";
	return out;
}

std::ostream& operator<<(std::ostream& out, Med& item)
{
	out << item.Name << "\t" << item.MyDate << "\t" << item.Section << "\t" << item.pr << "\t" << item.Count << "\n";
	return out;
}

std::istream& operator>>(std::istream& in, Items& item)
{
	char name[35];
	in.getline(name, 35);
	strcpy_s(item.Name, 35, name);
	in >> item.MyDate;
	in >> item.Section;
	in >> item.Count;
	return in;
}

std::istream& operator>>(std::istream& in, Price& item)
{
	char name[35];
	in.getline(name, 35);
	strcpy_s(item.Name, 35, name);
	in >> item.MyDate;
	in >> item.pr;
	return in;
}

std::istream& operator>>(std::istream& in, Med& item)
{
	char name[35];
	in.getline(name, 35);
	strcpy_s(item.Name, 35, name);
	in >> item.MyDate;
	in >> item.Section;
	in >> item.pr;
	in >> item.Count;
	return in;
}

void MB1::GetMeds(std::istream& in)
{
	in >> count;
	M = new Items[count];
	for (auto i = 0; i < count; ++i)
	{
		in.read(reinterpret_cast<char*>(&M[i]), sizeof(Items));
	}
}

void MB1::WriteMeds(std::ostream& out)
{
	out << count;
	for (auto i = 0; i < count; ++i)
	{
		out.write(reinterpret_cast<char*>(&(M[i])), sizeof(Items));
	}
}

void MB1::PutMeds(std::ostream& out)
{
	for (auto i = 0; i < count; ++i)
	{
		out << M[i];
	}
}

int MB1::Cmp(const void* l, const void* r)
{
	return strcmp((char*)l, (char*)r);
}

void MB1::SortUp()
{
	qsort(M, count, sizeof(M[0]), Cmp);
}

Items& MB1::operator[](const int id) const
{
	return M[id];
}

void MB2::GetMeds(std::istream& in)
{
	in >> count;
	M = new Price[count];
	for (auto i = 0; i < count; ++i)
	{
		in.read(reinterpret_cast<char*>(&M[i]), sizeof(Price));
	}
}

void MB2::WriteMeds(std::ostream& out)
{
	out << count;
	for (auto i = 0; i < count; ++i)
	{
		out.write(reinterpret_cast<char*>(&(M[i])), sizeof(Price));
	}
}

void MB2::PutMeds(std::ostream& out)
{
	for (auto i = 0; i < count; ++i)
	{
		out << M[i];
	}
}

int MB2::Cmp(const void* l, const void* r)
{
	return strcmp((char*)l, (char*)r);
}

void MB2::SortUp()
{
	qsort(M, count, sizeof(M[0]), Cmp);
}

Price& MB2::operator[](const int id) const
{
	return M[id];
}

void MB3::GetMeds(std::istream& in)
{
	in >> count;
	M = new Med[count];
	for (auto i = 0; i < count; ++i)
	{
		in.read(reinterpret_cast<char*>(&M[i]), sizeof(Med));
	}
}

void MB3::WriteMeds(std::ostream& out)
{
	out << count;
	for (auto i = 0; i < count; ++i)
	{
		out.write(reinterpret_cast<char*>(&(M[i])), sizeof(Med));
	}
}

void MB3::PutMeds(std::ostream& out)
{
	for (auto i = 0; i < count; ++i)
	{
		out << M[i];
	}
}

int MB3::Cmp(const void* l, const void* r)
{
	return strcmp((char*)l, (char*)r);
}

void MB3::SortUp()
{
	qsort(M, count, sizeof(M[0]), Cmp);
}

Med& MB3::operator[](const int id) const
{
	return M[id];
}

MB1::~MB1()
{
	delete[] M;
}

MB2::~MB2()
{
	delete[] M;
}

MB3::~MB3()
{
	delete[] M;
}

void MB1::push_back(Items& item)
{
	auto tmp = new Items[count + 1];
	for (auto i = 0; i < count; ++i)
	{
		tmp[i] = M[i];
	}
	M = tmp;
	M[count] = item;
	count++;
}

void MB2::push_back(Price& item)
{
	auto tmp = new Price[count + 1];
	for (auto i = 0; i < count; ++i)
	{
		tmp[i] = M[i];
	}
	M = tmp;
	M[count] = item;
	count++;
}

void MB3::push_back(Med& item)
{
	auto tmp = new Med[count + 1];
	for (auto i = 0; i < count; ++i)
	{
		tmp[i] = M[i];
	}
	M = tmp;
	M[count] = item;
	count++;
}

int MB1::GetSize() const
{
	return count;
}

int MB2::GetSize() const
{
	return count;
}

int MB3::GetSize() const
{
	return count;
}


