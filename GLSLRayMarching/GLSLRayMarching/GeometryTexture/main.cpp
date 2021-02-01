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

#define SCR_WIDTH (800*2)
#define SCR_HEIGHT (400*2)

#define GEOMETRY_TEXTURE_SIZE 1024
#define NORMAL_TEXTURE_SIZE 512

class GeometryTexture : public FrameWork
{
public:
	class Camera
	{
	public:
		Camera()
		{
		}

		~Camera()
		{
		}

		void SetWorldTransform(const Matrix4& worldTransform_)
		{
			worldTransform = worldTransform_;

			viewTransform = worldTransform.Inverse();
		}

		const Matrix4& GetWorldTransform() const
		{
			return worldTransform;
		}

		const Matrix4& GetViewTransform() const
		{
			return viewTransform;
		}

		void SetProjectionTransform(const Matrix4& projectionTransform_)
		{
			projectionTransform = projectionTransform_;
		}

		const Matrix4& GetProjectionTransform() const
		{
			return projectionTransform;
		}

		Matrix4 worldTransform;
		Matrix4 viewTransform;
		Matrix4 projectionTransform;
	};

	GeometryTexture()
		: FrameWork("GeometryTexture")
	{
	}

	virtual ~GeometryTexture()
	{
	}

	#define VECTOR_WIDTH  4
	struct VertexData
	{
		float px[VECTOR_WIDTH];
		float py[VECTOR_WIDTH];
	};

	virtual bool OnCreate() override
	{
		float vertices[] =
		{
			0.0, 0.0,
			0.0, 1.0,
			1.0, 0.0,

			1.0, 0.0,
			0.0, 1.0,
			1.0, 1.0
		};

		bool success = primitives
			.Begin()
			.FillVertices(0, 2, VertexAttribute::DataType::FLOAT, false, 0, 0, &vertices[0], sizeof(vertices) / sizeof(vertices[0]) / 2)
			.End();
		if (!success)
		{
			return false;
		}

		if (!geometryTexture.Create("bunny.p65.gim256.fmp.bmp", false))
		{
			return false;
		}
		geometryTexture.SetMinFilter(Texture::MinFilter::LINEAR_MIPMAP_LINEAR);
		geometryTexture.SetMagFilter(Texture::MagFilter::LINEAR);
		geometryTexture.SetWarpS(Texture::Wrap::CLAMP);
		geometryTexture.SetWarpR(Texture::Wrap::CLAMP);
		geometryTexture.SetWarpT(Texture::Wrap::CLAMP);

		if (!normalTexture.Create("bunny.p65.nim512.bmp", false))
		{
			return false;
		}
		normalTexture.SetMinFilter(Texture::MinFilter::LINEAR_MIPMAP_LINEAR);
		normalTexture.SetMagFilter(Texture::MagFilter::LINEAR);
		normalTexture.SetWarpS(Texture::Wrap::REPEAT);
		normalTexture.SetWarpR(Texture::Wrap::REPEAT);
		normalTexture.SetWarpT(Texture::Wrap::REPEAT);

		if (!geometryTextureShaderProgram.Create("BlitVS.glsl", "BlitPS.glsl"))
		{
			return false;
		}

		VertexData temp[] = 
		{
			{
				{  0.0,  0.0,  0.0,  0.0}, {  4.0,  5.0,  6.0,  7.0},
			},
			{
				{  8.0,  9.0, 10.0, 11.0}, { 12.0, 13.0, 14.0, 15.0},
			},
			{
				{ 16.0, 17.0, 18.0, 19.0}, { 20.0, 21.0, 22.0, 23.0},
			},
			{
				{ 24.0, 25.0, 26.0, 27.0}, { 28.0, 29.0, 30.0, 31.0},
			}
		};
		memcpy(vertexData, temp, sizeof(VertexData) * 4);

		if(	!buffer
			.Begin(Buffer::Type::SHADER_STORAGE_BUFFER, Buffer::Usage::STATIC_DRAW)
			.Fill(vertexData, sizeof(VertexData) * 4)
			.End()
		)
		{
			return false;
		}

		//buffer.BindShader(geometryTextureShaderProgram, "VertexData", 0);
		geometryTextureShaderProgram.BindShaderStorageBuffer(buffer, "VertexData", 0);

		return true;
	}

	void TestGUI(bool& wireframe, int& lod)
	{
		static bool enabled1 = false;
		static bool enabled2 = false;
		static bool enabled3 = false;
		static Vector4 c(0, 0, 0, 0);

		if (ImGui::Begin("SuperGameObject"))
		{
			if (ImGui::CollapsingHeader("Lod"))
			{
				ImGui::SliderInt("Lod", &lod, 0, 7, "LOD %d");
			}
			if (ImGui::CollapsingHeader("WireFrame"))
			{
				ImGui::Checkbox("WireFrame1", &wireframe);
			}
			if (ImGui::CollapsingHeader("Shape and CSG"))
			{
				ImGui::Indent(10.0f);
				ImGui::Checkbox("Render Shape1", &enabled1);
				ImGui::ColorEdit4("Shape1 Color", &c[0], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
				ImGui::Checkbox("Shape1 Wire", &enabled2);
				ImGui::Checkbox("Shape1 Solid", &enabled3);
				ImGui::Unindent(10.0f);
			}

			if (ImGui::CollapsingHeader("vertexData[0].px[0]"))
			{
				ImGui::SliderFloat4("vertexData[0].px[0]", &vertexData[0].px[0], -1, 1, "%f");
			}
		}

		ImGui::End();

		buffer.Update(0, vertexData, sizeof(VertexData) * 4);
	}

	virtual bool OnUpdate() override
	{
		static int lod = 0;
		static bool wireframe = false;
		TestGUI(wireframe, lod);

		static float test1 = 0.0f;
		test1 += 1;
		//worldTransform.SetTranslate(test1, 0, 0);
		worldTransform.SetTranslateRotXYZScale(0, 0, 0, 0, test1, 0, 6.0);
		camera.SetWorldTransform(worldTransform);

		Matrix4 cameraTransform;
		cameraTransform.SetLookAt(Vector3(5, 5, 5), Vector3(0, 0, 0), Vector3(0, 1, 0));
		camera.SetWorldTransform(cameraTransform);

		Matrix4 projectionTransform;
		projectionTransform.SetPerspectiveFov(45.0f, float(SCR_WIDTH) / SCR_HEIGHT, 1.0f, 1000.0f);
		camera.SetProjectionTransform(projectionTransform);

		//////////////////////////////////////////////////////
		renderStates.scissorTestState.enabled = true;
		renderStates.scissorTestState.pos = Vector2(0, 0);
		renderStates.scissorTestState.size = Vector2(SCR_WIDTH, SCR_HEIGHT);
		renderStates.viewportState.pos = Vector2(0, 0);
		renderStates.viewportState.size = Vector2(SCR_WIDTH, SCR_HEIGHT);

		renderStates.clearState.clearColor = ColorRGBA(0.0f, 0.0f, 0.0f, 1.0f);

		renderStates.clearState.clearColor = ColorRGBA(0.0f, 0.0f, 0.0f, 1.0f);
		renderStates.clearState.clearDepth = 1.0f;
		renderStates.clearState.clearStencil = 0;
		renderStates.clearState.enableClearColor = true;
		renderStates.clearState.enableClearDepth = true;
		renderStates.clearState.enableClearStencil = true;

		renderStates.polygonModeState.face = PolygonModeState::Face::FRONT_AND_BACK;
		if (wireframe)
			renderStates.polygonModeState.mode = PolygonModeState::Mode::LINE;
		else
			renderStates.polygonModeState.mode = PolygonModeState::Mode::FILL;

		renderStates.depthTestState.enabled = true;
		renderStates.depthTestState.func = DepthTestState::Func::LEQUAL;
		renderStates.Apply();

		geometryTexture.Bind(0);
		normalTexture.Bind(1);

		geometryTextureShaderProgram.Bind();
		geometryTextureShaderProgram.SetUniform1i("geometryTexture", 0);
		geometryTextureShaderProgram.SetUniform1i("normalTexture", 1);
		geometryTextureShaderProgram.SetUniformMatrix4fv("worldTransform", 1, worldTransform);
		geometryTextureShaderProgram.SetUniformMatrix4fv("viewTransform", 1, camera.GetViewTransform());
		geometryTextureShaderProgram.SetUniformMatrix4fv("projTransform", 1, camera.GetProjectionTransform());

		geometryTextureShaderProgram.SetUniform1i("lod", lod);

		float scale = powf(2.0f, floor(lod));
		int triangleCount = (int)(GEOMETRY_TEXTURE_SIZE * GEOMETRY_TEXTURE_SIZE / (scale) / (scale));
		Platform::Debug("%f: %f %f %d\n", lod, floor(lod), scale, triangleCount);

		primitives.Bind();
		primitives.DrawArrayInstanced(Primitives::Mode::TRIANGLES, 0, primitives.GetCount(), triangleCount);

		return true;
	}

	void OnDestroy() override
	{
		geometryTexture.Destroy();
		normalTexture.Destroy();

		geometryTextureShaderProgram.Destroy();
		primitives.Destroy();
	}
private:
	VertexData vertexData[sizeof(VertexData) * 4];

	Matrix4 worldTransform;
	Camera camera;

	Texture2DFile geometryTexture;
	Texture2DFile normalTexture;
	ShaderProgram geometryTextureShaderProgram;
	Buffer buffer;
	RenderStates renderStates;
	Primitives primitives;
};

/*
#include <GLFW/glfw3.h>
#include <array>
#include <iostream>
#include <vector>

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void glfw_error_callback(int error_code, const char* description)
{
	std::cerr << "GLFW Error: [" << error_code << "] " << description << '\n';
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

auto create_glfw_window()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	return glfwCreateWindow(800, 600, "OpenGL and AoSoA layout", nullptr, nullptr);
}

void set_callbacks(GLFWwindow* window)
{
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetErrorCallback(glfw_error_callback);
}

void print_versions()
{
	std::cout << "Using GLFW " << glfwGetVersionString() << '\n';
}

void GLAPIENTRY MessageCallback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam = nullptr)
{
	std::cerr << "[GL DEBUG] " << (type == GL_DEBUG_TYPE_ERROR ? "Error: " : "") << message << '\n';
}

constexpr auto VECTOR_WIDTH = 4;
constexpr auto VERTEX_COUNT = 16;

struct VertexData
{
	std::array<float, VECTOR_WIDTH> px;
	std::array<float, VECTOR_WIDTH> py;
};

static const char* vertex_shader_source =
"#version 430\n"
"struct Vertex4\n"
"{\n"
"    float px[4]; // position x\n"
"    float py[4]; // position y\n"
"};\n"
"layout(std430, binding=0) buffer VertexData\n"
"{\n"
"    Vertex4 vertices[];\n"
"};\n"
"void main()\n"
"{\n"
"  int dataIx = gl_VertexID / 4;\n"
"  int vertexIx = gl_VertexID % 4;\n"
"  vec2 vertexPosition = vec2(vertices[dataIx].px[vertexIx], vertices[dataIx].py[vertexIx]);\n"
"}\n";

static const char* fragment_shader_source =
"#version 430\n"
"out vec4 out_color;\n"
"void main()\n"
"{\n"
"    out_color = vec4(1.0, 0.5, 0.5, 0.25);\n"
"}\n";

GLFWwindow* window;

int test(int argc, char* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	window = glfwCreateWindow(800, 600, "test", NULL, NULL);
	if (window == NULL)
	{
		Platform::Debug("Failed to create GLFW window\n");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, (void (*)(GLFWwindow*, int, int))(framebuffer_size_callback));

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Platform::Debug("Failed to initialize GLAD\n");
		return false;
	}

	glfwSetErrorCallback(glfw_error_callback);

	std::vector<VertexData> vertices(VERTEX_COUNT / VECTOR_WIDTH);
	vertices[0] = {
		{-0.75f, 0.75f, 0.75f, -0.75f},
		{-0.75f, -0.75f, 0.75f, 0.75f}
	};
	vertices[1] = {
		{-0.50f, 0.50f, 0.50f, -0.50f},
		{-0.50f, -0.50f, 0.50f, 0.50f},
	};
	vertices[2] = {
		{-0.25f, 0.25f, 0.25f, -0.25f},
		{-0.25f, -0.25f, 0.25f, 0.25f},
	};
	vertices[3] = {
		{-0.05f, 0.05f, 0.05f, -0.05f},
		{-0.05f, -0.05f, 0.05f, 0.05f},
	};

	auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
	glCompileShader(vertex_shader);

	auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
	glCompileShader(fragment_shader);

	auto program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	const GLuint position_attrib_index = 0;
	glBindAttribLocation(program, position_attrib_index, "vertexPosition");

	glLinkProgram(program);

	//glUseProgram(program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	//
	// SSBO
	//
	int err;
	err = glGetError();
	const GLuint ssbo_binding_point = 0;
	GLuint ssbo{};
	glGenBuffers(1, &ssbo);
	err = glGetError();

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	err = glGetError();

	glBufferData(GL_SHADER_STORAGE_BUFFER, vertices.size() * sizeof(VertexData), vertices.data(), GL_STATIC_DRAW);
	err = glGetError();

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	err = glGetError();

	const auto block_index = glGetProgramResourceIndex(program, GL_SHADER_STORAGE_BLOCK, "VertexData");
	err = glGetError();

	glShaderStorageBlockBinding(program, block_index, ssbo_binding_point);
	err = glGetError();

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding_point, ssbo);
	err = glGetError();

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	err = glGetError();

	//
	// VAO
	//
	//GLuint vao{};
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);
	//glEnableVertexAttribArray(position_attrib_index);
	//glVertexAttribPointer(position_attrib_index, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glClearColor(0.15f, 0.15f, 0.2f, 1.0f);
	glPointSize(10.0f);

	while (!glfwWindowShouldClose(window)) 
	{
		process_input(window);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		//glDrawArrays(GL_POINTS, 0, VERTEX_COUNT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
*/
int main(int argc, char* argv[])
{
	//test(argc, argv);

	GeometryTexture chapter;

	if (!chapter.Create(SCR_WIDTH, SCR_HEIGHT))
		return -1;

	chapter.Start();

	chapter.Destroy();

	return 0;
}