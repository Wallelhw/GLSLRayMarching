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

#define SCR_WIDTH (800*2)
#define SCR_HEIGHT (400*2)

class IKChain2DBase
{
public:
	IKChain2DBase()
	{
	}

	virtual ~IKChain2DBase()
	{
	}

	bool Create()
	{
		return OnCreate();
	}

	void Update(const Vector2& targetPosition)
	{
		OnUpdate(targetPosition);
	}

	void Destroy()
	{
		OnDestroy();
	}
protected:
	virtual bool OnCreate() = 0;
	virtual void OnUpdate(const Vector2& targetPosition) = 0;
	virtual void OnDestroy() = 0;
private:
};

class IKChain2D : public IKChain2DBase
{
public:
	IKChain2D()
	{
	}

	virtual ~IKChain2D()
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

	virtual bool OnCreate() override
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

		// Platform::Debug("%f %f %f %f %f %f\n", targetPosition.X(), targetPosition.Y(), jointAngle[0], jointAngle[1]);
	}

	virtual void OnDestroy() override
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

class IKChain2DGradientDescent : public IKChain2DBase
{
public:
	IKChain2DGradientDescent()
	{
	}

	virtual ~IKChain2DGradientDescent()
	{
	}

	virtual bool OnCreate() override
	{
		return true;
	}

	virtual void OnUpdate(const Vector2& targetPosition) override
	{
		GradientDecent(targetPosition, values);

		for (int i = 0; i < values.size(); i++)
		{
			Platform::Debug("%f, ", values[i]);
		}

		Platform::Debug("\n");
	}

	virtual void OnDestroy() override
	{
	}

	IKChain2DGradientDescent& Begin(float sampleDistance_, float learningRate_)
	{
		sampleDistance = sampleDistance_;
		learningRate = learningRate_;

		values.clear();

		return *this;
	}

	IKChain2DGradientDescent& PushValue(float value)
	{
		values.push_back(value);
		
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
	
	int GetValueCount() const
	{
		return values.size();
	}
protected:
	virtual float CostFunction(const Vector2& target, const std::vector<float>& values) = 0;

private:
	float GetPartialGradient(const Vector2& target, std::vector<float>& values, int i)
	{
		float tempvalue = values[i];

		float f_x = CostFunction(target, values);
		
		values[i] += sampleDistance;
		float f_x_plus_d = CostFunction(target, values);

		float gradient = (f_x_plus_d - f_x) / sampleDistance;

		values[i] = tempvalue;

		return gradient;
	}

	void GradientDecent(const Vector2& target, std::vector<float>& values)
	{
		for (int i = 0; i < values.size(); i++)
		{
			values[i] -= learningRate * GetPartialGradient(target, values, i);
		}
	}
private:
	float sampleDistance;
	float learningRate;
	std::vector<float> values;
};

class IKChain2DGradientDescent1 : public IKChain2DGradientDescent
{
public:
	IKChain2DGradientDescent1()
	{
	}

	virtual ~IKChain2DGradientDescent1()
	{
	}

	void SetPositions(const Vector2& joint0Position_, 
					  const Vector2& joint1Position_, const Vector2& effectorPosition_)
	{
		root = joint0Position_;
		length[0] = (joint1Position_ - joint0Position_).Length(); // c
		length[1] = (effectorPosition_ - joint1Position_).Length(); // a

		Vector2 targetToBase = effectorPosition_ - joint0Position_;
		float targetToBaseLength = targetToBase.Length();
		float cosAlpha = (Math::Sqr(targetToBaseLength) + Math::Sqr(length[0]) - Math::Sqr(length[1])) / (2.0 * targetToBaseLength * length[0]);
		float alpha = Math::ACos(cosAlpha) * Math::Radian2Degree;

		// Inner angle beta
		float cosBeta = (Math::Sqr(length[1]) + Math::Sqr(length[0]) - Math::Sqr(targetToBaseLength)) / (2.0 * length[1] * length[0]);
		float beta = Math::ACos(cosBeta) * Math::Radian2Degree;

		// Angle from Joint0 and Target
		float atan = Math::ATan2(targetToBase.Y(), targetToBase.X()) * Math::Radian2Degree;
		
		Begin(5, 5).
			PushValue(10).
			PushValue(10).
		End();
	}

	virtual float CostFunction(const Vector2& target, const std::vector<float>& values) override
	{
		Matrix4 m;

		Vector3 hand = Vector3(length[1], 0, 0);
		m.SetRotateZ(values[1]);
		hand = m * hand;

		hand += Vector3(length[0], 0.0, 0.0);
		m.SetRotateZ(values[0]);
		hand = m * hand;

		return (Vector2(hand.X(), hand.Y()) - target).Length();
	}

	const Vector2& GetRoot() const
	{
		return root;
	}

	float GetLength(int i) const
	{
		return length[i];
	}

	float GetJointAngle(int i) const
	{
		return GetValue(i);
	}
private:
	Vector2 root;
	float length[2];
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

#define USE_GRADIENT

	virtual bool OnCreate() override
	{
#ifndef USE_GRADIENT
		ikChain2D.SetPositions(Vector2(0, 0), Vector2(3, 3), Vector2(6, -2));
		if (!ikChain2D.Create())
			return false;
#else
		ikChain2D.SetPositions(Vector2(0, 0), Vector2(3, 3), Vector2(6, -2));
		if (!ikChain2D.Create())
			return false;
#endif
		ColorRGBA colors[] =
		{
			ColorRGBA(1.0, 0.0, 0.0, 1.0),
			ColorRGBA(0.0, 0.0, 1.0, 1.0),
		};

		for (int i = 0; i < 2; i++)
		{
			bool success = shape[i].
				Begin().
				AddXYRect(Vector3(ikChain2D.GetLength(i), 0.1, 0), Vector3(-ikChain2D.GetLength(i) / 2, 0, 0)).
				End();

			if (!success)
				return false;

			shape[i].GetUniformBlockData().color = colors[i];

			if (i - 1 >= 0)
				shape[i - 1].AddChild(&shape[i]);
		}

		return true;
	}

	virtual bool OnUpdate() override
	{
		UpdateCamera();
		UpdateJoint();

		ClearState clearState;
		clearState.clearColor = ColorRGBA(0.0f, 0.0f, 0.0f, 1.0f);
		clearState.clearDepth = 1.0f;
		clearState.clearStencil = 0;
		clearState.enableClearColor = true;
		clearState.enableClearDepth = true;
		clearState.enableClearStencil = true;
		clearState.Apply();
		for (int i = 0; i < 2; i++)
			shape[i].Render(camera, Vector2(SCR_WIDTH, SCR_HEIGHT));

		return true;
	}

	void OnDestroy() override
	{
		for (int i = 0; i < 2; i++)
			shape[i].Destroy();
	}

	void UpdateJoint()
	{
#ifndef USE_GRADIENT
		Vector4 mouse = GetMouse();
		Vector2 targetPosition = Vector2
		(
			-(mouse.X() - SCR_WIDTH / 2) / (SCR_WIDTH / 2) * 10.0f,
			(mouse.Y() - SCR_HEIGHT / 2) / (SCR_HEIGHT / 2) * (10.0f * SCR_HEIGHT) / SCR_WIDTH
		);

		ikChain2D.Update(targetPosition);

		for (int i = 0; i < 2; i++)
		{
			if (i == 0)
			{
				shape[i].SetTranslateRotZXYScale(ikChain2D.GetRoot().X(), ikChain2D.GetRoot().Y(), 0,
					ikChain2D.GetJointAngle(i), 0, 0, 1.0);
	}
			else
			{
				shape[i].SetTranslateRotZXYScale((i - 1 >= 0 ? ikChain2D.GetLength(i - 1) : 0), 0, 0, ikChain2D.GetJointAngle(i), 0, 0, 1.0);
			}
}
#else
		Vector4 mouse = GetMouse();
		Vector2 targetPosition = Vector2
		(
			-(mouse.X() - SCR_WIDTH / 2) / (SCR_WIDTH / 2) * 10.0f,
			(mouse.Y() - SCR_HEIGHT / 2) / (SCR_HEIGHT / 2) * (10.0f * SCR_HEIGHT) / SCR_WIDTH
		);

		ikChain2D.Update(targetPosition);

		for (int i = 0; i < 2; i++)
		{
			if (i == 0)
			{
				shape[i].SetTranslateRotZXYScale(ikChain2D.GetRoot().X(), ikChain2D.GetRoot().Y(), 0,
					                                 ikChain2D.GetJointAngle(i), 0, 0, 1.0);
			}
			else
			{
				shape[i].SetTranslateRotZXYScale((i - 1 >= 0 ? ikChain2D.GetLength(i - 1) : 0), 0, 0, ikChain2D.GetJointAngle(i), 0, 0, 1.0);
			}
		}
#endif
	}

	void UpdateCamera()
	{
		Vector2 mousedelta = GetMouseDelta();
		//Platform::Debug("%f %f\n", mousedelta.X(), mousedelta.Y());

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
		//		Platform::Debug("%f %f\n", theta, phi);

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
private:
	Camera camera;
	Vector3 pos;
	float phi;
	float theta;

#ifndef USE_GRADIENT
	IKChain2D ikChain2D;
#else
	IKChain2DGradientDescent1 ikChain2D;
#endif
	Shape<IKDemo2D2BData> shape[2];
};

int main(int argc, char* argv[])
{
	IKDemo2D2B chapter;

	if (!chapter.Create(SCR_WIDTH, SCR_HEIGHT))
		return -1;

	chapter.Start();

	chapter.Destroy();

	return 0;
}