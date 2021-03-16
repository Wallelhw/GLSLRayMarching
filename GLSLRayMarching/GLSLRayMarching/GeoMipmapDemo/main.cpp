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
#include "GeoMipmap.h"
#include "GeoMorph.h"
#include "Camera.h"

#define SCR_WIDTH (800*2)
#define SCR_HEIGHT (400*2)

//////////////////////////////////////////////////////////////////////
class GeoMipmapDemo : public FrameWork
{
public:
	GeoMipmapDemo()
		: FrameWork("GeoMipmapDemo")
#ifdef CAPTURE_GRAPHICS
		, pos(0, 1, 0)
#else
		, pos(100, 600, 100)
#endif
		, phi(0.7343)
		, theta(-0.5733)
	{
	}

	virtual ~GeoMipmapDemo()
	{
	}

	virtual bool OnCreate() override
	{
		if (!geoMipmap.Create(PATCH_SIZE))
			return false;

		if (!geoMorph.Create())
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

		static int delay = 0;
		static bool wire = true;
		static bool solid = false;
		static float alpha = 0.0f;
		delay++;
		if (delay > 2)
		{
			if (IsKeyPressed('1'))
			{
				wire = !wire;
			}
			if (IsKeyPressed('2'))
			{
				solid = !solid;
			}
			delay = 0;
		}
		if (IsKeyPressed('3'))
		{
			alpha += 0.01f;
			alpha = (alpha > 1) ? 1 : alpha;
		}
		if (IsKeyPressed('4'))
		{
			alpha -= 0.01f;
			alpha = (alpha < 0) ? 0 : alpha;
		}

		// geoMipmap.Update(camera, Vector2(SCR_WIDTH, SCR_HEIGHT), solid, wire);

		geoMorph.Update(camera, Vector2(SCR_WIDTH, SCR_HEIGHT), solid, wire, alpha);
		
		return true;
	}

	void OnDestroy() override
	{
		geoMipmap.Destroy();
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

#ifdef CAPTURE_GRAPHICS
		if (IsKeyPressed('W'))
		{
			pos += Vector3(1.2, 0, 0);
		}

		if (IsKeyPressed('S'))
		{
			pos -= Vector3(1.2, 0, 0);
		}

		if (IsKeyPressed('A'))
		{
			pos += Vector3(0, 0, 1.2);
		}

		if (IsKeyPressed('D'))
		{
			pos -= Vector3(0, 0, 1.2);
		}
		Vector3 obj = pos + Vector3(0, -1, 0);

		Matrix4 cameraTransform;
		cameraTransform.SetLookAt(pos, obj, Vector3::UnitX);
		camera.SetLocalTransform(cameraTransform);
		camera.SetPerspectiveFov(90.0f, float(SCR_WIDTH) / SCR_HEIGHT, 1.0f, 5000.0f);
#else
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
		cameraTransform.SetLookAt(pos, obj, Vector3::UnitY);
		camera.SetLocalTransform(cameraTransform);
		camera.SetPerspectiveFov(90.0f, float(SCR_WIDTH) / SCR_HEIGHT, 1.0f, 5000.0f);
#endif
	}
private:
	Camera camera;
	Vector3 pos;
	float phi;
	float theta;

	GeoMipmap<Vector2> geoMipmap;
	GeoMorph geoMorph;
};

int main(int argc, char* argv[])
{
	GeoMipmapDemo chapter;

	if (!chapter.Create(SCR_WIDTH, SCR_HEIGHT))
		return -1;

	chapter.Start();

	chapter.Destroy();

	return 0;
}