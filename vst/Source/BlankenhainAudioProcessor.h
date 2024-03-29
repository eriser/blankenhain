#pragma once

#include <juce>
#include <vector>

class BlankenhainAudioProcessor : public AudioProcessor
{
public:
	BlankenhainAudioProcessor();

	bool hasEditor() const override;

	const String getName() const override;

	const String getInputChannelName(int channelIndex) const override;
	const String getOutputChannelName(int channelIndex) const override;
	bool isInputChannelStereoPair(int index) const override;
	bool isOutputChannelStereoPair(int index) const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool silenceInProducesSilenceOut() const override;
	double getTailLengthSeconds() const override;

	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const String getProgramName(int index) override;
	void changeProgramName(int index, const String& newName) override;

	void getStateInformation(MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	double getLastKnownSampleRate(void);
	void setLastKnownSampleRate(double in);

	int getLastKnownBlockSize(void);
	void setLastKnownBlockSize(int in);

	std::vector<float> getMeterValues(void);

	//ONLY CALL THESE TWO FROM WITHIN PROCESSBLOCK
	void initializing(AudioSampleBuffer&);
	void finalizing(AudioSampleBuffer&);

	//METERING
	void meteringBuffer(AudioSampleBuffer&);
	void meteringSingle(float, float);

protected:
	virtual var getState() = 0;
	virtual void setState(const var& state) = 0;

	template<size_t BlockSize, typename ProcessFunction>
	void processBlockwise(AudioSampleBuffer& audioBuffer, Sample* processBuffer, ProcessFunction processFunction);

	double lastKnownSampleRate = 44100;
	int lastKnownBlockSize = 512;

	//Left Peak, Right Peak, Left RMS, Right RMS
	std::vector<float> meterValues = { 0.f,0.f,0.f,0.f };

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlankenhainAudioProcessor)
};


template<size_t BlockSize, typename ProcessFunction>
void BlankenhainAudioProcessor::processBlockwise(AudioSampleBuffer& audioBuffer, Sample* processBuffer, ProcessFunction processFunction) {
	size_t offset = 0;
	for (; offset + BlockSize <= audioBuffer.getNumSamples(); offset += BlockSize) {
		for (size_t i = 0; i < BlockSize; i++) {
			int sampleIndex = offset + i;
			processBuffer[i] = Sample(audioBuffer.getSample(0, sampleIndex), audioBuffer.getSample(1, sampleIndex));
		}
		processFunction(BlockSize, offset);
		for (size_t i = 0; i < BlockSize; i++) {
			int sampleIndex = offset + i;
			alignas(16) double sampleValues[2];
			processBuffer[i].store_aligned(sampleValues);
			audioBuffer.setSample(0, sampleIndex, sampleValues[0]);
			audioBuffer.setSample(1, sampleIndex, sampleValues[1]);
		}
	}
	const size_t remainingSamples = audioBuffer.getNumSamples() - offset;
	for (size_t i = 0; i < remainingSamples; i++) {
		int sampleIndex = offset + i;
		processBuffer[i] = Sample(audioBuffer.getSample(0, sampleIndex), audioBuffer.getSample(1, sampleIndex));
	}
	processFunction(remainingSamples, offset);
	for (size_t i = 0; i < remainingSamples; i++) {
		int sampleIndex = offset + i;
		alignas(16) double sampleValues[2];
		processBuffer[i].store_aligned(sampleValues);
		audioBuffer.setSample(0, sampleIndex, sampleValues[0]);
		audioBuffer.setSample(1, sampleIndex, sampleValues[1]);
	}
}
