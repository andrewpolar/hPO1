#include "Triangles.h"

//In this dataset targets are areas of triangles given by vertices.
std::unique_ptr<double[]> Triangles::GetInput() {
	auto input = std::make_unique<double[]>(nFeatures);
	for (int i = 0; i < nFeatures; ++i) {
		input[i] = static_cast<double>((rand() % 1000) / 1000.0) * 100.0;
	}
	return input;
}
double Triangles::GetTarget(std::unique_ptr<double[]>& x) {
	double x1 = x[0];
	double x2 = x[1];
	double x3 = x[2];
	double y1 = x[3];
	double y2 = x[4];
	double y3 = x[5];
	double X1 = fabs(x1 - x2);
	double Y1 = fabs(y1 - y2);
	double X2 = fabs(x1 - x3);
	double Y2 = fabs(y1 - y3);
	double X3 = fabs(x2 - x3);
	double Y3 = fabs(y2 - y3);
	double a = sqrt(X1 * X1 + Y1 * Y1);
	double b = sqrt(X2 * X2 + Y2 * Y2);
	double c = sqrt(X3 * X3 + Y3 * Y3);
	double s = (a + b + c) / 2.0;
	double rtn = sqrt(s * (s - a) * (s - b) * (s - c));
	return rtn;
}

bool Triangles::Populate() {
	nRecords = 10000;
	nFeatures = 6;
	target = std::make_unique<double[]>(nRecords);
	inputs = std::make_unique<std::unique_ptr<double[]>[]>(nRecords);

	int counter = 0;
	while (true) {
		inputs[counter] = GetInput();
		target[counter] = GetTarget(inputs[counter]);
		if (++counter >= nRecords) break;
	}

	nValidationRecords = 1000;
	validationtarget = std::make_unique<double[]>(nValidationRecords);
	validationinputs = std::make_unique<std::unique_ptr<double[]>[]>(nValidationRecords);

	counter = 0;
	while (true) {
		validationinputs[counter] = GetInput();
		validationtarget[counter] = GetTarget(validationinputs[counter]);
		if (++counter >= nValidationRecords) break;
	}
	return true;
}

