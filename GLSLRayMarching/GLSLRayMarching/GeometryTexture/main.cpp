#include "Texture.h"
#include "Scene.h"
#include "RenderStates.h"
#include "ShaderProgram.h"
#include "Buffer.h"
#include "Primitives.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "GUI.h"
#include "Camera.h"
#include "GameObject.h"
#include "FrameWork2.h"

#define GEOMETRY_TEXTURE_SIZE 1024
#define NORMAL_TEXTURE_SIZE 512

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

	if (!demoApp.Create(800 * 2, 400 * 2, "GeometryTexture", "GeometryTexture"))
		return -1;

	demoApp.Start();

	demoApp.Destroy();

	return 0;
}