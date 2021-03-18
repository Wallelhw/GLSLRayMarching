#include "FrameWork2.h"
#include "Video.h"

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


/////////////////////////////////////////////////////////////////////
#include "DefaultScene.h"
#include "Mac1Scene.h"
#include "Mac2Scene.h"
#include "Mac3Scene.h"

Scene::Creator<DefaultScene> DefaultSceneCreator("Default");
Scene::Creator<Mac1Scene> MacScene1Creator("Mac1");
Scene::Creator<Mac2Scene> MacScene2Creator("Mac2");
Scene::Creator<Mac3Scene> MacScene3Creator("Mac3");

class MacShaderDemoApp : public FrameWork2
{
public:
	MacShaderDemoApp()
		: FrameWork2()
	{
	}

	virtual ~MacShaderDemoApp()
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
	MacShaderDemoApp macShaderDemoApp;

	if (!macShaderDemoApp.Create(800*2, 400 * 2, "MacShaderDemo", "Mac1"))
		return -1;

	macShaderDemoApp.Start();

	macShaderDemoApp.Destroy();

	return 0;
}