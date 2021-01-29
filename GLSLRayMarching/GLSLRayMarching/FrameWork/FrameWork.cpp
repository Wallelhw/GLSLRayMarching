#include "FrameWork.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "GUI.h"

class FrameWorkImpl
{
public:
	FrameWorkImpl(const std::string& name_)
		: name(name_)
		, window(nullptr)
		, width(0)
		, height(0)
		, theta(0.0)
		, phi(0.0)
		, time(0.0)
		, deltaTime(0.0)
		, frameCounter(0)
		, mouse(0.0, 0.0, 0.0, 0.0)
		, mouseDelta(0.0, 0.0)
		, mouseButtonStatus(0)
		, moved(false)
	{
	}

	std::string name;
	GLFWwindow* window;
	int width;
	int height;
	float theta;
	float phi;

	double time;
	double deltaTime;
	int frameCounter;

	int mouseButtonStatus;
	Vector4 mouse;
	Vector2 mouseDelta;

	bool moved;
};

FrameWork* FrameWork::instance = nullptr;

//static void framebuffer_size_callback(void* window, int width, int height);
//static void processInput(void* window);

void FrameWork::framebuffer_size_callback(void* win, int width, int height)
{
	GLFWwindow* window = (GLFWwindow*)win;
	instance->impl->width = width;
	instance->impl->height = height;
}

void FrameWork::processInput(void* win)
{
	GLFWwindow* window = (GLFWwindow*)win;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	double xpos = 0;
	double ypos = 0;
	glfwGetCursorPos(window, &xpos, &ypos);
	ypos = instance->impl->height - ypos;

	int oldMouseButtonStatus = instance->impl->mouseButtonStatus;
	instance->impl->mouseButtonStatus = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (instance->impl->mouseButtonStatus == GLFW_PRESS)
	{
		if (oldMouseButtonStatus != instance->impl->mouseButtonStatus)
		{
			instance->impl->mouse.Z() = 1;
			instance->impl->mouse.W() = 1;
		}
		else
		{
			instance->impl->mouse.Z() = 1;
			instance->impl->mouse.W() = 0;
		}

		instance->impl->mouse.X() = xpos;
		instance->impl->mouse.Y() = ypos;
	}
	else if (instance->impl->mouseButtonStatus == GLFW_RELEASE)
	{
		if (oldMouseButtonStatus != instance->impl->mouseButtonStatus)
		{
			//Platform::Debug("mouse unclick\n");
		}
		else
		{
			//Platform::Debug("mouse release\n");
		}

		instance->impl->mouse.Z() = -1;
		instance->impl->mouse.W() = 0;
	}

	instance->impl->moved = (fabs(instance->impl->mouseDelta.X()) < 0.01f) && (fabs(instance->impl->mouseDelta.Y()) < 0.01f);
	instance->impl->theta += instance->impl->mouseDelta.X() / instance->impl->width * 180.0f * 2.0f;
	instance->impl->theta = fmod(instance->impl->theta, 180.0f * 2.0f);
	instance->impl->phi -= instance->impl->mouseDelta.Y() / instance->impl->height * 180.0f;

	if (instance->impl->phi > 70)
	{
		instance->impl->phi = 70;
	}
	else if (instance->impl->phi < -70)
	{
		instance->impl->phi = -70;
	}
}

FrameWork::FrameWork(const std::string& name_)
	: impl(nullptr)
{
	assert(!instance);
	instance = this;

	impl = new FrameWorkImpl(name_);
}

FrameWork::~FrameWork()
{
	if (impl)
	{
		delete impl;
		impl = nullptr;
	}

	instance = nullptr;
}

bool FrameWork::Create(int width_, int height_)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	impl->width = width_;
	impl->height = height_;
	impl->window = glfwCreateWindow(impl->width, impl->height, impl->name.c_str(), NULL, NULL);
	if (impl->window == NULL)
	{
		Platform::Debug("Failed to create GLFW window\n");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(impl->window);
	glfwSetFramebufferSizeCallback(impl->window, (void (*)(GLFWwindow*, int, int))(framebuffer_size_callback) );

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
	ImGui_ImplGlfw_InitForOpenGL(impl->window, true);
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

	if (!OnCreate())
	{
		Platform::Debug("Failed to init Scene\n");
		return false;
	}

	return true;
}

bool FrameWork::Start()
{
	if (impl->frameCounter == 0)
	{
		impl->time = glfwGetTime();
	}

	while (!glfwWindowShouldClose(impl->window))
	{
		double now = glfwGetTime();
		impl->deltaTime = now - impl->time;
		impl->time = now;

		glfwPollEvents();
		processInput(impl->window);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (!OnUpdate())
			return false;

		impl->frameCounter++;

		// Rendering
		ImGui::Render();

		// glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
		// glUseProgram(0);
		ImDrawData* drawData = ImGui::GetDrawData();
		if (drawData)
			ImGui_ImplOpenGL3_RenderDrawData(drawData);
		//glUseProgram(last_program);

		glfwMakeContextCurrent(impl->window);
		glfwSwapBuffers(impl->window);
	}

	return true;
}

void FrameWork::Destroy()
{
	OnDestroy();

	glfwDestroyWindow(impl->window);
	glfwTerminate();
}

bool FrameWork::IsKeyPressed(char key) const
{
	return glfwGetKey(impl->window, GLFW_KEY_0 + key - '0') == GLFW_PRESS;
}

bool FrameWork::IsKeyRelease(char key) const
{
	return glfwGetKey(impl->window, GLFW_KEY_0 + key - '0') == GLFW_RELEASE;
}

bool FrameWork::IsKeyRepeat(char key) const
{
	return glfwGetKey(impl->window, GLFW_KEY_0 + key - '0') == GLFW_REPEAT;
}

void FrameWork::GetTheta(float& theta_, float& phi_) const
{
	theta_ = impl->theta;
	phi_ = impl->phi;
}

double FrameWork::GetTime() const
{
	return impl->time;
}

double FrameWork::GetDeltaTime() const
{
	return impl->deltaTime;
}

Vector4 FrameWork::GetMouse() const
{
	return impl->mouse;
}

Vector2 FrameWork::GetMouseDelta() const
{
	return impl->mouseDelta;
}

int FrameWork::GetMouseButtonStatus() const
{
	return impl->mouseButtonStatus;
}

int FrameWork::GetFrameCounter() const
{
	return impl->frameCounter;
}

unsigned int FrameWork::GetWidth() const
{
	return impl->width;
}

unsigned int FrameWork::GetHeight() const
{
	return impl->height;
}