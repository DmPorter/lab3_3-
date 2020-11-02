#pragma once
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sstream>

struct Point {
	double x, y;
	Point(double x0 = 0, double y0 = 0) :x(x0), y(y0) {}
};

class function
{


public:
	friend std::ostream& operator << (std::ostream&, const function&);
	friend std::istream& operator >> (std::istream&, function&);
	inline function() :i(0), P(nullptr) {};//+
	function(int i, double x[]);//+
	function(int i, double x[], double(*f)(double));//+
	function(const Point P);//+
	function(const function& ob);//+
	function& operator +=(const function&);//+
	function& operator +=(const Point&);//+
	function operator +(const Point& A) const;//+
	function operator +(const function& A) const;//+
	int SetPoint(double x, double y);//+
	int findPos(double key) const;//+
	double GetMinMax(int(*f)(double, double));//+
	int GetType();//+
	double operator()(double) const;//+
	double operator[](const double x);//+
	~function() { delete[] P;}//+
	function& operator =(const function& ob);//+
	double Square() const;	
	void Equation() const;

private:

	Point* P;
	int i;

};

