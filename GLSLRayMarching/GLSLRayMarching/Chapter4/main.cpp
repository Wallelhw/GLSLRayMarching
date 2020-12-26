#include "FrameWork.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "VertexArrayObject.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 400

class Chapter4 : public FrameWork
{
public:
	Chapter4()
		: FrameWork("Chapter4")
	{
	}

	virtual ~Chapter4()
	{
	}

	virtual bool OnCreate() override
	{
		float vertices[] = {
			 1.0f,  1.0f, 0.0f,  // top right
			 1.0f, -1.0f, 0.0f,  // bottom right
			-1.0f, -1.0f, 0.0f,  // bottom left
			-1.0f,  1.0f, 0.0f   // top left 
		};
		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		if (!frameBufferTexture.Create(SCR_WIDTH, SCR_HEIGHT, 4, true))
		{
			return false;
		}

		if (!envMap.Create("../assets/env1.png", false))
		{
			return false;
		}

		if (!pathTraceShaderProgram.Create("PathTraceVS.glsl", "PathTracePS.glsl"))
		{
			return false;
		}

		if (!proprocessingShaderProgram.Create("BlitVS.glsl", "BlitPS.glsl"))
		{
			return false;
		}

		if (!vertexArrayObject.Create(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0])))
		{
			return false;
		}

		sampleCount = 100;

		return true;
	}

	virtual bool OnUpdate() override
	{
		glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		if (IsKeyPressed(' '))
		{
			sampleCount++;
			if (sampleCount > 1000)
				sampleCount = 1;

			printf("%d\n", sampleCount);
		}

		//////////////////////////////////////////////////////
		frameBufferTexture.Bind();

		envMap.Bind(0);

		pathTraceShaderProgram.Bind();
		pathTraceShaderProgram.SetUniform2f("screenSize", SCR_WIDTH, SCR_HEIGHT);
		pathTraceShaderProgram.SetUniform1i("envMap", 0);
		pathTraceShaderProgram.SetUniform1f("envMapIntensity", 1.0);
		pathTraceShaderProgram.SetUniform1i("sampleCount", sampleCount);

		pathTraceShaderProgram.SetUniform3f("camera.origin", 0.0, 0.0, 2.0);
		pathTraceShaderProgram.SetUniform3f("camera.target", 0, 0, 0);
		pathTraceShaderProgram.SetUniform3f("camera.up", 0, 1, 0);
		pathTraceShaderProgram.SetUniform1f("camera.vfov", 90.0f);
		pathTraceShaderProgram.SetUniform1f("camera.aspect", float(SCR_WIDTH) / SCR_HEIGHT);

		vertexArrayObject.Bind();
		vertexArrayObject.Draw(GL_TRIANGLES, 6);

		//////////////////////////////////////////////////////
		frameBufferTexture.UnBind();

		frameBufferTexture.GetColorAttachment(GL_COLOR_ATTACHMENT0)->Bind(0);
		proprocessingShaderProgram.Bind();
		proprocessingShaderProgram.SetUniform2f("screenSize", SCR_WIDTH, SCR_HEIGHT);
		proprocessingShaderProgram.SetUniform1i("frameBufferTexture", 0);

		vertexArrayObject.Bind();
		vertexArrayObject.Draw(GL_TRIANGLES, 6);

		return true;
	}

	void OnDestroy() override
	{
		envMap.Destroy();
		pathTraceShaderProgram.Destroy();

		frameBufferTexture.Destroy();
		proprocessingShaderProgram.Destroy();
		vertexArrayObject.Destroy();
	}
private:
	Texture2DFrameBuffer frameBufferTexture;

	TextureCubeMapFile envMap;
	ShaderProgram pathTraceShaderProgram;

	ShaderProgram proprocessingShaderProgram;
	VertexArrayObject vertexArrayObject;

	int sampleCount;
};

int main()
{
	Chapter4 chapter;

	if (!chapter.Create(SCR_WIDTH, SCR_HEIGHT))
		return -1;

	chapter.Start();

	chapter.Destroy();

	return 0;
}