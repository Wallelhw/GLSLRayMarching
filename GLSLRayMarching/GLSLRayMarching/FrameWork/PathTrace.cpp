#include "PathTrace.h"
#include "PTScene.h"
#include "Loader.h"
#include "TiledRenderer.h"
#include "Platform.h"
#include "Vector2.h"

class PathTraceImp
{
public:
    PathTraceImp()
        : scene(nullptr)
        , renderer(nullptr)
    {
    }

    ~PathTraceImp()
    {
    }

    PTScene* scene;
    TiledRenderer* renderer;
};

///////////////////////////////////////////////////////
PathTrace::PathTrace()
    : impl(nullptr)
{
}

PathTrace::~PathTrace()
{
}

bool PathTrace::Initiate(const char* sceneName)
{
    std::string assetsDir = "../assets/";
    std::string shadersDir = "../assets/shaders/";
    std::string scenePath = assetsDir + sceneName;

    ///////////////////////////////////////////////////////
    impl = new PathTraceImp();
    if (!impl)
        return false;

    ///////////////////////////////////////////////////////
    impl->scene = new PTScene();
    if (!impl->scene)
        return false;

    RenderOptions renderOptions;
    LoadSceneFromFile(scenePath.c_str(), impl->scene, renderOptions);
    impl->scene->renderOptions = renderOptions;

    ///////////////////////////////////////////////////////
    // InitRenderer()
    impl->renderer = new TiledRenderer(impl->scene, shadersDir);
    if (!impl->renderer)
        return false;

    impl->renderer->Init();
    return true;
}

bool PathTrace::Update()
{
    if (Platform::GetKey(Platform::KeyCode::Mouse0) || Platform::GetKey(Platform::KeyCode::Mouse1) || Platform::GetKey(Platform::KeyCode::Mouse2))
    {
        float multiplier = 1.0f;
        if (Platform::GetKey(Platform::KeyCode::LeftShift))
        {
            multiplier *= 10.0f;
        }

        if (Platform::GetKey(Platform::KeyCode::Mouse0))
        {
            Vector2 mouseDelta = Vector2(Platform::GetMouseDX(), Platform::GetMouseDY());
            impl->scene->camera->OffsetOrientation(mouseDelta.X() * 0.03 * multiplier, mouseDelta.Y() * 0.03 * multiplier);
        }
        else if (Platform::GetKey(Platform::KeyCode::Mouse1))
        {
            Vector2 mouseDelta = Vector2(Platform::GetMouseDX(), Platform::GetMouseDY());
            impl->scene->camera->SetRadius(mouseDelta.Y() * 0.001 * multiplier);
        }
        else if (Platform::GetKey(Platform::KeyCode::Mouse2))
        {
            Vector2 mouseDelta = Vector2(Platform::GetMouseDX(), Platform::GetMouseDY());
            impl->scene->camera->Strafe(mouseDelta.X() * 0.0001 * multiplier, mouseDelta.Y() * 0.0001 * multiplier);
        }
        impl->scene->camera->isMoving = true;
    }

    return true;
}

void PathTrace::Render()
{
    float dt = Platform::GetDeltaTime();
    impl->renderer->Update(dt);
    //lastTime = presentTime;

    glClearColor(0., 0., 0., 0.);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    //auto io = ImGui::GetIO();
    impl->renderer->Render();
    const iVec2 screenSize = impl->renderer->GetScreenSize();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, screenSize.x, screenSize.y);
    impl->renderer->Present();
}

void PathTrace::Terminate()
{
    if (impl)
    {
        if (impl->renderer)
        {
            delete impl->renderer;
            impl->renderer = nullptr;
        }

        if (impl)
        {
            delete impl->scene;
            impl->scene = nullptr;
        }

        delete impl;
        impl = nullptr;
    }
}