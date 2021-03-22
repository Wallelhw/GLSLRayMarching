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
        , selectedInstance(0)
        , renderer(nullptr)
        , keyPressed(false)
    {
    }

    ~PathTraceImp()
    {
    }

    PTScene* scene;
    int selectedInstance;
    TiledRenderer* renderer;
    bool keyPressed;
};

///////////////////////////////////////////////////////
PathTrace::PathTrace()
    : impl(nullptr)
{
}

PathTrace::~PathTrace()
{
}

std::string shadersDir = "../assets/shaders/";
std::string assetsDir = "../assets/";

bool PathTrace::Construct(const char* sceneName)
{
    ///////////////////////////////////////////////////////
    impl = new PathTraceImp();
    if (!impl)
        return false;

    ///////////////////////////////////////////////////////
    impl->scene = new PTScene();
    if (!impl->scene)
        return false;

    RenderOptions renderOptions;
    LoadSceneFromFile(sceneName, impl->scene, renderOptions);
    //loadCornellTestScene(scene, renderOptions);
    impl->selectedInstance = 0;
    impl->scene->renderOptions = renderOptions;

    ///////////////////////////////////////////////////////
    // InitRenderer()
    impl->renderer = new TiledRenderer(impl->scene, shadersDir);
    if (!impl->renderer)
        return false;

    impl->renderer->Init();
    return true;
}

void PathTrace::Render()
{
    impl->keyPressed = false;
    
    if(Platform::GetKey(Platform::KeyCode::Mouse0) || Platform::GetKey(Platform::KeyCode::Mouse1) || Platform::GetKey(Platform::KeyCode::Mouse2))
    {
        float mouseSensitivity = 0.1f;

        if (Platform::GetKey(Platform::KeyCode::Mouse0))
        {
            Vector2 mouseDelta = Vector2(Platform::GetMouseDX(), Platform::GetMouseDY());
            impl->scene->camera->OffsetOrientation(mouseSensitivity * mouseDelta.X(), mouseSensitivity * mouseDelta.Y());
        }
        else if (Platform::GetKey(Platform::KeyCode::Mouse1))
        {
            Vector2 mouseDelta = Vector2(Platform::GetMouseDX(), Platform::GetMouseDY());
            impl->scene->camera->SetRadius(mouseSensitivity * mouseDelta.Y());
        }
        else if (Platform::GetKey(Platform::KeyCode::Mouse2))
        {
            Vector2 mouseDelta = Vector2(Platform::GetMouseDX(), Platform::GetMouseDY());
            impl->scene->camera->Strafe(mouseSensitivity * mouseDelta.X(), mouseSensitivity * mouseDelta.Y());
        }
        impl->scene->camera->isMoving = true;
    }

    /*
    if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow) && ImGui::IsAnyMouseDown() && !ImGuizmo::IsOver())
    {
        if (ImGui::IsMouseDown(0))
        {
            ImVec2 mouseDelta = ImGui::GetMouseDragDelta(0, 0);
            scene->camera->OffsetOrientation(mouseDelta.x, mouseDelta.y);
            ImGui::ResetMouseDragDelta(0);
        }
        else if (ImGui::IsMouseDown(1))
        {
            ImVec2 mouseDelta = ImGui::GetMouseDragDelta(1, 0);
            scene->camera->SetRadius(mouseSensitivity * mouseDelta.y);
            ImGui::ResetMouseDragDelta(1);
        }
        else if (ImGui::IsMouseDown(2))
        {
            ImVec2 mouseDelta = ImGui::GetMouseDragDelta(2, 0);
            scene->camera->Strafe(mouseSensitivity * mouseDelta.x, mouseSensitivity * mouseDelta.y);
            ImGui::ResetMouseDragDelta(2);
        }
        scene->camera->isMoving = true;
    }
    */

    impl->renderer->Update(128);
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

    // Rendering
    //ImGui::Render();
    //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    //SDL_GL_SwapWindow(loopdata.mWindow);
}

void PathTrace::Destroy()
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