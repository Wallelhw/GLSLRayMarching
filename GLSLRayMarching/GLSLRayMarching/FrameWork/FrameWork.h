#ifndef _FrameWork_h_
#define _FrameWork_h_

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw.h>
#include <imgui\imgui_impl_opengl3.h>
#include <stdio.h>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <iostream>
#include <assert.h>
#include <string>
#include <map>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

class FrameWork;
static FrameWork* instance = nullptr;

class FrameWork
{
public:
	FrameWork(const std::string& name_)
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
	{
		assert(!instance);

		instance = this;
	}

	virtual ~FrameWork()
	{
		instance = nullptr;
	}

	bool Create(int width_, int height_)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
		width = width_;
		height = height_;
		window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
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
		ImGui_ImplGlfw_InitForOpenGL(window, true);
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
			std::cout << "Failed to init Scene" << std::endl;
			return false;
		}

		return true;
	}

	bool Start()
	{
		if (frameCounter == 0)
		{
			time = glfwGetTime();
		}

		while (!glfwWindowShouldClose(window))
		{
			double now = glfwGetTime();
			deltaTime = now - time;
			time = now;

			glfwPollEvents();
			processInput(window);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (!OnUpdate())
				return false;

			frameCounter++;

			// Rendering
			ImGui::Render();

			// glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
			// glUseProgram(0);
			ImDrawData* drawData = ImGui::GetDrawData();
			if (drawData)
				ImGui_ImplOpenGL3_RenderDrawData(drawData);
			//glUseProgram(last_program);

			glfwMakeContextCurrent(window);
			glfwSwapBuffers(window);
		}

		return true;
	}

	void Destroy()
	{
		OnDestroy();

		glfwDestroyWindow(window);
		glfwTerminate();
	}
protected:
	bool IsKeyPressed(char key)
	{
		return glfwGetKey(window, GLFW_KEY_0 + key - '0') == GLFW_PRESS;
	}

	bool IsKeyRelease(char key)
	{
		return glfwGetKey(window, GLFW_KEY_0 + key - '0') == GLFW_RELEASE;
	}

	bool IsKeyRepeat(char key)
	{
		return glfwGetKey(window, GLFW_KEY_0 + key - '0') == GLFW_REPEAT;
	}

	void GetTheta(float& theta_, float& phi_)
	{
		theta_ = theta;
		phi_ = phi;
	}

	double GetTime()
	{
		return time;
	}

	double GetDeltaTime()
	{
		return deltaTime;
	}

	vec4 GetMouse()
	{
		return mouse;
	}

	vec2 GetMouseDelta()
	{
		return mouseDelta;
	}

	int GetMouseButtonStatus()
	{
		return mouseButtonStatus;
	}

	int GetFrameCounter()
	{
		return frameCounter;
	}

	virtual bool OnCreate() = 0;
	virtual bool OnUpdate() = 0;
	virtual void OnDestroy() = 0;
private:
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	static void processInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}

		double xpos = 0;
		double ypos = 0;
		glfwGetCursorPos(window, &xpos, &ypos);
		ypos = instance->height - ypos;
		
		int oldMouseButtonStatus = instance->mouseButtonStatus;
		instance->mouseButtonStatus = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if (instance->mouseButtonStatus== GLFW_PRESS)
		{
			if (oldMouseButtonStatus != instance->mouseButtonStatus)
			{
				instance->mouse.Z() = 1;
				instance->mouse.W() = 1;
			}
			else
			{
				instance->mouse.Z() = 1;
				instance->mouse.W() = 0;
			}

			instance->mouse.X() = xpos;
			instance->mouse.Y() = ypos;
		}
		else if (instance->mouseButtonStatus== GLFW_RELEASE)
		{
			if (oldMouseButtonStatus != instance->mouseButtonStatus)
			{
				//printf("mouse unclick\n");
			}
			else
			{
				//printf("mouse release\n");
			}

			instance->mouse.Z() = -1;
			instance->mouse.W() = 0;
		}

		instance->moved = (fabs(instance->mouseDelta.X()) < 0.01f) && (fabs(instance->mouseDelta.Y()) < 0.01f);
		instance->theta += instance->mouseDelta.X() / instance->width * 180.0f * 2.0f;
		instance->theta = fmod(instance->theta, 180.0f * 2.0f);
		instance->phi -= instance->mouseDelta.Y() / instance->height * 180.0f;

		if (instance->phi > 70)
		{
			instance->phi = 70;
		}
		else if (instance->phi < -70)
		{
			instance->phi = -70;
		}
	}
protected:
	bool moved;

private:
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
	vec4 mouse;
	vec2 mouseDelta;
};

#endif