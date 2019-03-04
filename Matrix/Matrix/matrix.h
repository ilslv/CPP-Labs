#pragma once
#include  <iostream>

class Matrix
{
	int **array_, x_, y_;

	friend std::ostream& operator<<(std::ostream& out, Matrix const& m);
	void copy(Matrix const& m);
public:
	Matrix(int x, int y);
	Matrix(int x, int y, int value);
	Matrix(Matrix const& m);
	Matrix(Matrix&& m) noexcept;
	int* operator[](int i) const;
	void resize(int x, int y);
	void resize(int x, int y, int value);
	Matrix& operator=(Matrix const& m);
	Matrix operator+(Matrix const& m) const;
	Matrix operator*(Matrix const& m) const;
	int getDimensions(int d) const;
	int max() const;
	int min() const;
	int number_of_entries(int a) const;
	virtual ~Matrix();
};

std::ostream& operator<<(std::ostream& out, Matrix const & m);
