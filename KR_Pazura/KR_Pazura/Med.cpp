#include "Med.h"

int MB1::GetNumberOfElements() const
{
	return count;
}

int MB2::GetNumberOfElements() const
{
	return count;
}

int MB3::GetNumberOfElements() const
{
	return count;
}

Items* MB1::operator[](const int id) const
{
	return &M[id];
}

Price* MB2::operator[](const int id) const
{
	return &M[id];
}

Med* MB3::operator[](const int id) const
{
	return &M[id];
}


bool Items::operator==(Items& cmp) const
{
	return strcmp(Name, cmp.Name) == 0;
}

bool Med::operator==(Items& cmp) const
{
	return strcmp(Name, cmp.Name) == 0;
}

bool Price::operator==(Items& cmp) const
{
	return strcmp(Name, cmp.Name) == 0;
}

std::istream& operator>>(std::istream& in, Items& item)
{
	in >> item.Name;
	in >> item.myDate;
	in >> item.Section;
	in >> item.Count;
	return in;
}

std::istream& operator>>(std::istream& in, Price& item)
{
	in >> item.Name;
	in >> item.pr;
	return in;
}

std::istream& operator>>(std::istream& in, Med& item)
{
	in >> item.Name;
	in >> item.myDate;
	in >> item.Section;
	in >> item.pr;
	in >> item.Count;
	return in;
}

std::ostream& operator<<(std::ostream& out, Items& item)
{
	out << item.Name << "\t" << item.myDate << "\t" << item.Section << "\t" << item.Count << "\n";
	return out;
}

std::ostream& operator<<(std::ostream& out, Price& item)
{
	out << item.Name << "\t" << item.pr << "\n";
	return out;
}

std::ostream& operator<<(std::ostream& out, Med& item)
{
	out << item.Name << "\t" << item.myDate << "\t" << item.Section << "\t" << item.pr << "\t" << item.Count << "\n";
	return out;
}

void* Items::operator[](const int id)
{
	switch (id)
	{
	case 0:
		return static_cast<void*>(Name);
	case 1:
		return static_cast<void*>(myDate);
	case 2:
		return static_cast<void*>(Section);
	case 3:
		return static_cast<void*>(&Count);
	default:
		throw std::invalid_argument("No field with such index!");
	}
}

void* Price::operator[](const int id)
{
	switch (id)
	{
	case 0:
		return static_cast<void*>(Name);
	case 1:
		return static_cast<void*>(&pr);
	default:
		throw std::invalid_argument("No field with such index!");
	}
}

void* Med::operator[](const int id)
{
	switch (id)
	{
	case 0:
		return static_cast<void*>(Name);
	case 1:
		return static_cast<void*>(myDate);
	case 2:
		return static_cast<void*>(Section);
	case 3:
		return static_cast<void*>(&pr);
	case 4:
		return static_cast<void*>(&Count);
	default:
		throw std::invalid_argument("No field with such index!");
	}
}

void MB1::GetMeds(std::istream& in)
{
	in >> count;
	M = new Items[count];
	for (auto i = 0; i < count; ++i)
	{
		in >> M[i];
	}
}

void MB1::PutMeds(std::ostream& out)
{
	for (auto i = 0; i < count; ++i)
	{
		out << M[i];
	}
}

int MB1::CmpUp(const void* l, const void* r)
{
	return strcmp(((Items*)(l))->Name, ((Items*)(r))->Name);
}

int MB1::CmpDown(const void* l, const void* r)
{
	return strcmp(((Items*)(r))->Name, ((Items*)(l))->Name);
}


void MB1::SortUp()
{
	qsort(M, count, sizeof(M[0]), CmpUp);
}

void MB1::SortDown()
{
	qsort(M, count, sizeof(M[0]), CmpDown);
}

MB1::~MB1()
{
	delete[] M;
}


void MB2::GetMeds(std::istream& in)
{
	in >> count;
	M = new Price[count];
	for (auto i = 0; i < count; ++i)
	{
		in >> M[i];
	}
}

void MB2::PutMeds(std::ostream& out)
{
	for (auto i = 0; i < count; ++i)
	{
		out << M[i];
	}
}

int MB2::CmpUp(const void* l, const void* r)
{
	return strcmp(((Price*)(l))->Name, ((Price*)(r))->Name);
}

int MB2::CmpDown(const void* l, const void* r)
{
	return strcmp(((Price*)(r))->Name, ((Price*)(l))->Name);
}


void MB2::SortUp()
{
	qsort(M, count, sizeof(M[0]), CmpUp);
}

void MB2::SortDown()
{
	qsort(M, count, sizeof(M[0]), CmpDown);
}

MB2::~MB2()
{
	delete[] M;
}

void MB3::GetMeds(std::istream& in)
{
	in >> count;
	M = new Med[count];
	for (auto i = 0; i < count; ++i)
	{
		in >> M[i];
	}
}

void MB3::PutMeds(std::ostream& out)
{
	for (auto i = 0; i < count; ++i)
	{
		out << M[i];
	}
}

int MB3::CmpUp(const void* l, const void* r)
{
	return strcmp(((Med*)(l))->Name, ((Med*)(r))->Name);
}

int MB3::CmpDown(const void* l, const void* r)
{
	return strcmp(((Med*)(r))->Name, ((Med*)(l))->Name);
}


void MB3::SortUp()
{
	qsort(M, count, sizeof(M[0]), CmpUp);
}

void MB3::SortDown()
{
	qsort(M, count, sizeof(M[0]), CmpDown);
}

MB3::~MB3()
{
	delete[] M;
}

void MB3::push_back(const Med item)
{
	count++;
	const auto tmp = new Med[count];
	for (auto i = 0; i < count - 1; ++i)
	{
		tmp[i] = M[i];
	}
	M = tmp;
	M[count - 1] = item;
}

int MB3::CmpSearch(const void* key, const void* val)
{
	return strcmp((char*)(key), ((Med*)(val))->Name);
}

Med* MB3::find(char* key) const
{
	return static_cast<Med*>(bsearch(key, M, count, sizeof(M[0]), CmpSearch));
}

