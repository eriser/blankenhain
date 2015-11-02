#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED

#include "Sample.h"

struct Filter {
	Filter();

	void processHigh(Sample* data, size_t numberOfSamples, double frequency, double resonance);
	void processLow(Sample* data, size_t numberOfSamples, double frequency, double resonance);
	void processBand(Sample* data, size_t numberOfSamples, double frequency, double resonance);
	void processNotch(Sample* data, size_t numberOfSamples, double frequency, double resonance);
	void process(Sample* data, size_t numberOfSamples, double a1, double a2, double b0, double b1, double b2);

	Sample x_1;
	Sample y_1, y_2;
};

#endif // FILTER_H_INCLUDED