#pragma once
#include "Data.h"

class Circles : public Data
{
public:
	std::unique_ptr<double[]> GetInput();
	double GetTarget(std::unique_ptr<double[]>& x);
	bool Populate();
private:
	void GetCenter(double x1, double y1, double x2, double y2, double x3, double y3, double& cx, double& cy, double& r);
};

