//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Platform.h"
#include "Video.h" 

#include "ShaderProgram.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "Primitives.h"
#include "RenderStates.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "GUI.h"

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

#define SCR_WIDTH (800)
#define SCR_HEIGHT (800)

/////////////////////////////////////////////////////////////////////
Video::Graphics3Component::Graphics3Component(GameObject& gameObject_)
	: Component(gameObject_)
{
	Video::Manager::GetInstance().Add(this);
}

Video::Graphics3Component::~Graphics3Component()
{
	Video::Manager::GetInstance().Remove(this);
}

void Video::Graphics3Component::Render()
{
	OnRender();
}

bool Video::Graphics3Component::OnConstruct()
{
	return true;
}

bool Video::Graphics3Component::OnStart()
{
	return true;
}

bool Video::Graphics3Component::OnUpdate()
{
	return true;
}

bool Video::Graphics3Component::OnPause()
{
	return true;
}

void Video::Graphics3Component::OnResume()
{
}

void Video::Graphics3Component::OnStop()
{
}

void Video::Graphics3Component::OnDestruct()
{
}

/////////////////////////////////////////////////////////////////////
Video::RendererComponent::RendererComponent(GameObject& gameObject_)
	: Component(gameObject_)
{
	Video::Manager::GetInstance().Add(this);
}

Video::RendererComponent::~RendererComponent()
{
	Video::Manager::GetInstance().Remove(this);
}

void Video::RendererComponent::Render()
{
	OnRender();
}

bool Video::RendererComponent::OnConstruct()
{
	return true;
}

bool Video::RendererComponent::OnStart()
{
	return true;
}

bool Video::RendererComponent::OnUpdate()
{
	return true;
}

bool Video::RendererComponent::OnPause()
{
	return true;
}

void Video::RendererComponent::OnResume()
{
}

void Video::RendererComponent::OnStop()
{
}

void Video::RendererComponent::OnDestruct()
{
}

/////////////////////////////////////////////////////////////////////
Video::CameraComponent::CameraComponent(GameObject& gameObject_)
	: Component(gameObject_)
{
	Video::Manager::GetInstance().Add(this);
}

Video::CameraComponent::~CameraComponent()
{
	Video::Manager::GetInstance().Remove(this);
}

void Video::CameraComponent::Render()
{
	OnRender();
}

bool Video::CameraComponent::OnConstruct()
{
	return true;
}

bool Video::CameraComponent::OnStart()
{
	return true;
}

bool Video::CameraComponent::OnUpdate()
{
	return true;
}

bool Video::CameraComponent::OnPause()
{
	return true;
}

void Video::CameraComponent::OnResume()
{
}

void Video::CameraComponent::OnStop()
{
}

void Video::CameraComponent::OnDestruct()
{
}

/////////////////////////////////////////////////////////////////////
class GLData
{
public:
	GLData()
		: window(nullptr)
		, width(0)
		, height(0)
		, theta(0.0)
		, phi(0.0)
		, time(0.0)
		, deltaTime(0.0)
		, frameCounter(-1)
		, mouse(0.0, 0.0, 0.0, 0.0)
		, mouseDelta(0.0, 0.0)
		, mouseLButtonStatus(0)
		, mouseMButtonStatus(0)
		, mouseRButtonStatus(0)
		, moved(false)
		, updateState(0)
		, shouldClose(false)
	{
	}

	GLFWwindow* window;
	int width;
	int height;
	float theta;
	float phi;

	double time;
	double deltaTime;
	int frameCounter;

	int mouseLButtonStatus;
	int mouseMButtonStatus;
	int mouseRButtonStatus;
	Vector4 mouse;
	Vector2 mouseDelta;

	bool moved;

	int updateState;
	bool shouldClose;
};

GLData glData;

void framebuffer_size_callback(void* win, int width, int height)
{
	GLFWwindow* window = (GLFWwindow*)win;
	glData.width = width;
	glData.height = height;
}

static void glfw_error_callback(int error_code, const char* description)
{
	Platform::Debug("GLFW Error: [%d] %s\n", error_code, description);
}

void processInput(void* win)
{
	GLFWwindow* window = (GLFWwindow*)win;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	
	static double xlastpos = 0;
	static double ylastpos = 0;
	double xpos = 0;
	double ypos = 0;
	glfwGetCursorPos(window, &xpos, &ypos);
	ypos = glData.height - ypos;
	glData.mouseDelta = Vector2(xpos - xlastpos, ypos - ylastpos);
	xlastpos = xpos;
	ylastpos = ypos;

	glData.mouseMButtonStatus = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
	glData.mouseRButtonStatus = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

	int oldMouseButtonStatus = glData.mouseLButtonStatus;
	glData.mouseLButtonStatus = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (glData.mouseLButtonStatus == GLFW_PRESS)
	{
		if (oldMouseButtonStatus != glData.mouseLButtonStatus)
		{
			glData.mouse.Z() = 1;
			glData.mouse.W() = 1;
		}
		else
		{
			glData.mouse.Z() = 1;
			glData.mouse.W() = 0;
		}

		glData.mouse.X() = xpos;
		glData.mouse.Y() = ypos;
	}
	else if (glData.mouseLButtonStatus == GLFW_RELEASE)
	{
		if (oldMouseButtonStatus != glData.mouseLButtonStatus)
		{
			//Platform::Debug("mouse unclick\n");
		}
		else
		{
			//Platform::Debug("mouse release\n");
		}

		glData.mouse.Z() = -1;
		glData.mouse.W() = 0;
	}

	glData.moved = (fabs(glData.mouseDelta.X()) < 0.01f) && (fabs(glData.mouseDelta.Y()) < 0.01f);
	glData.theta += glData.mouseDelta.X() / glData.width * 180.0f * 2.0f;
	glData.theta = fmod(glData.theta, 180.0f * 2.0f);
	glData.phi -= glData.mouseDelta.Y() / glData.height * 180.0f;

	if (glData.phi > 70)
	{
		glData.phi = 70;
	}
	else if (glData.phi < -70)
	{
		glData.phi = -70;
	}
}

bool Video::Manager::IsKeyPressed(char key) const
{
	return glfwGetKey(glData.window, GLFW_KEY_0 + key - '0') == GLFW_PRESS;
}

bool Video::Manager::IsKeyRelease(char key) const
{
	return glfwGetKey(glData.window, GLFW_KEY_0 + key - '0') == GLFW_RELEASE;
}

bool Video::Manager::IsKeyRepeat(char key) const
{
	return glfwGetKey(glData.window, GLFW_KEY_0 + key - '0') == GLFW_REPEAT;
}

void Video::Manager::GetTheta(float& theta_, float& phi_) const
{
	theta_ = glData.theta;
	phi_ = glData.phi;
}

double Video::Manager::GetTime() const
{
	return glData.time;
}

double Video::Manager::GetDeltaTime() const
{
	return glData.deltaTime;
}

Vector4 Video::Manager::GetMouse() const
{
	return glData.mouse;
}

Vector2 Video::Manager::GetMouseDelta() const
{
	return glData.mouseDelta;
}

int Video::Manager::GetMouseLeftButtonStatus() const
{
	return glData.mouseLButtonStatus;
}

int Video::Manager::GetMouseMidButtonStatus() const
{
	return glData.mouseMButtonStatus;
}

int Video::Manager::GetMouseRightButtonStatus() const
{
	return glData.mouseRButtonStatus;
}

int Video::Manager::GetFrameCounter() const
{
	return glData.frameCounter;
}

unsigned int Video::Manager::GetWidth() const
{
	return glData.width;
}

unsigned int Video::Manager::GetHeight() const
{
	return glData.height;
}

bool Video::Manager::ShouldClose() const
{
	return glData.shouldClose;
}

Video::Manager::Manager()
{
}

Video::Manager::~Manager()
{
}

Video::Manager& Video::Manager::GetInstance()
{
	static Video::Manager instance;

	return instance;
}

bool Video::Manager::Initialize()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glData.width = SCR_WIDTH;
	glData.height = SCR_HEIGHT;
	glData.window = glfwCreateWindow(glData.width, glData.height, "MacShaderDemo", NULL, NULL);
	if (glData.window == NULL)
	{
		Platform::Debug("Failed to create GLFW window\n");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(glData.window);
	glfwSetFramebufferSizeCallback(glData.window, (void (*)(GLFWwindow*, int, int))(framebuffer_size_callback));

	//glfwSetErrorCallback(glfw_error_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Platform::Debug("Failed to initialize GLAD\n");
		return false;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(glData.window, true);
	ImGui_ImplOpenGL3_Init();

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	return true;
}

bool Video::Manager::Process()
{
	if (glData.frameCounter == 0)
	{
		glData.time = glfwGetTime();
	}

	glData.shouldClose = glfwWindowShouldClose(glData.window);

	double now = glfwGetTime();
	glData.deltaTime = now - glData.time;
	glData.time = now;

	glfwPollEvents();
	processInput(glData.window);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (glData.updateState == 0) // running
	{
		if (glfwGetKey(glData.window, ']'))
		{
			glData.updateState = 1; // paused
		}
	}
	else if (glData.updateState == 1) // paused
	{
		if (glfwGetKey(glData.window, ']'))
		{
			glData.updateState = 1; // running
		}
	}

	glClearColor(0., 0., 0., 0.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	for (auto& camera : cameras)
	{
		camera->Render();

		for (auto& renderer : renderers)
		{
			renderer->Render();
		}

		for (auto& graphics3 : graphics3s)
		{
			graphics3->Render();
		}
	}

	// Rendering
	ImGui::Render();

	// glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
	// glUseProgram(0);
	ImDrawData* drawData = ImGui::GetDrawData();
	if (drawData)
		ImGui_ImplOpenGL3_RenderDrawData(drawData);
	//glUseProgram(last_program);

	glfwMakeContextCurrent(glData.window);
	glfwSwapBuffers(glData.window);

	glData.frameCounter++;

	return true;
}

bool Video::Manager::Pause()
{
	return true;
}

void Video::Manager::Resume()
{
}

void Video::Manager::Terminate()
{
	cameras.clear();
	renderers.clear();
	graphics3s.clear();

	glfwDestroyWindow(glData.window);
	glfwTerminate();
}

void Video::Manager::Add(CameraComponent* camera)
{
	auto itr = std::find(cameras.begin(), cameras.end(), camera);
	if (itr != cameras.end())
	{
		Platform::Error("duplicated CameraComponent is declared\n");
		return;
	}

	cameras.push_back(camera);
}

void Video::Manager::Add(RendererComponent* renderer)
{
	auto itr = std::find(renderers.begin(), renderers.end(), renderer);
	if (itr != renderers.end())
	{
		Platform::Error("duplicated RendererComponent is declared\n");
		return;
	}

	renderers.push_back(renderer);
}

void Video::Manager::Add(Graphics3Component* graphics3)
{
	auto itr = std::find(graphics3s.begin(), graphics3s.end(), graphics3);
	if (itr != graphics3s.end())
	{
		Platform::Error("duplicated Graphics3Component is declared\n");
		return;
	}

	graphics3s.push_back(graphics3);
}

void Video::Manager::Remove(CameraComponent* camera)
{
	auto itr = std::find(cameras.begin(), cameras.end(), camera);
	if (itr != cameras.end())
		cameras.erase(itr);
}

void Video::Manager::Remove(RendererComponent* renderer)
{
	auto itr = std::find(renderers.begin(), renderers.end(), renderer);
	if (itr != renderers.end())
		renderers.erase(itr);
}

void Video::Manager::Remove(Graphics3Component* graphics3)
{
	auto itr = std::find(graphics3s.begin(), graphics3s.end(), graphics3);
	if (itr != graphics3s.end())
		graphics3s.erase(itr);
}

/////////////////////////////////////////////////////////////////////
bool Video::Service::Initialize()
{
	return Video::Manager::GetInstance().Initialize();
}

bool Video::Service::Process()
{
	return Video::Manager::GetInstance().Process();
}

bool Video::Service::Pause()
{
	return Video::Manager::GetInstance().Pause();
}

void Video::Service::Resume()
{
	Video::Manager::GetInstance().Resume();
}

void Video::Service::Terminate()
{
	Video::Manager::GetInstance().Terminate();
}