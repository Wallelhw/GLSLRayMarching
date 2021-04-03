#ifndef _IK2DFABRIKJoint_h_
#define _IK2DFABRIKJoint_h_

#include "IK2DBase.h"

class IK2DFABRIKJoint : public IK2DBase
{
public:
	IK2DFABRIKJoint()
	{
	}

	virtual ~IK2DFABRIKJoint()
	{
	}

	IK2DFABRIKJoint& Begin(float iteration_, bool enableAngleLimit_)
	{
		iteration = iteration_;
		enableAngleLimit = enableAngleLimit_;
		jointPositions.clear();

		return *this;
	}

	IK2DFABRIKJoint& AddJoint(const Vector2& jointPosition_, const float& min_, const float& max_)
	{
		jointPositions.push_back(jointPosition_);
		mins.push_back(min_);
		maxs.push_back(max_);

		return *this;
	}

	bool End(const Vector2& effectorPosition_)
	{
		effectorPosition = effectorPosition_;

		return true;
	}

	virtual bool OnInitiate() override
	{
		return true;
	}

	virtual void OnUpdate(const Vector2& targetValue) override
	{
		std::vector<float> d(jointPositions.size() - 1);
		float allLinkLength = 0;
		for (int i = 0; i < jointPositions.size() - 1; i++)
		{
			d[i] = (jointPositions[i] - jointPositions[i + 1]).Length();

			allLinkLength += d[i];
		}

		float rootToTargetValueLength = (jointPositions[0] - targetValue).Length();
		if (allLinkLength >= rootToTargetValueLength)
		{
			Vector2 dir = targetValue - jointPositions[0];
			dir.Normalize();

			jointPositions[0] = jointPositions[0];
			for (int i = 1; i < jointPositions.size(); i++)
			{
				jointPositions[i] = jointPositions[i - 1] + dir * d[i];
			}
		}
		else
		{
			int i = 0;
			Vector2 moveTo;
			while (i++ <= iteration && (jointPositions.back() - targetValue).Length() < 0.1)
			{
				moveTo = targetValue;
				for (int i = jointPositions.size() - 1; i > 0; i--)
				{
					jointPositions[i] = moveTo;

					Vector2 dir = (jointPositions[i - 1] - jointPositions[i]);
					dir.Normalize();

					moveTo = jointPositions[i] + dir * d[i - 1];
				}

				moveTo = jointPositions[0];

				for (int i = 1; i < jointPositions.size() - 1; i++)
				{
					Vector2 dir = (jointPositions[i - 1] - jointPositions[i]);
					dir.Normalize();

					moveTo = jointPositions[i] + dir * d[i];

					jointPositions[i] = moveTo;
				}
			};
		}
	}

	virtual void OnTerminate() override
	{
	}

	const Vector2& GetRoot() const
	{
		return jointPositions[0];
	}

	const Vector2& GetEffectorPosition() const
	{
		return effectorPosition;
	}

	float GetLength(int i) const
	{
		if (i < jointPositions.size() - 1)
			return (jointPositions[i + 1] - jointPositions[i]).Length();
		else
			return (effectorPosition - jointPositions[i]).Length();
	}

	float GetJointCount() const
	{
		return jointPositions.size();
	}

	float GetJointAngle(int i) const
	{
		return 0.0;
	}
private:
	Vector2 effectorPosition;
	std::vector<Vector2> jointPositions;
	std::vector<float> mins;
	std::vector<float> maxs;

	int iteration;
	bool enableAngleLimit;
};


#endif