#pragma once
#include "Data.h"

class Triangles : public Data
{
public:
	std::unique_ptr<double[]> GetInput();
	double GetTarget(std::unique_ptr<double[]>& x);
	bool Populate();
};

