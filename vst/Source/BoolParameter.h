#pragma once

#include <juce>

class BoolParameter : public AudioProcessorParameter
{
public:
	BoolParameter(const String& name, bool defaultValue);

	float getValue() const override;
	void setValue(float newValue) override;
	float getDefaultValue() const override;
	String getName(int maximumStringLength) const override;
	String getLabel() const override;
	float getValueForText(const String &text) const override;
	int getNumSteps() const override;

	bool getBoolValue() const;
	void setBoolValue(bool newValue);

private:
	const bool defaultValue;
	bool value;
	const String name;
};
