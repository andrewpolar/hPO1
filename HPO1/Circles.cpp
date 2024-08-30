#include "Circles.h"

//In this dataset the target is distance between centers of two circles
//given by random three points.
std::unique_ptr<double[]> Circles::GetInput() {
	auto input = std::make_unique<double[]>(nFeatures);
	for (int i = 0; i < nFeatures; ++i) {
		input[i] = static_cast<double>((rand() % 1000) / 1000.0) * 100.0;
	}
	return input;
}

void Circles::GetCenter(double x1, double y1, double x2, double y2, double x3, double y3, double& cx, double& cy, double& r) {
	const double epsilon = 0.0001;
	double midpoint1_x = (x1 + x2) / 2.0;
	double midpoint1_y = (y1 + y2) / 2.0;
	double midpoint2_x = (x2 + x3) / 2.0;
	double midpoint2_y = (y2 + y3) / 2.0;

	double slope12 = (y2 - y1) / (x2 - x1);
	double slope23 = (y3 - y2) / (x3 - x2);

	if (fabs(slope12) < epsilon) {
		slope12 = epsilon;
	}
	if (fabs(slope23) < epsilon) {
		slope23 = epsilon;
	}

	double slopePBS1 = -1.0 / slope12;
	double slopePBS2 = -1.0 / slope23;

	//now we solve the following system
	//y - midpoint1_y = slopePBS1 (x - midpoint1_x)
	//y - midpoint2_y = slopePBS2 (x - midpoint2_x) 
	cx = (midpoint2_y - midpoint1_y + slopePBS1 * midpoint1_x - slopePBS2 * midpoint2_x) / (slopePBS1 - slopePBS2);
	cy = slopePBS1 * (cx - midpoint1_x) + midpoint1_y;
	r = sqrt(fabs((cx - x1) * (cx - x1) + (cy - y1) * (cy - y1)));
}

double Circles::GetTarget(std::unique_ptr<double[]>& x) {
	double cx1, cy1, r1;
	double cx2, cy2, r2;
	GetCenter(x[0], x[1], x[2], x[3], x[4], x[5], cx1, cy1, r1);
	GetCenter(x[6], x[7], x[8], x[9], x[10], x[11], cx2, cy2, r2);
	return sqrt((cx1 - cx2) * (cx1 - cx2) + (cy1 - cy2) * (cy1 - cy2));
}

bool Circles::Populate() {
	nRecords = 10000;
	nFeatures = 12;
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

