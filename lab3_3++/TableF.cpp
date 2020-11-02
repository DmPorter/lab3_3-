
#include "tablef.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <math.h>
#define SIZE 100
#define CENTER_X 100
#define CENTER_Y  85
#define SCALE     10

function::function(int i, double x[]) {
	this->i = 0;
	for (int l = 0; l < i; l++) {
		SetPoint(x[l], 0);
	}
}

function::function(int i, double x[], double(*f)(double)) {
	this->i = 0;
	for (int l = 0; l < i; l++) {
		SetPoint(*x, f(*x));
		*x++;
	}
}

function::function(Point P) {
	this->i = 0;
	SetPoint(P.x, P.y);
}

function::function(const function& ob) :i(ob.i), P(nullptr) {
	if (i > 0) {
		P = new Point[i];

		for (int l = 0; l < ob.i; l++) {
			this->P[l] = ob.P[l];
		}
	}
}

function& function::operator +=(const Point& A) {

	SetPoint(A.x, A.y);

	return *this;
}

function function::operator +(const Point& A) const {
	function T;
	try {
		T.P = new Point[this->i];
	}
	catch (std::bad_alloc & ba)
	{
		std::cout << "------ too many items in function: " << ba.what() << std::endl;
		delete[] P;
		return *this;
	}
	for (int l = 0; l < this->i; l++) {
		T.P[l] = this->P[l];
		T.i = this->i;
	}

	Point* old = T.P;
	int i = T.i;
	try {
		T.P = new Point[i + 1];
	}
	catch (std::bad_alloc & ba)
	{
		std::cout << "------ too many items in function: " << ba.what() << std::endl;
		delete[] P;
	}

	for (int d = 0; d < i; d++) {
		T.P[d] = old[d];
	}
	i--;
	if (T.P[findPos(A.x)].x == A.x)
		return T;
	else {
		while (i >= 0 && T.P[i].x > A.x) {
			T.P[i + 1] = T.P[i];
			i = i - 1;
		}
		T.P[i + 1].x = A.x;
		T.P[i + 1].y = A.y;
		T.i++;
	}

	return T;
}

function function::operator +(const function& F) const {
	function T;

	try {
		T.P = new Point[this->i];
	}
	catch (std::bad_alloc & ba)
	{
		std::cout << "------ too many items in function: " << ba.what() << std::endl;
		delete[] P;
		return *this;
	}
	for (int l = 0; l < this->i; l++) {
		T.P[l] = this->P[l];
		T.i = this->i;
	}

	for (int j = 0; j < F.i; j++) {
		T = T + F.P[j];
	}
	return T;
}

function& function::operator +=(const function& F) {
	for (int i = 0; i < F.i; i++)
	{
		SetPoint(F.P[i].x, F.P[i].y);
	}
	return *this;
}


int function::SetPoint(double x, double y) {
	Point* old = this->P;
	int i = this->i;
	try {
		P = new Point[i + 1];
	}
	catch (std::bad_alloc & ba)
	{
		std::cout << "------ too many items in function: " << ba.what() << std::endl;
		delete[] P;
		return 0;
	}

	for (int d = 0; d < i; d++) {
		this->P[d] = old[d];
	}
	i--;
	if (this->P[findPos(x)].x == x)
		return -2;
	else {
		while (i >= 0 && this->P[i].x > x) {
			P[i + 1] = P[i];
			i = i - 1;
		}
		this->P[i + 1].x = x;
		this->P[i + 1].y = y;
		this->i++;
	}
	return 1;
}

int function::findPos(double key) const {
	bool flag = false;
	int l = 0;
	int r = this->i;
	int mid;

	if (this->i == 0)
		return -1;
	while ((l <= r) && (flag != true)) {
		mid = (l + r) / 2; // считываем срединный индекс отрезка [l,r]
		if (this->P[mid].x == key) flag = true;
		if (this->P[mid].x > key)
			r = mid - 1; // проверяем, какую часть нужно отбросить
		else l = mid + 1;
	}
	return mid;
}

std::ostream& operator << (std::ostream& s, const function& st) {
	if (st.i == 0)
		s << "Table is free";
	for (int i = 0; i < st.i; i++)
		s << "(" << st.P[i].x << "; " << st.P[i].y << ")" << std::endl << std::endl;
	return s;
}


std::istream& operator >> (std::istream& s, function& st) {
	double x, y;

	s >> x >> y;
	if (!s.good())
		s.setstate(std::ios::failbit);
	else {
		Point P;
		P.x = x;
		P.y = y;
		st += P;
	}

	return s;
}

double function::GetMinMax(int(*f)(double, double))
{
	if (this->i == 0)
		return 0;
	double res = this->P[0].y;
	int i;
	for (i = 0; i < this->i; ++i)
		if (f(this->P[i].y, res) > 0)
			res = this->P[i].y;
	return res;
}

int function::GetType() {
	if (this->i == 0)
		return -3;
	else if (this->i == 1)
		return 0;
	else {
		int flagi = 0; int flagd = 0; int flagc = 0;
		for (int l = 1; l < this->i; l++) {
			if (this->P[l - 1].y < this->P[l].y)
				flagi = 1;
			else if (this->P[l - 1].y > this->P[l].y)
				flagd = 1;
			else flagc = 1;
			if (flagi == 1 && flagd == 1)
				return -2;
		}
		if (flagi == 1)
			return 1;
		else if (flagd == 1)
			return -1;
		else if (flagc == 1)
			return 0;
	}
}

double function::operator() (double x) const {
	int i = findPos(x);
	double a0, a1;
	if (this->i == 0)
		return 0;
	if (i == 0) i++;
	a1 = (this->P[i].y - this->P[i - 1].y) / (this->P[i].x - this->P[i - 1].x);
	a0 = this->P[i - 1].y - a1 * this->P[i - 1].x;

	return a0 + a1 * x;
}

double function::operator[](const double x) {
	int i = findPos(x);
	double x1 = abs(this->P[i].x - x);
	double x2 = abs(this->P[i - 1].x - x);
	if (min(x1, x2) == x2)
		i--;
	return this->P[i].y;
}

function& function::operator =(const function& ob) {
	if (this == &ob)
		return *this;
	this->i = ob.i;
	if (!i) {
		this->P = nullptr;
	}
	this->P = new Point[ob.i];
	for (int l = 0; l < ob.i; l++) {
		this->P[l] = ob.P[l];
	}

	return *this;
}

double function::Square() const{
	if (this->i == 0 || this->i == 1) return 0;
	double s = 0;
	for (int i = 0; i < this->i - 1; i++) {
		if (this->P[i].y > 0 && this->P[i + 1].y > 0)
			s += abs((this->P[i].y + this->P[i + 1].y) * (this->P[i].x - this->P[i + 1].x) / 2);
		if (this->P[i].y <= 0 || this->P[i + 1].y <= 0) {
			double a1 = (this->P[i + 1].y - this->P[i].y) / (this->P[i + 1].x - this->P[i].x);
			double a0 = this->P[i].y - a1 * this->P[i].x;
			double x0 = (-1) * a0 / a1;
			s += abs(this->P[i].y) * double(0.5) * abs(x0 - this->P[i].x);
			s += abs(this->P[i + 1].y) * double(0.5) * abs(this->P[i + 1].x - x0);
		}
	}
	return s;
}

void function::Equation() const {
	if(this->i == 0)
		std::cout << "Table is free";

	else if (this->i == 1)
		std::cout << "Point is one";
	else {
		for (int i = 0; i < this->i - 1; i++) {
			double a1 = (this->P[i + 1].y - this->P[i].y) / (this->P[i + 1].x - this->P[i].x);
			double a0 = this->P[i].y - a1 * this->P[i].x;
			if (a0 > 0 && a1 > 0)
				std::cout << "x = [" << this->P[i].x << ";" << this->P[i + 1].x << "]: " << "y = " << a1 << "x + " << a0 << std::endl << std::endl;
			else if (a0 < 0 && a1 > 0)
				std::cout << "x = [" << this->P[i].x << ";" << this->P[i + 1].x << "]: " << "y = " << a1 << "x - " << abs(a0) << std::endl << std::endl;
			else if (a0 < 0 && a1 < 0)
				std::cout << "x = [" << this->P[i].x << ";" << this->P[i + 1].x << "]: " << "y = -" << abs(a1) << "x - " << abs(a0) << std::endl << std::endl;
			else if (a0 > 0 && a1 < 0)
				std::cout << "x = [" << this->P[i].x << ";" << this->P[i + 1].x << "]: " << "y = -" << abs(a1) << "x + " << abs(a0) << std::endl << std::endl;
			else if (a0 == 0 && a1 < 0)
				std::cout << "x = [" << this->P[i].x << ";" << this->P[i + 1].x << "]: " << "y = -" << abs(a1) << std::endl << std::endl;
			else if (a0 == 0 && a1 > 0)
				std::cout << "x = [" << this->P[i].x << ";" << this->P[i + 1].x << "]: " << "y = " << abs(a1) << "x" << std::endl << std::endl;
			else if (a0 == 0 && a1 == 0)
				std::cout << "x = [" << this->P[i].x << ";" << this->P[i + 1].x << "]: " << "y = 0" << std::endl << std::endl;
			else if (a0 < 0 && a1 == 0)
				std::cout << "x = [" << this->P[i].x << ";" << this->P[i + 1].x << "]: " << "y = "<< "-" << abs(a0) << std::endl << std::endl;
			else if (a0 > 0 && a1 == 0)
				std::cout << "x = [" << this->P[i].x << ";" << this->P[i + 1].x << "]: " << "y = " << abs(a0) << std::endl << std::endl;
		}
	}


}