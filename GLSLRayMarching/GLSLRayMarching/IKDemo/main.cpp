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
class IKDemo : public FrameWork
{
public:
	class IKDemoData
	{
	public:
		Matrix4 worldTransform;
		Matrix4 viewTransform;
		Matrix4 projTransform;
	};

	IKDemo()
		: FrameWork("IKDemo")
		, pos(100, 600, 100)
		, phi(0.7343)
		, theta(-0.5733)
	{
	}

	virtual ~IKDemo()
	{
	}

	virtual bool OnCreate() override
	{
		std::vector<Vector3> vertices;
		Shape<IKDemoData>::AddXYRect(vertices, Vector3(2, 1, 0), Vector3(-2, -1, 0));
		if (!shape1.Create(vertices))
			return false;

		if (!shape2.Create(vertices))
			return false;
		
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

		static float angle = 0.0f;
		angle++;
		shape1.SetRotateZ(angle);
		shape2.SetRotateZ(angle*10);

		shape1.Update(camera, Vector2(SCR_WIDTH, SCR_HEIGHT));
		shape2.Update(camera, Vector2(SCR_WIDTH, SCR_HEIGHT));

		return true;
	}

	void OnDestroy() override
	{
		shape1.Destroy();
		shape2.Destroy();
	}

	void UpdateCamera()
	{
		Vector2 mousedelta = GetMouseDelta();
		Platform::Debug("%f %f\n", mousedelta.X(), mousedelta.Y());

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
		camera.SetPerspectiveFov(90.0f, float(SCR_WIDTH) / SCR_HEIGHT, 1.0f, 5000.0f);
	}
private:
	Camera camera;
	Vector3 pos;
	float phi;
	float theta;

	Shape<IKDemoData> shape1;
	Shape<IKDemoData> shape2;
};

int main(int argc, char* argv[])
{
	IKDemo chapter;

	if (!chapter.Create(SCR_WIDTH, SCR_HEIGHT))
		return -1;

	chapter.Start();

	chapter.Destroy();

	return 0;
}