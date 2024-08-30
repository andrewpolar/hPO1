#include "Formula.h"

//This dataset computes target by formula. It has no meaning, just an
//algebraic expression.
std::unique_ptr<double[]> Formula::GetInput() {
	auto input = std::make_unique<double[]>(nFeatures);
	for (int i = 0; i < nFeatures; ++i) {
		input[i] = static_cast<double>((rand() % 1000) / 1000.0);
	}
	return input;
}
double Formula::GetTarget(std::unique_ptr<double[]>& x) {
	double pi = 3.14159265359;
	double y = (1.0 / pi);
	y *= (2.0 + 2.0 * x[2]);
	y *= (1.0 / 3.0);
	y *= atan(20.0 * (x[0] - 0.5 + x[1] / 6.0) * exp(x[4])) + pi / 2.0;
	double z = (1.0 / pi);
	z *= (2.0 + 2.0 * x[3]);
	z *= (1.0 / 3.0);
	z *= atan(20.0 * (x[0] - 0.5 - x[1] / 6.0) * exp(x[4])) + pi / 2.0;
	return y + z;
}

bool Formula::Populate() {
	nRecords = 10000;
	nFeatures = 5;
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

