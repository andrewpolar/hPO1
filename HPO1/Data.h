#pragma once
#include <iostream>

class Data
{
public:
	virtual std::unique_ptr<double[]> GetInput() = 0;
	virtual double GetTarget(std::unique_ptr<double[]>& x) = 0;
	virtual bool Populate() = 0;
	std::unique_ptr<std::unique_ptr<double[]>[]> inputs;
	std::unique_ptr<std::unique_ptr<double[]>[]> validationinputs;
	std::unique_ptr<double[]> target;
	std::unique_ptr<double[]> validationtarget;
	int nRecords = -1;
	int nValidationRecords = -1;
	int nFeatures = -1;
};

