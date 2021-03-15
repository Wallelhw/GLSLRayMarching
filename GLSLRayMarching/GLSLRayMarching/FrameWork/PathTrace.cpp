#include "PathTrace.h"
#include "PTScene.h"
#include "Loader.h"
#include "TiledRenderer.h"

class PathTraceImp
{
public:
    PathTraceImp()
        : scene(nullptr)
        , selectedInstance(0)
    {
    }

    ~PathTraceImp()
    {
    }

    PTScene* scene;
    int selectedInstance;
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
    /*
    keyPressed = false;

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

    impl->scene->camera->isMoving = false;
    impl->renderer->Update(128);
    //lastTime = presentTime;

    glClearColor(0., 0., 0., 0.);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    //auto io = ImGui::GetIO();
    impl->renderer->Render();
    //const glm::ivec2 screenSize = renderer->GetScreenSize();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 800, 800);
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