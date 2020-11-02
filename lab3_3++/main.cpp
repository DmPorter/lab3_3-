#pragma once
#include"tablef.h"
#include"menu.h"
#include"util.h"
#include <iostream>

double f(double x) {
	return x * x;
}

int main() {
	function table;
	dialog(table);
	return 0;
}