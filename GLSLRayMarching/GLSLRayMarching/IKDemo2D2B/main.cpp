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


#define Joint0Pos Vector2(0, 0)
#define Joint1Pos Vector2(3, 3)
#define HandPos Vector2(6, -2)

	class IKChain2D
	{
	public:
		IKChain2D()
		{
		}

		~IKChain2D()
		{
		}

		bool Create()
		{
			length[0] = (Joint0Pos - Joint1Pos).Length(); // c
			length[1] = (Joint1Pos - HandPos).Length(); // a

			return true;
		}

		void Update(const Vector2& targetPosition)
		{
			// Distance from Joint0 to Target
			targetToBase = (targetPosition - Joint0Pos);
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

		void Destroy()
		{
		}

		float GetLength(int i)
		{
			return length[i];
		}

		float GetJointAngle(int i)
		{
			return jointAngle[i];
		}

		const Vector2& GetTargetToBase()
		{
			return targetToBase;
		}

		Vector2 targetToBase;
		float length[2];
		float jointAngle[2];
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
		if (!ikChain2D.Create())
			return false;

		ColorRGBA colors[] =
		{
			ColorRGBA(1.0, 0.0, 0.0, 1.0),
			ColorRGBA(0.0, 0.0, 1.0, 1.0),
		};

		for(int i=0; i<2; i++)
		{
			std::vector<Vector3> vertices;
			Shape<IKDemo2D2BData>::AddXYRect(vertices, Vector3(ikChain2D.GetLength(i), 0.1, 0), Vector3(-ikChain2D.GetLength(i) / 2, 0, 0));
			if (!shape[i].Create(vertices))
				return false;

			shape[i].GetUniformBlockData().color = colors[i];

			if(i-1>=0)
				shape[i-1].AddChild(&shape[i]);
		}
		
		return true;
	}

	virtual bool OnUpdate() override
	{
		ClearState clearState;
		clearState.clearColor = ColorRGBA(0.0f, 0.0f, 0.0f, 1.0f);
		clearState.clearDepth = 1.0f;
		clearState.clearStencil = 0;
		clearState.enableClearColor = true;
		clearState.enableClearDepth = true;
		clearState.enableClearStencil = true;
		clearState.Apply();

		UpdateCamera();
		UpdateJoint();

		for(int i=0; i<2; i++)
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
		Vector4 mouse = GetMouse();
		Vector2 targetPosition = Vector2
		(
			-(mouse.X() - SCR_WIDTH / 2) / (SCR_WIDTH / 2) * 10.0f,
			 (mouse.Y() - SCR_HEIGHT / 2) / (SCR_HEIGHT / 2) * (10.0f * SCR_HEIGHT) / SCR_WIDTH
		);

		ikChain2D.Update(targetPosition);

		for (int i = 0; i < 2; i++)
		{
			shape[i].SetTranslateRotZXYScale((i-1>=0? ikChain2D.GetLength(i-1):0), 0, 0, ikChain2D.GetJointAngle(i), 0, 0, 1.0);
		}
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
			pos -= dir ;
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
		cameraTransform.SetLookAt(Vector3(0, 0, -10), Vector3::Zero, Vector3::UnitY);
		camera.SetLocalTransform(cameraTransform);
		camera.SetOrthogonal(20.0f, (20.0f * SCR_HEIGHT) / SCR_WIDTH, 1.0f, 5000.0f);
	}
private:
	Camera camera;
	Vector3 pos;
	float phi;
	float theta;

	IKChain2D ikChain2D;
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