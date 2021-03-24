#ifndef _IK2D2Joint_h_
#define _IK2D2Joint_h_

#include "IK2DBase.h"

class IK2D2Joint : public IK2DBase
{
public:
	IK2D2Joint()
	{
	}

	virtual ~IK2D2Joint()
	{
	}

	void SetPositions(const Vector2& joint0Position_, const Vector2& joint1Position_, const Vector2& effectorPosition_)
	{
		joint0Position = joint0Position_;
		joint1Position = joint1Position_;
		effectorPosition = effectorPosition_;

		length[0] = (joint1Position - joint0Position).Length(); // c
		length[1] = (effectorPosition - joint1Position).Length(); // a
	}

	virtual bool OnInitiate() override
	{
		return true;
	}

	virtual void OnUpdate(const Vector2& targetPosition) override
	{
		// Distance from Joint0 to Target
		Vector2 targetToBase = (targetPosition - joint0Position);
		float targetToBaseLength = targetToBase.Length();

		// Inner angle alpha
		float cosAlpha = (Math::Sqr(targetToBaseLength) + Math::Sqr(length[0]) - Math::Sqr(length[1])) / (2.0 * targetToBaseLength * length[0]);
		float alpha = Math::ACos(cosAlpha) * Math::Radian2Degree;

		// Inner angle beta
		float cosBeta = (Math::Sqr(length[1]) + Math::Sqr(length[0]) - Math::Sqr(targetToBaseLength)) / (2.0 * length[1] * length[0]);
		float beta = Math::ACos(cosBeta) * Math::Radian2Degree;

		// Angle from Joint0 and Target
		float atan = Math::ATan2(targetToBase.Y(), targetToBase.X()) * Math::Radian2Degree;

		if (length[1] + length[0] < targetToBaseLength)
		{
			jointAngle[0] = atan;
			jointAngle[1] = 0;
		}
		else
		{
			jointAngle[0] = atan - alpha; // Angle A
			jointAngle[1] = 180 - beta; // Angle B
		}

		// Debug("%f %f %f %f %f %f\n", targetPosition.X(), targetPosition.Y(), jointAngle[0], jointAngle[1]);
	}

	virtual void OnTerminate() override
	{
	}

	const Vector2& GetRoot() const
	{
		return joint0Position;
	}

	float GetLength(int i)
	{
		return length[i];
	}

	float GetJointAngle(int i)
	{
		return jointAngle[i];
	}

	float GetJointCount() const
	{
		return 2;
	}

	const Vector2& GetEffectorToBase()
	{
		return effectorPosition - joint0Position;
	}

	Vector2 joint0Position;
	Vector2 joint1Position;
	Vector2 effectorPosition;

	float length[2];
	float jointAngle[2];
};

#endif