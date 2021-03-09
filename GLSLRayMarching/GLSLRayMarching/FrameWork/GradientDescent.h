#ifndef _GradientDescent_h_
#define _GradientDescent_h_

#include "Platform.h"
#include <vector>

template<class T>
class GradientDescent
{
public:
	GradientDescent()
	{
	}

	virtual ~GradientDescent()
	{
	}

	void Update(const T& targetValue, std::function<float(const T&, const std::vector<float>&)> cb)
	{
		for (int i = 0; i < iteration; i++)
			GradientDecent(targetValue, values, cb);
	}

	GradientDescent& Begin(float sampleDistance_, float learningRate_, float iteration_ = 10)
	{
		sampleDistance = sampleDistance_;
		learningRate = learningRate_;
		iteration = iteration_;

		values.clear();

		return *this;
	}

	GradientDescent& PushValue(float value, float valueMinimun, float valueMaximun)
	{
		values.push_back(value);
		valueMinimuns.push_back(valueMinimun);
		valueMaximuns.push_back(valueMaximun);

		return *this;
	}

	bool End()
	{
		return true;
	}

	float GetValue(int i) const
	{
		return values[i];
	}

	float GetValueMinimun(int i) const
	{
		return valueMinimuns[i];
	}

	float GetValueMaximun(int i) const
	{
		return valueMaximuns[i];
	}

	int GetValueCount() const
	{
		return values.size();
	}
protected:
private:
	float GetPartialGradient(const T& targetValue, std::vector<float>& values, int i, std::function<float(const T&, const std::vector<float>&)> costFunc)
	{
		float tempvalue = values[i];

		float f_x = costFunc(targetValue, values);

		values[i] += sampleDistance;
		float f_x_plus_d = costFunc(targetValue, values);

		float gradient = (f_x_plus_d - f_x) / sampleDistance;

		values[i] = tempvalue;

		return gradient;
	}

	void GradientDecent(const T& targetValue, std::vector<float>& values, std::function<float(const T&, const std::vector<float>&)> costFunc)
	{
		for (int i = 0; i < values.size(); i++)
		{
			values[i] -= learningRate * GetPartialGradient(targetValue, values, i, costFunc);
			
			if (values[i] > valueMaximuns[i])
				values[i] = valueMaximuns[i];

			if (values[i] < valueMinimuns[i])
				values[i] = valueMinimuns[i];
		}
	}
private:
	float sampleDistance;
	float learningRate;
	int iteration;
	std::vector<float> values;
	std::vector<float> valueMinimuns;
	std::vector<float> valueMaximuns;
};

#endif