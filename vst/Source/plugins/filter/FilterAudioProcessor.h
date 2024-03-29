#pragma once

#include <juce>
#include "BlankenhainAudioProcessor.h"
#include "FloatParameter.h"

class FilterAudioProcessor : public BlankenhainAudioProcessor
{
public:
	FilterAudioProcessor();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;

	void setResonance(FloatParameter resonance);
	void setFrequency(FloatParameter frequency);
	void setFilterType(int index);
	float getResonance();
	float getFrequency();
	int getFilterType();

protected:
	var getState() override;
	void setState(const var& state) override;

private:
	FloatParameter* resonance;
	FloatParameter* frequency;
	enum FilterType {
		High = 0,
		Low,
		Band,
		Notch,
	} filterType;

	Sample* internalBuffer = new Sample[FilterConstants::blockSize];
	Filter* filter = new Filter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterAudioProcessor)
};
