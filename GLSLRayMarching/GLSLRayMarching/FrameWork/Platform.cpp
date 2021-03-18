//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Platform.h"

std::string appName = "";
std::string initialScene = "";
void* handle = nullptr;
int width = 0;
int height = 0;

bool keystates[KEY_COUNT] = {};
Platform::Mouse mouse = { 0, 0, 0, 0, 0 };

double currentTime = 0;
double deltaTime = 0;
int totalFrameCounter = 0;
int sceneFrameCounter = 0;
bool quitApp = false;

Platform::JoyStick joySticks[JOYSTICK_COUNT];
int joyStickCount = 0;

#if (PLATFORM == WINDOWS_64)
#include <time.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

void framebuffer_size_callback(void* win, int width_, int height_)
{
	GLFWwindow* window = (GLFWwindow*)win;
	width = width_;
	height = height_;
}

static void glfw_error_callback(int error_code, const char* description)
{
	Debug("GLFW Error: [%d] %s\n", error_code, description);
}
#endif

bool Platform::Instantiate(int width_, int height_, const char* appName_, const char* initialScene_)
{
#if (PLATFORM == WINDOWS_64)
	currentTime = glfwGetTime();
	deltaTime = 0;
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	appName = appName_;
	initialScene = initialScene_;
	width = width_;
	height = height_;
	handle = glfwCreateWindow(width, height, appName.c_str(), NULL, NULL);
	if (handle == nullptr)
	{
		Debug("Failed to create GLFW window\n");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent((GLFWwindow*)handle);
	glfwSetFramebufferSizeCallback((GLFWwindow*)handle, (void (*)(GLFWwindow*, int, int))(framebuffer_size_callback));
	glfwSetErrorCallback(glfw_error_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Debug("Failed to initialize GLAD\n");
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
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)handle, true);
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

#elif (PLATFORM == MACOSX)
#elif (PLATFORM == LINUX)
#elif (PLATFORM == ANDROID)
#elif (PLATFORM == IOS)
#elif (PLATFORM == PS4)
#elif (PLATFORM == XBOXONE)
#elif (PLATFORM == NSWITCH)
#elif (PLATFORM == PS5)
#elif (PLATFORM == XSX)
#else
#pragma error("unsupported PLATFORM type. Please make sure PLATFORM is defined")
#endif

	return true;
}

#include <iostream>

bool Platform::PreUpdate()
{
#if (PLATFORM == WINDOWS_64)
	///////////////////////////////////////////////////
	// time
	double now = glfwGetTime();
	deltaTime = now - currentTime;
	currentTime = now;

	///////////////////////////////////////////////////
	glfwPollEvents();

	///////////////////////////////////////////////////
	// Quit Signal
	if (glfwGetKey((GLFWwindow*)handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose((GLFWwindow*)handle, true);
	}

	if(glfwWindowShouldClose((GLFWwindow*)handle))
		quitApp = true;

	///////////////////////////////////////////////////
	// Mouse Button
	for (int i = GLFW_MOUSE_BUTTON_1; i < GLFW_MOUSE_BUTTON_8; i++)
	{
		if (glfwGetMouseButton((GLFWwindow*)handle, i))
			mouse.button |= ((0x01 << i));
		else
			mouse.button &= (~(0x01 << i));
	}

	//if (glfwGetMouseButton((GLFWwindow*)handle, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
		//Debug("GLFW_PRESS %d\n", mouseButton);
	//else if (glfwGetMouseButton((GLFWwindow*)handle, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE)
		//Debug("GLFW_RELEASE %d\n", mouseButton);
	//else if (glfwGetMouseButton((GLFWwindow*)handle, GLFW_MOUSE_BUTTON_1) == GLFW_REPEAT)
		//Debug("GLFW_REPEAT %d\n", mouseButton);
	for (int i = GLFW_KEY_A; i < GLFW_KEY_Z; i++)
	{
		keystates[i] = (glfwGetKey((GLFWwindow*)handle, i) == GLFW_PRESS);
	}

	///////////////////////////////////////////////////
	// Mouse Movement
	double newMouseX = 0;
	double newMouseY = 0;
	glfwGetCursorPos((GLFWwindow*)handle, &newMouseX, &newMouseY);
	newMouseY = GetHeight() - 1 - newMouseY;
	mouse.dx = newMouseX - mouse.x;
	mouse.dy = newMouseY - mouse.y;
	mouse.x = newMouseX;
	mouse.y = newMouseY;
	//Debug("%f %f %f %f\n", mousex, mousey, mousedx, mousedy);

	///////////////////////////////////////////////////
	// JoyStick
	for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_16 && i<JOYSTICK_COUNT; i++)
	{
		if (glfwJoystickPresent(i))
		{
			const char* name = glfwGetJoystickName(i);
			
			//std::cout << name << ":";

			int count;
			const float* axes = glfwGetJoystickAxes(i, &count);
			for (int j = 0; j < count && j< JOYSTICK_AXE_COUNT; j++)
			{
				joySticks[i].axis[j] = axes[j];
				//std::cout << ((fabs(axes[j]) < 0.0001) ? 0 :axes[j]) << " ,";
			}
						
			const unsigned char* buttons = glfwGetJoystickButtons(i, &count);
			for (int j = 0; j < count && j < JOYSTICK_BUTTON_COUNT; j++)
			{
				joySticks[i].button[j] = buttons[j];
				//std::cout << (buttons[j] ? '1' : '0');
			}

			//std::cout << "\n";

			joyStickCount++;
		}
	}

	///////////////////////////////////////////////////
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
#elif (PLATFORM == MACOSX)
#elif (PLATFORM == LINUX)
#elif (PLATFORM == ANDROID)
#elif (PLATFORM == IOS)
#elif (PLATFORM == PS4)
#elif (PLATFORM == XBOXONE)
#elif (PLATFORM == NSWITCH)
#elif (PLATFORM == PS5)
#elif (PLATFORM == XSX)
#else
	#pragma error("unsupported PLATFORM type. Please make sure PLATFORM is defined")
#endif

	return true;
}

bool Platform::PostUpdate()
{
#if (PLATFORM == WINDOWS_64)
	// Rendering
	ImGui::Render();

	// glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
	// glUseProgram(0);
	ImDrawData* drawData = ImGui::GetDrawData();
	if (drawData)
		ImGui_ImplOpenGL3_RenderDrawData(drawData);
	//glUseProgram(last_program);

	glfwMakeContextCurrent((GLFWwindow*)handle);
	glfwSwapBuffers((GLFWwindow*)handle);

	totalFrameCounter++;
	sceneFrameCounter++;
#elif (PLATFORM == MACOSX)
#elif (PLATFORM == LINUX)
#elif (PLATFORM == ANDROID)
#elif (PLATFORM == IOS)
#elif (PLATFORM == PS4)
#elif (PLATFORM == XBOXONE)
#elif (PLATFORM == NSWITCH)
#elif (PLATFORM == PS5)
#elif (PLATFORM == XSX)
#else
#pragma error("unsupported PLATFORM type. Please make sure PLATFORM is defined")
#endif

	return true;
}

bool Platform::Pause()
{
	return true;
}

void Platform::Resume()
{
}

void Platform::Terminate()
{
#if (PLATFORM == WINDOWS_64)
	glfwDestroyWindow((GLFWwindow*)handle);
	glfwTerminate();
#elif (PLATFORM == MACOSX)
#elif (PLATFORM == LINUX)
#elif (PLATFORM == ANDROID)
#elif (PLATFORM == IOS)
#elif (PLATFORM == PS4)
#elif (PLATFORM == XBOXONE)
#elif (PLATFORM == NSWITCH)
#elif (PLATFORM == PS5)
#elif (PLATFORM == XSX)
#else
#pragma error("unsupported PLATFORM type. Please make sure PLATFORM is defined")
#endif
}

void* Platform::GetHandle()
{
	return handle;
}

const std::string& Platform::GetAppName()
{
	return appName;
}

const std::string& Platform::GetInitialScene()
{
	return initialScene;
}

int Platform::GetWidth()
{
	return width;
}

int Platform::GetHeight()
{
	return height;
}

int Platform::GetKeyCount()
{
	return KEY_COUNT;
}

bool Platform::GetKey(int i)
{
	return keystates[i];
}

int Platform::GetJoyStickCount()
{
	return joyStickCount;
}

Platform::JoyStick Platform::GetJoyStick(int i)
{
	return joySticks[i];
}

Platform::Mouse Platform::GetMouse()
{
	return mouse;
}

bool Platform::GetMouseButton(int which)
{
	return mouse.button & (1<<which);
}

float Platform::GetMouseX()
{
	return mouse.x;
}

float Platform::GetMouseY()
{
	return mouse.y;
}

float Platform::GetMouseDX()
{
	return mouse.dx;
}

float Platform::GetMouseDY()
{
	return mouse.dy;
}

double Platform::GetTime()
{
	return currentTime;
}

double Platform::GetDeltaTime()
{
	return deltaTime;
}

int Platform::GetTotalFrameCounter()
{
	return totalFrameCounter;
}

int Platform::GetSceneFrameCounter()
{
	return sceneFrameCounter;
}

void Platform::ResetSceneFrameCounter()
{
	sceneFrameCounter = -1;
}

bool Platform::QuitApp()
{
	return quitApp;
}

const char* Format(const char* format, ...)
{
	static char buffer[4096];

	va_list aptr;
	int ret;

	va_start(aptr, format);
	ret = vsprintf(buffer, format, aptr);
	va_end(aptr);

	return (const char*)buffer;
}

void Verbose(const char* format, ...)
{
	char buffer[4096];

	va_list aptr;
	int ret;

	va_start(aptr, format);
	ret = vsprintf(buffer, format, aptr);
	va_end(aptr);

	printf("Verbose: %s", buffer);
}

void Debug(const char* format, ...)
{
	char buffer[4096];

	va_list aptr;
	int ret;

	va_start(aptr, format);
	ret = vsprintf(buffer, format, aptr);
	va_end(aptr);

	printf("Debug: %s", buffer);
}

void Info(const char* format, ...)
{
	char buffer[4096];

	va_list aptr;
	int ret;

	va_start(aptr, format);
	ret = vsprintf(buffer, format, aptr);
	va_end(aptr);

	printf("Info: %s", buffer);
}

void Warning(const char* format, ...)
{
	char buffer[4096];

	va_list aptr;
	int ret;

	va_start(aptr, format);
	ret = vsprintf(buffer, format, aptr);
	va_end(aptr);

	printf("Warning: %s", buffer);
}

void Error(const char* format, ...)
{
	char buffer[4096];

	va_list aptr;
	int ret;

	va_start(aptr, format);
	ret = vsprintf(buffer, format, aptr);
	va_end(aptr);

	printf("Error: %s", buffer);
}

void MemSet(void* dst, int val, int size)
{
	::memset(dst, val, size);
}

void MemCpy(void* dst, const void* src, int size)
{
	::memcpy(dst, src, size);
}

int MemCmp(const void* s1, const void* s2, int size)
{
	return ::memcmp(s1, s2, size);
}