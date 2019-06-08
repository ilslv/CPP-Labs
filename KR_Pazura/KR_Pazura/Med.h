#pragma once
#include <iostream>

struct Items
{
	char Name[35];
	char myDate[8];
	char Section[10];
	int Count;
	bool operator==(Items&) const;
	void* operator[](int id);
};

struct Price
{
	char Name[35];
	double pr;
	bool operator==(Items&) const;
	void* operator[](int id);
};

struct Med
{
	char Name[35];
	char myDate[8];
	char Section[10];
	double pr;
	int Count;
	bool operator==(Items&) const;
	void* operator[](int id);
};

class ABS_Med
{
public:
	virtual void GetMeds(std::istream&) = 0;
	virtual void PutMeds(std::ostream&) = 0;
	virtual void SortUp() = 0;
	virtual void SortDown() = 0;
	virtual int GetNumberOfElements() const = 0;
};

class MB1 : public ABS_Med
{
	int count = 0;
	Items* M  = nullptr;
	static int CmpUp(const void*, const void*);
	static int CmpDown(const void*, const void*);
public:
	MB1() = default;
	MB1(const MB1&) = delete;
	MB1(MB1&&) = delete;
	void GetMeds(std::istream&) override;
	void PutMeds(std::ostream&) override;
	void SortUp() override;
	void SortDown() override;
	Items* operator[](int id) const;
	int GetNumberOfElements() const override;
	virtual  ~MB1();
};

class MB2 :public ABS_Med
{
	int count = 0;
	Price* M = nullptr;
	static int CmpUp(const void*, const void*);
	static int CmpDown(const void*, const void*);
public:
	MB2() = default;
	MB2(const MB2&) = delete;
	MB2(MB2&&) = delete;
	void GetMeds(std::istream&) override;
	void PutMeds(std::ostream&) override;
	void SortUp() override;
	void SortDown() override;
	Price* operator[](int id) const;
	int GetNumberOfElements() const;
	virtual  ~MB2();
};

class MB3 :public ABS_Med
{
	int count = 0;
	Med* M = nullptr;
	static int CmpUp(const void*, const void*);
	static int CmpDown(const void*, const void*);
	static int CmpSearch(const void*, const void*);
public:
	MB3() = default;
	MB3(const MB3&) = delete;
	MB3(MB3&&) = delete;
	void GetMeds(std::istream&) override;
	void PutMeds(std::ostream&) override;
	void SortUp() override;
	void SortDown() override;
	Med* operator[](int id) const; 
	int GetNumberOfElements() const;
	void push_back(Med);
	Med* find(char *key) const;
	virtual  ~MB3();
};