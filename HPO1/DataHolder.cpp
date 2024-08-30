#include "DataHolder.h"

//////////// MF ////////////
double DataHolder::GetMFTarget(std::unique_ptr<double[]>& x) {
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

std::unique_ptr<double[]> DataHolder::GetMFInput() {
	auto input = std::make_unique<double[]>(nMFFeatures);
	for (int i = 0; i < nMFFeatures; ++i) {
		input[i] = static_cast<double>((rand() % 1000) / 1000.0);
	}
	return input;
}

bool DataHolder::PopulateMFData() {
	nMFRecords = 10000;
	nMFFeatures = 5;
	MFtarget = std::make_unique<double[]>(nMFRecords);
	MFinputs = std::make_unique<std::unique_ptr<double[]>[]>(nMFRecords);

	int counter = 0;
	while (true) {
		MFinputs[counter] = GetMFInput();
		MFtarget[counter] = GetMFTarget(MFinputs[counter]);
		if (++counter >= nMFRecords) break;
	}
	return true;
}

//////////// CR ////////////
std::unique_ptr<double[]> DataHolder::GetCRInput() {
	auto input = std::make_unique<double[]>(nCRFeatures);
	for (int i = 0; i < nCRFeatures; ++i) {
		input[i] = static_cast<double>((rand() % 1000) / 1000.0) * 100.0;
	}
	return input;
}

void DataHolder::GetCenter(double x1, double y1, double x2, double y2, double x3, double y3, double& cx, double& cy, double& r) {
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

double DataHolder::GetCRTarget(std::unique_ptr<double[]>& x) {
	double cx1, cy1, r1;
	double cx2, cy2, r2;
	GetCenter(x[0], x[1], x[2], x[3], x[4], x[5], cx1, cy1, r1);
	GetCenter(x[6], x[7], x[8], x[9], x[10], x[11], cx2, cy2, r2);
	return sqrt((cx1 - cx2) * (cx1 - cx2) + (cy1 - cy2) * (cy1 - cy2));
}

bool DataHolder::PopulateCRData() {
	nCRRecords = 10000;
	nCRFeatures = 12;
	CRtarget = std::make_unique<double[]>(nCRRecords);
	CRinputs = std::make_unique<std::unique_ptr<double[]>[]>(nCRRecords);

	int counter = 0;
	while (true) {
		CRinputs[counter] = GetCRInput();
		CRtarget[counter] = GetCRTarget(CRinputs[counter]);
		if (++counter >= nCRRecords) break;
	}
	return true;
}

//////////// TR ////////////
std::unique_ptr<double[]> DataHolder::GetTRInput() {
	auto input = std::make_unique<double[]>(nTRFeatures);
	for (int i = 0; i < nTRFeatures; ++i) {
		input[i] = static_cast<double>((rand() % 1000) / 1000.0) * 100.0;
	}
	return input;
}
double DataHolder::GetTRTarget(double x1, double x2, double x3, double y1, double y2, double y3) {
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

bool DataHolder::PopulateTRData() {
	nTRRecords = 10000;
	nTRFeatures = 6;
	TRtarget = std::make_unique<double[]>(nTRRecords);
	TRinputs = std::make_unique<std::unique_ptr<double[]>[]>(nTRRecords);

	int counter = 0;
	while (true) {
		TRinputs[counter] = GetTRInput();
		TRtarget[counter] = GetTRTarget(
			TRinputs[counter][0], TRinputs[counter][1], TRinputs[counter][2],
			TRinputs[counter][3], TRinputs[counter][4], TRinputs[counter][5]
		);
		if (++counter >= nTRRecords) break;
	}
	return true;
}
