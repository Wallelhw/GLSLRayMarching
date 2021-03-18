#include "FrameWork.h"
#include "Texture.h"
#include "RenderStates.h"
#include "ShaderProgram.h"
#include "Buffer.h"
#include "Primitives.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "GUI.h"
#include "AABB3.h"
#include "Shape.h"
#include "Camera.h"
#include "IK2DNJoint.h"
#include "IK2D2Joint.h"

#define SCR_WIDTH (800*2)
#define SCR_HEIGHT (400*2)

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

//////////////////////////////////////////////////////////////////////
class IKDemo2DCCD : public FrameWork
{
public:
	class IKDemo2DCCDData
	{
	public:
		Matrix4 worldTransform;
		Matrix4 viewTransform;
		Matrix4 projTransform;
		ColorRGBA color;
	};

	IKDemo2DCCD()
		: FrameWork("IKDemo2DCCD")
		, pos(100, 600, 100)
		, phi(0.7343)
		, theta(-0.5733)
	{
	}

	virtual ~IKDemo2DCCD()
	{
	}

#define JOINT_COUNT 6

	virtual bool OnCreate() override
	{
		Vector2 pos(1, 3);

		ikChain2D.Begin(10, true, true);
		for (int i = 0; i < JOINT_COUNT; i++)
		{
			ikChain2D.AddJoint(pos, i == 0 ? -90 : -45, i == 0 ? 90 : 45, 0.05);
			pos += Vector2(Math::IntervalRandom(-2, 2), Math::IntervalRandom(-2, 2));
		}
		ikChain2D.End(pos);
		if (!ikChain2D.Create())
			return false;

		shapes.resize(ikChain2D.GetJointCount());
		for (int i = 0; i < shapes.size(); i++)
		{
			bool success =
				shapes[i].Begin().
				XYRect(Vector3(ikChain2D.GetLength(i), 0.1, 0), Vector3(-ikChain2D.GetLength(i) / 2, 0, 0)).
				End();

			if (!success)
				return false;

			ColorRGBA color = ColorRGBA(Math::UnitRandom(), Math::UnitRandom(), Math::UnitRandom());
			shapes[i].GetUniformBlockData().color = color;

			if (i - 1 >= 0)
				shapes[i - 1].AddChild(&shapes[i]);
		}

		return true;
	}

	virtual bool OnUpdate() override
	{
		UpdateCamera();

		UpdateJoint();

		return true;
	}

	void UpdateCamera()
	{
		Vector2 mousedelta = GetMouseDelta();
		if (GetMouseRightButtonStatus())
		{
			phi += mousedelta.X() / (SCR_WIDTH / 2) * 180 * Math::Degree2Radian;
			//if (phi > 180 * Math::Degree2Radian)
				//phi = 180 * Math::Degree2Radian;
			//if (phi < -180 * Math::Degree2Radian)
				//phi = -180 * Math::Degree2Radian;

			theta += mousedelta.Y() / (SCR_HEIGHT / 2) * 180 * Math::Degree2Radian;
			if (theta > 89.9 * Math::Degree2Radian)
				theta = 89.9 * Math::Degree2Radian;
			if (theta < -89.9 * Math::Degree2Radian)
				theta = -89.9 * Math::Degree2Radian;
		}

		Vector3 dir = Vector3(Math::Cos(theta) * Math::Cos(phi), Math::Sin(theta), Math::Cos(theta) * Math::Sin(phi)); dir.Normalize();
		Vector3 xAxis = dir.Cross(Vector3::UnitY); xAxis.Normalize();
		//		Debug("%f %f\n", theta, phi);

		if (IsKeyPressed('W'))
		{
			pos += dir;
		}

		if (IsKeyPressed('S'))
		{
			pos -= dir;
		}

		if (IsKeyPressed('A'))
		{
			pos -= xAxis;
		}

		if (IsKeyPressed('D'))
		{
			pos += xAxis;
		}
		Vector3 obj = pos + dir;
		Matrix4 cameraTransform;
		//cameraTransform.SetLookAt(pos, obj, Vector3::UnitY);
		//cameraTransform.SetLookAt(Vector3(10, 10, -10), Vector3::Zero, Vector3::UnitY);
		cameraTransform.SetLookAt(Vector3(0, 0, -10), Vector3::Zero, Vector3::UnitY);
		camera.SetLocalTransform(cameraTransform);
		camera.SetOrthogonal(20.0f, (20.0f * SCR_HEIGHT) / SCR_WIDTH, 1.0f, 5000.0f);
	}

	void UpdateJoint()
	{
		Vector4 mouse = GetMouse();
		Vector2 targetPosition = Vector2
		(
			-(mouse.X() - SCR_WIDTH / 2) / (SCR_WIDTH / 2) * 10.0f,
			(mouse.Y() - SCR_HEIGHT / 2) / (SCR_HEIGHT / 2) * (10.0f * SCR_HEIGHT) / SCR_WIDTH
		);

		ikChain2D.Update(targetPosition);

		for (int i = 0; i < shapes.size(); i++)
		{
			if (i == 0)
				shapes[i].SetTranslateRotZXYScale(ikChain2D.GetRoot().X(), ikChain2D.GetRoot().Y(), 0, ikChain2D.GetJointAngle(i), 0, 0, 1.0);
			else
				shapes[i].SetTranslateRotZXYScale((i - 1 >= 0 ? ikChain2D.GetLength(i - 1) : 0), 0, 0, ikChain2D.GetJointAngle(i), 0, 0, 1.0);
		}

		ClearState clearState;
		clearState.clearColor = ColorRGBA(0.0f, 0.0f, 0.0f, 1.0f);
		clearState.clearDepth = 1.0f;
		clearState.clearStencil = 0;
		clearState.enableClearColor = true;
		clearState.enableClearDepth = true;
		clearState.enableClearStencil = true;
		clearState.Apply();
		for (int i = 0; i < shapes.size(); i++)
			shapes[i].Render(camera, Vector2(SCR_WIDTH, SCR_HEIGHT));
	}

	void OnDestroy() override
	{
		for (int i = 0; i < shapes.size(); i++)
			shapes[i].Destroy();
	}
private:
	Camera camera;
	Vector3 pos;
	float phi;
	float theta;

	IK2DCCDJoint ikChain2D;
	std::vector<Shape<IKDemo2DCCDData>> shapes;
};


//////////////////////////////////////////////////////////////////////
class IKDemo2DNB : public FrameWork
{
public:
	class IKDemo2DNBData
	{
	public:
		Matrix4 worldTransform;
		Matrix4 viewTransform;
		Matrix4 projTransform;
		ColorRGBA color;
	};

	IKDemo2DNB()
		: FrameWork("IKDemo2D2B")
		, pos(100, 600, 100)
		, phi(0.7343)
		, theta(-0.5733)
	{
	}

	virtual ~IKDemo2DNB()
	{
	}

#define JOINT_COUNT 6

	virtual bool OnCreate() override
	{
		Vector2 pos(1, 3);

		ikChain2D.Begin(1, 10);
		for (int i = 0; i < JOINT_COUNT; i++)
		{
			ikChain2D.AddJoint(pos, i == 0 ? -90 : -45, i == 0 ? 90 : 45);
			pos += Vector2(Math::IntervalRandom(-2, 2), Math::IntervalRandom(-2, 2));
		}
		ikChain2D.End(pos);
		if (!ikChain2D.Create())
			return false;

		shapes.resize(ikChain2D.GetJointCount());
		for (int i = 0; i < shapes.size(); i++)
		{
			bool success =
				shapes[i].Begin().
				XYRect(Vector3(ikChain2D.GetLength(i), 0.1, 0), Vector3(-ikChain2D.GetLength(i) / 2, 0, 0)).
				End();

			if (!success)
				return false;

			ColorRGBA color = ColorRGBA(Math::UnitRandom(), Math::UnitRandom(), Math::UnitRandom());
			shapes[i].GetUniformBlockData().color = color;

			if (i - 1 >= 0)
				shapes[i - 1].AddChild(&shapes[i]);
		}

		return true;
	}

	virtual bool OnUpdate() override
	{
		UpdateCamera();

		UpdateJoint();

		return true;
	}

	void UpdateCamera()
	{
		Vector2 mousedelta = GetMouseDelta();
		if (GetMouseRightButtonStatus())
		{
			phi += mousedelta.X() / (SCR_WIDTH / 2) * 180 * Math::Degree2Radian;
			//if (phi > 180 * Math::Degree2Radian)
				//phi = 180 * Math::Degree2Radian;
			//if (phi < -180 * Math::Degree2Radian)
				//phi = -180 * Math::Degree2Radian;

			theta += mousedelta.Y() / (SCR_HEIGHT / 2) * 180 * Math::Degree2Radian;
			if (theta > 89.9 * Math::Degree2Radian)
				theta = 89.9 * Math::Degree2Radian;
			if (theta < -89.9 * Math::Degree2Radian)
				theta = -89.9 * Math::Degree2Radian;
		}

		Vector3 dir = Vector3(Math::Cos(theta) * Math::Cos(phi), Math::Sin(theta), Math::Cos(theta) * Math::Sin(phi)); dir.Normalize();
		Vector3 xAxis = dir.Cross(Vector3::UnitY); xAxis.Normalize();
		//		Debug("%f %f\n", theta, phi);

		if (IsKeyPressed('W'))
		{
			pos += dir;
		}

		if (IsKeyPressed('S'))
		{
			pos -= dir;
		}

		if (IsKeyPressed('A'))
		{
			pos -= xAxis;
		}

		if (IsKeyPressed('D'))
		{
			pos += xAxis;
		}
		Vector3 obj = pos + dir;
		Matrix4 cameraTransform;
		//cameraTransform.SetLookAt(pos, obj, Vector3::UnitY);
		//cameraTransform.SetLookAt(Vector3(10, 10, -10), Vector3::Zero, Vector3::UnitY);
		cameraTransform.SetLookAt(Vector3(0, 0, -10), Vector3::Zero, Vector3::UnitY);
		camera.SetLocalTransform(cameraTransform);
		camera.SetOrthogonal(20.0f, (20.0f * SCR_HEIGHT) / SCR_WIDTH, 1.0f, 5000.0f);
	}

	void UpdateJoint()
	{
		Vector4 mouse = GetMouse();
		Vector2 targetPosition = Vector2
		(
			-(mouse.X() - SCR_WIDTH / 2) / (SCR_WIDTH / 2) * 10.0f,
			(mouse.Y() - SCR_HEIGHT / 2) / (SCR_HEIGHT / 2) * (10.0f * SCR_HEIGHT) / SCR_WIDTH
		);

		ikChain2D.Update(targetPosition);

		for (int i = 0; i < shapes.size(); i++)
		{
			if (i == 0)
				shapes[i].SetTranslateRotZXYScale(ikChain2D.GetRoot().X(), ikChain2D.GetRoot().Y(), 0, ikChain2D.GetJointAngle(i), 0, 0, 1.0);
			else
				shapes[i].SetTranslateRotZXYScale((i - 1 >= 0 ? ikChain2D.GetLength(i - 1) : 0), 0, 0, ikChain2D.GetJointAngle(i), 0, 0, 1.0);
		}

		ClearState clearState;
		clearState.clearColor = ColorRGBA(0.0f, 0.0f, 0.0f, 1.0f);
		clearState.clearDepth = 1.0f;
		clearState.clearStencil = 0;
		clearState.enableClearColor = true;
		clearState.enableClearDepth = true;
		clearState.enableClearStencil = true;
		clearState.Apply();
		for (int i = 0; i < shapes.size(); i++)
			shapes[i].Render(camera, Vector2(SCR_WIDTH, SCR_HEIGHT));
	}

	void OnDestroy() override
	{
		for (int i = 0; i < shapes.size(); i++)
			shapes[i].Destroy();
	}
private:
	Camera camera;
	Vector3 pos;
	float phi;
	float theta;

	IK2DNJoint ikChain2D;
	std::vector<Shape<IKDemo2DNBData>> shapes;
};

//////////////////////////////////////////////////////////////////////
class IKDemo2D2B : public FrameWork
{
public:
	class IKDemo2D2BData
	{
	public:
		Matrix4 worldTransform;
		Matrix4 viewTransform;
		Matrix4 projTransform;
		ColorRGBA color;
	};

	IKDemo2D2B()
		: FrameWork("IKDemo2D2B")
		, pos(100, 600, 100)
		, phi(0.7343)
		, theta(-0.5733)
	{
	}

	virtual ~IKDemo2D2B()
	{
	}

	virtual bool OnCreate() override
	{
		ikChain2D.SetPositions(Vector2(0, 0), Vector2(3, 3), Vector2(6, -2));
		if (!ikChain2D.Create())
			return false;

		shapes.resize(ikChain2D.GetJointCount());
		for (int i = 0; i < shapes.size(); i++)
		{
			bool success =
				shapes[i].Begin().
				XYRect(Vector3(ikChain2D.GetLength(i), 0.1, 0), Vector3(-ikChain2D.GetLength(i) / 2, 0, 0)).
				End();

			if (!success)
				return false;

			ColorRGBA color = ColorRGBA(Math::UnitRandom(), Math::UnitRandom(), Math::UnitRandom());
			shapes[i].GetUniformBlockData().color = color;

			if (i - 1 >= 0)
				shapes[i - 1].AddChild(&shapes[i]);
		}

		return true;
	}

	virtual bool OnUpdate() override
	{
		UpdateCamera();

		UpdateJoint();

		return true;
	}

	void UpdateCamera()
	{
		Vector2 mousedelta = GetMouseDelta();
		if (GetMouseRightButtonStatus())
		{
			phi += mousedelta.X() / (SCR_WIDTH / 2) * 180 * Math::Degree2Radian;
			//if (phi > 180 * Math::Degree2Radian)
				//phi = 180 * Math::Degree2Radian;
			//if (phi < -180 * Math::Degree2Radian)
				//phi = -180 * Math::Degree2Radian;

			theta += mousedelta.Y() / (SCR_HEIGHT / 2) * 180 * Math::Degree2Radian;
			if (theta > 89.9 * Math::Degree2Radian)
				theta = 89.9 * Math::Degree2Radian;
			if (theta < -89.9 * Math::Degree2Radian)
				theta = -89.9 * Math::Degree2Radian;
		}

		Vector3 dir = Vector3(Math::Cos(theta) * Math::Cos(phi), Math::Sin(theta), Math::Cos(theta) * Math::Sin(phi)); dir.Normalize();
		Vector3 xAxis = dir.Cross(Vector3::UnitY); xAxis.Normalize();
		//		Debug("%f %f\n", theta, phi);

		if (IsKeyPressed('W'))
		{
			pos += dir;
		}

		if (IsKeyPressed('S'))
		{
			pos -= dir;
		}

		if (IsKeyPressed('A'))
		{
			pos -= xAxis;
		}

		if (IsKeyPressed('D'))
		{
			pos += xAxis;
		}
		Vector3 obj = pos + dir;
		Matrix4 cameraTransform;
		//cameraTransform.SetLookAt(pos, obj, Vector3::UnitY);
		//cameraTransform.SetLookAt(Vector3(10, 10, -10), Vector3::Zero, Vector3::UnitY);
		cameraTransform.SetLookAt(Vector3(0, 0, -10), Vector3::Zero, Vector3::UnitY);
		camera.SetLocalTransform(cameraTransform);
		camera.SetOrthogonal(20.0f, (20.0f * SCR_HEIGHT) / SCR_WIDTH, 1.0f, 5000.0f);
	}

	void UpdateJoint()
	{
		Vector4 mouse = GetMouse();
		Vector2 targetPosition = Vector2
		(
			-(mouse.X() - SCR_WIDTH / 2) / (SCR_WIDTH / 2) * 10.0f,
			(mouse.Y() - SCR_HEIGHT / 2) / (SCR_HEIGHT / 2) * (10.0f * SCR_HEIGHT) / SCR_WIDTH
		);

		ikChain2D.Update(targetPosition);

		for (int i = 0; i < shapes.size(); i++)
		{
			if (i == 0)
				shapes[i].SetTranslateRotZXYScale(ikChain2D.GetRoot().X(), ikChain2D.GetRoot().Y(), 0, ikChain2D.GetJointAngle(i), 0, 0, 1.0);
			else
				shapes[i].SetTranslateRotZXYScale((i - 1 >= 0 ? ikChain2D.GetLength(i - 1) : 0), 0, 0, ikChain2D.GetJointAngle(i), 0, 0, 1.0);
		}

		ClearState clearState;
		clearState.clearColor = ColorRGBA(0.0f, 0.0f, 0.0f, 1.0f);
		clearState.clearDepth = 1.0f;
		clearState.clearStencil = 0;
		clearState.enableClearColor = true;
		clearState.enableClearDepth = true;
		clearState.enableClearStencil = true;
		clearState.Apply();
		for (int i = 0; i < shapes.size(); i++)
			shapes[i].Render(camera, Vector2(SCR_WIDTH, SCR_HEIGHT));
	}

	void OnDestroy() override
	{
		for (int i = 0; i < shapes.size(); i++)
			shapes[i].Destroy();
	}
private:
	Camera camera;
	Vector3 pos;
	float phi;
	float theta;

	IK2D2Joint ikChain2D;
	std::vector<Shape<IKDemo2D2BData>> shapes;
};

int ikDemo2D2B(int argc, char* argv[])
{
	IKDemo2D2B chapter;

	if (!chapter.Create(SCR_WIDTH, SCR_HEIGHT))
		return -1;

	chapter.Start();

	chapter.Destroy();

	return 0;
}

int ikDemo2DNB(int argc, char* argv[])
{
	IKDemo2DNB chapter;

	if (!chapter.Create(SCR_WIDTH, SCR_HEIGHT))
		return -1;

	chapter.Start();

	chapter.Destroy();

	return 0;
}

int ikDemo2DCCD(int argc, char* argv[])
{
	IKDemo2DCCD chapter;

	if (!chapter.Create(SCR_WIDTH, SCR_HEIGHT))
		return -1;

	chapter.Start();

	chapter.Destroy();

	return 0;
}


int main(int argc, char* argv[])
{
	//return ikDemo2D2B(argc, argv);
	//return ikDemo2DNB(argc, argv);
	return ikDemo2DCCD(argc, argv);
}