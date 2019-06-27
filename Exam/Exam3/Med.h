#pragma once
#include <ostream>
#include <istream>

struct Items
{
	char Name[35];
	char MyDate[8];
	char Section[10];
	int Count;
	bool operator==(Items&) const;
	friend std::istream& operator>>(std::istream&, Items&);
	friend std::ostream& operator<<(std::ostream&, Items&);
};

struct Price
{
	char Name[35];
	char MyDate[8];
	double pr;
	bool operator==(Price&) const;
	friend std::istream& operator>>(std::istream&, Price&);
	friend std::ostream& operator<<(std::ostream&, Price&);
};

struct Med
{
	char Name[35];
	char MyDate[8];
	char Section[10];
	double pr;
	int Count;
	bool operator==(Med&) const;
	friend std::istream& operator>>(std::istream&, Med&);
	friend std::ostream& operator<<(std::ostream&, Med&);
};

class ABS_Med
{
public:
	virtual void GetMeds(std::istream&) = 0;
	virtual void WriteMeds(std::ostream&) = 0;
	virtual void PutMeds(std::ostream&) = 0;
	virtual void SortUp() = 0;
};

class MB1 : public ABS_Med
{
	int count = 0;
	Items* M = nullptr;
	static int Cmp(const void*, const void*);
public:
	MB1() = default;
	virtual ~MB1();
	void GetMeds(std::istream&) override;
	void WriteMeds(std::ostream&) override;
	void PutMeds(std::ostream&) override;
	void SortUp() override;
	Items& operator[](int) const;
	void push_back(Items&);
	int GetSize() const;
};

class MB2 : public ABS_Med
{
	int count = 0;
	Price* M = nullptr;
	static int Cmp(const void*, const void*);
public:
	MB2() = default;
	virtual ~MB2();
	void GetMeds(std::istream&) override;
	void WriteMeds(std::ostream&) override;
	void PutMeds(std::ostream&) override;
	void SortUp() override;
	Price& operator[](int) const;
	void push_back(Price&);
	int GetSize() const;
};

class MB3 : public ABS_Med
{
	int count = 0;
	Med* M = nullptr;
	static int Cmp(const void*, const void*);
public:
	MB3() = default;
	virtual ~MB3();
	void GetMeds(std::istream&) override;
	void WriteMeds(std::ostream&) override;
	void PutMeds(std::ostream&) override;
	void SortUp() override;
	Med& operator[](int) const;
	void push_back(Med&);
	int GetSize() const;
};