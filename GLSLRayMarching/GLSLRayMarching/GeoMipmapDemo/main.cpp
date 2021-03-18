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
#include "FrameWork2.h"

/////////////////////////////////////////////////////////////////////
#include "Service.h"
#include "ResourceAccess.h"
#include "ResourceImport.h"
#include "Scene.h"
#include "Input.h"
#include "Physics3D.h"
#include "Physics2D.h"
#include "Video.h"
#include "Audio.h"
Service<Video> VideoService("Video");
Service<ResourceAccess> ResourceAccessService("ResourceAccess");
Service<ResourceImport> ResourceImportService("ResourceImport");
Service<Scene> SceneService("Scene");
Service<Input> InputService("Input");
Service<Physics3D> Physics3DService("Physics3D");
Service<Physics2D> Physics2DService("Physics2D");
Service<Audio> AudioService("Audio");

/////////////////////////////////////////////////////////////////////
#include "GameObject.h"

//////////////////////////////////////////////////////////////////////
class TerrainCameraComponent : public Video::CameraComponent
{
public:
	TerrainCameraComponent(GameObject& object)
		: Video::CameraComponent(object)
#ifdef CAPTURE_GRAPHICS
		, pos(0, 1, 0)
#else
		, pos(100, 600, 100)
#endif
		, phi(0.7343)
		, theta(-0.5733)
	{
	}

	virtual ~TerrainCameraComponent()
	{
	}

	virtual bool OnConstruct() override
	{
		return true;
	}

	virtual bool OnStart() override
	{
		return true;
	}

	virtual bool OnUpdate() override
	{
		Vector2 mousedelta = Vector2(Platform::GetMouseDX(), Platform::GetMouseDY());
		//Debug("%f %f\n", mousedelta.X(), mousedelta.Y());

		if (Platform::GetMouseButton(2))
		{
			phi += mousedelta.X() / (Platform::GetWidth() / 2) * 180 * Math::Degree2Radian;
			//if (phi > 180 * Math::Degree2Radian)
				//phi = 180 * Math::Degree2Radian;
			//if (phi < -180 * Math::Degree2Radian)
				//phi = -180 * Math::Degree2Radian;

			theta += mousedelta.Y() / (Platform::GetHeight() / 2) * 180 * Math::Degree2Radian;
			if (theta > 89.9 * Math::Degree2Radian)
				theta = 89.9 * Math::Degree2Radian;
			if (theta < -89.9 * Math::Degree2Radian)
				theta = -89.9 * Math::Degree2Radian;
		}

		Vector3 dir = Vector3(Math::Cos(theta) * Math::Cos(phi), Math::Sin(theta), Math::Cos(theta) * Math::Sin(phi)); dir.Normalize();
		Vector3 xAxis = dir.Cross(Vector3::UnitY); xAxis.Normalize();
		//		Debug("%f %f\n", theta, phi);

#ifdef CAPTURE_GRAPHICS
		/*
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
		*/
		Vector3 obj = pos + Vector3(0, -1, 0);

		Matrix4 cameraTransform;
		cameraTransform.SetLookAt(pos, obj, Vector3::UnitX);
		camera.SetLocalTransform(cameraTransform);
		camera.SetPerspectiveFov(90.0f, float(Platform::GetWidth()) / Platform::GetHeight(), 1.0f, 5000.0f);
#else
		/*
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
		*/

		Vector3 obj = pos + dir;

		Matrix4 cameraTransform;
		cameraTransform.SetLookAt(pos, obj, Vector3::UnitY);
		camera.SetLocalTransform(cameraTransform);
		camera.SetPerspectiveFov(90.0f, float(Platform::GetWidth()) / Platform::GetHeight(), 1.0f, 5000.0f);
#endif

		return true;
	}

	virtual bool OnPause() override
	{
		return true;
	}

	virtual void OnResume() override
	{
	}

	virtual void OnStop() override
	{
	}

	virtual void OnDestruct() override
	{
	}

	virtual void OnRender() override
	{
		ClearState clearState;
		clearState.clearColor = ColorRGBA(0.0f, 0.0f, 0.0f, 1.0f);
		clearState.clearDepth = 1.0f;
		clearState.clearStencil = 0;
		clearState.enableClearColor = true;
		clearState.enableClearDepth = true;
		clearState.enableClearStencil = true;
		clearState.Apply();
	}

	Camera camera;
	Vector3 pos;
	float phi;
	float theta;
};

class Terrain : public Video::Graphics3Component
{
public:
	class UniformData
	{
	public:
		Matrix4 worldTransform;
		Matrix4 viewTransform;
		Matrix4 projTransform;
		ColorRGBA color;
	};

	Terrain(GameObject& gameObject, TerrainCameraComponent& terrainCameraComponent_)
		: Video::Graphics3Component(gameObject)
		, terrainCameraComponent(terrainCameraComponent_)
	{
	}

	~Terrain()
	{
	}

	virtual bool OnConstruct() override
	{
		if (!geoMipmap.Create(PATCH_SIZE))
			return false;

		if (!geoMorph.Create())
			return false;

		return true;
	}

	virtual bool OnUpdate() override
	{
		return true;
	}

	virtual bool OnPause() override
	{
		return true;
	}

	virtual void OnResume() override
	{
	}

	virtual void OnStop() override
	{
	}

	virtual void OnDestruct() override
	{
		geoMipmap.Destroy();
		geoMorph.Destroy();
	}

	virtual void OnRender() override
	{
		Vector2 mouse = Vector2(Platform::GetMouseX(), Platform::GetMouseY());
		static int delay = 0;
		static bool wire = true;
		static bool solid = false;
		static float alpha = 0.0f;
		delay++;
		/*
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
		*/

		//geoMipmap.Render(terrainCameraComponent.camera, Vector2(Platform::GetWidth(), Platform::GetHeight()), solid, wire);
		geoMorph.Render(terrainCameraComponent.camera, Vector2(Platform::GetWidth(), Platform::GetHeight()), solid, wire, alpha);
	}

	TerrainCameraComponent& terrainCameraComponent;

	GeoMipmap<Vector2> geoMipmap;
	GeoMorph geoMorph;
};

class GeoMipmapScene : public Scene
{
public:
	GeoMipmapScene()
	: Scene()
	, terrainCameraComponent(gameObject)
	, terrain(gameObject, terrainCameraComponent)
	{
	}

	virtual ~GeoMipmapScene()
	{
	}
protected:
	virtual bool OnConstruct() override
	{
		gameObject.Add(&terrainCameraComponent);

		return true;
	}

	virtual bool OnStart() override
	{
		return true;
	}

	virtual bool OnUpdate() override
	{
		return true;
	}

	virtual bool OnPause() override
	{
		return true;
	}

	virtual void OnResume() override
	{
	}

	virtual void OnStop() override
	{
	}

	virtual void OnDestruct() override
	{
	}
private:
	GameObject gameObject;
	TerrainCameraComponent terrainCameraComponent;
	Terrain terrain;
};

Scene::Creator<GeoMipmapScene> GeoMipmapSceneCreator("GeoMipmap");

class DemoApp : public FrameWork2
{
public:
	DemoApp()
		: FrameWork2()
	{
	}

	virtual ~DemoApp()
	{
	}

	virtual bool OnCreate() override
	{
		return true;
	}

	virtual bool OnUpdate() override
	{
		return true;
	}

	void OnDestroy() override
	{
	}
private:
};

int main(int argc, char** argv)
{
	DemoApp demoApp;

	if (!demoApp.Create(800 * 2, 400 * 2, "GeoMipmap", "GeoMipmap"))
		return -1;

	demoApp.Start();

	demoApp.Destroy();

	return 0;
}