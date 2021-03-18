#ifndef _IK2DCCDJoint_h_
#define _IK2DCCDJoint_h_

#include "IK2DBase.h"
class IK2DCCDJoint : public IK2DBase
{
public:
	IK2DCCDJoint()
	{
	}

	virtual ~IK2DCCDJoint()
	{
	}

	float Cross(const Vector2& b, const Vector2& a)
	{
		return Vector3(a.X(), a.Y(), 0).Cross(Vector3(b.X(), b.Y(), 0)).Z();
	}

	IK2DCCDJoint& Begin(float iteration_, bool enableDamp_, bool enableAngleLimit_)
	{
		iteration = iteration_;
		enableDamp = enableDamp_;
		enableAngleLimit = enableAngleLimit_;
		jointPositions.clear();

		return *this;
	}

	IK2DCCDJoint& AddJoint(const Vector2& jointPosition_, const float& min_, const float& max_, const float& damp_)
	{
		jointPositions.push_back(jointPosition_);
		angles.push_back(0);
		mins.push_back(min_);
		maxs.push_back(max_);
		damps.push_back(damp_);

		return *this;
	}

	bool End(const Vector2& effectorPosition_)
	{
		effectorPosition = effectorPosition_;

		return true;
	}

	virtual bool OnCreate() override
	{
		return true;
	}

	virtual void OnUpdate(const Vector2& targetValue) override
	{
#define MAX_IK_TRIES 100 // TIMES THROUGH THE CCD LOOP
#define IK_POS_THRESH 1.0f // THRESHOLD FOR SUCCESS

		Vector2 rootPos;
		Vector2 curEnd;
		Vector2 desiredEnd;
		Vector2 targetVector;
		Vector2 curVector;
		Vector2 crossResult;
		double cosAngle;
		double turnAngle;
		double turnDeg;
		int link;
		int i;

		///////////////////////////////////////////////////////////////////////////////
		// START AT THE LAST LINK IN THE CHAIN
		link = jointPositions.size() - 1;
		i = 0; // LOOP COUNTER SO I KNOW WHEN TO QUIT
		do
		{
			rootPos = jointPositions[link];
			curEnd = effectorPosition;
			desiredEnd = targetValue;

			if ((curEnd - desiredEnd).SquaredLength() > IK_POS_THRESH)
			{
				curVector = curEnd - rootPos;
				curVector.Normalize();

				targetVector = targetValue - rootPos;
				targetVector.Normalize();

				cosAngle = targetVector.Dot(curVector);
				if (cosAngle < 0.99999f)
				{
					float direction = Cross(targetVector, curVector);
					if (direction > 0.0f)
					{
						// DAMPING
						turnDeg = Math::ACos((float)cosAngle) * Math::Radian2Degree; // GET THE ANGLE
						if (enableDamp && turnDeg > damps[link])
							turnDeg = damps[link];

						// ACTUALLY TURN THE LINK
						angles[link] -= (float)turnDeg;

						// DOF RESTRICTIONS
						if (enableAngleLimit && angles[link] < mins[link])
							angles[link] = mins[link];
					}
					else if (direction < 0.0f)
					{
						turnDeg = Math::ACos((float)cosAngle) * Math::Radian2Degree; // GET THE ANGLE

						// DAMPING
						if (enableDamp && turnDeg > damps[link])
							turnDeg = damps[link];

						// ACTUALLY TURN THE LINK
						angles[link] += (float)turnDeg;

						// DOF RESTRICTIONS
						if (enableAngleLimit && angles[link] > maxs[link])
							angles[link] = maxs[link];
					}
				}
				if (--link < 0)
					link = jointPositions.size() - 1;
			}
			// QUIT IF I AM CLOSE ENOUGH OR BEEN RUNNING LONG ENOUGH
		} while (i++ < MAX_IK_TRIES && (curEnd - desiredEnd).SquaredLength() > IK_POS_THRESH);
	}

	virtual void OnDestroy() override
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
		return angles[i];
	}
private:
	Vector2 effectorPosition;
	std::vector<Vector2> jointPositions;
	std::vector<float> angles;
	std::vector<float> mins;
	std::vector<float> maxs;
	std::vector<float> damps;

	int iteration;
	bool enableDamp;
	bool enableAngleLimit;
};


#endif