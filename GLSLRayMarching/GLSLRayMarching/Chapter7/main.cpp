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

class Chapter7 : public FrameWork
{
public:
	Chapter7()
		: FrameWork("Chapter7")
	{
	}

	virtual ~Chapter7()
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

		bool success =
			vertexArrayObject
			.Begin()
			.FillVertices(0, 3, VertexAttribute::FLOAT, false, 0, 0, vertices, sizeof(vertices) / sizeof(vertices[0]))
			.FillIndices(indices, sizeof(indices) / sizeof(indices[0]))
			.End();
		if (!success)
		{
			return false;
		}

		cameraPos = vec3(0.0, 0.0, 2.0);
		cameraTarget = vec3(0, 0, 0);
		cameraUp = vec3(0, 1, 0);

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

		float theta;
		float phi;
		GetTheta(theta, phi);

		/*
		float y = sin(phi * 3.14 / 180.0f);
		float x = cos(phi * 3.14 / 180.0f) * cos(theta * 3.14 / 180.0f);
		float z = cos(phi * 3.14 / 180.0f) * sin(theta * 3.14 / 180.0f);

		cameraTarget[0] = cameraPos[0] + x;
		cameraTarget[1] = cameraPos[1] + y;
		cameraTarget[2] = cameraPos[2] + z;
		if (IsKeyPressed('W'))
		{
			cameraPos += (cameraTarget - cameraPos) * 0.016;
		}
		if (IsKeyPressed('S'))
		{
			cameraPos -= (cameraTarget - cameraPos) * 0.016;
		}
		if (IsKeyPressed('D'))
		{
			cameraPos += (cameraTarget - cameraPos).Cross(vec3(0, 1, 0)) * 0.016;
		}
		if (IsKeyPressed('A'))
		{
			cameraPos -= (cameraTarget - cameraPos).Cross(vec3(0, 1, 0)) * 0.016;
		}
		*/

		static float materialRoughness = 1.0;
		if (IsKeyPressed('R'))
		{
			materialRoughness += 0.01;
			if (materialRoughness > 1.0)
			{
				materialRoughness = 1.0;
			}

			printf("materialRoughness = %3.2f\n", materialRoughness);
		}
		if (IsKeyPressed('F'))
		{
			materialRoughness -= 0.01;
			if (materialRoughness < 0.0)
			{
				materialRoughness = 0.0;
			}

			printf("materialRoughness = %3.2f\n", materialRoughness);
		}

		static float materialMetallic = 0.0;
		if (IsKeyPressed('T'))
		{
			materialMetallic += 0.01;
			if (materialMetallic > 1.0)
			{
				materialMetallic = 1.0;
			}

			printf("materialMetallic = %3.2f\n", materialMetallic);
		}
		if (IsKeyPressed('G'))
		{
			materialMetallic -= 0.01;
			if (materialMetallic < 0.0)
			{
				materialMetallic = 0.0;
			}

			printf("materialMetallic = %3.2f\n", materialMetallic);
		}
		static float anisotropic = 0.0;
		if (IsKeyPressed('Y'))
		{
			anisotropic += 0.01;
			if (anisotropic > 1.0)
			{
				anisotropic = 1.0;
			}

			printf("anisotropic = %3.2f\n", anisotropic);
		}
		if (IsKeyPressed('H'))
		{
			anisotropic -= 0.01;
			if (anisotropic < -20.0)
			{
				anisotropic = -20.0;
			}

			printf("anisotropic = %3.2f\n", anisotropic);
		}

		//////////////////////////////////////////////////////
		frameBufferTexture.Bind();

		envMap.Bind(0);

		pathTraceShaderProgram.Bind();
		pathTraceShaderProgram.SetUniform2f("screenSize", SCR_WIDTH, SCR_HEIGHT);
		pathTraceShaderProgram.SetUniform1i("envMap", 0);
		pathTraceShaderProgram.SetUniform1f("envMapIntensity", 1.0);
		pathTraceShaderProgram.SetUniform1i("sampleCount", sampleCount);

		pathTraceShaderProgram.SetUniform3f("camera.origin", cameraPos[0], cameraPos[1], cameraPos[2]);
		pathTraceShaderProgram.SetUniform3f("camera.target", cameraTarget[0], cameraTarget[1], cameraTarget[2]);
		pathTraceShaderProgram.SetUniform3f("camera.up", cameraUp[0], cameraUp[1], cameraUp[2]);
		pathTraceShaderProgram.SetUniform1f("camera.vfov", 90.0f);
		pathTraceShaderProgram.SetUniform1f("camera.aspect", float(SCR_WIDTH) / SCR_HEIGHT);
		pathTraceShaderProgram.SetUniform1f("materialMetallic", materialMetallic);
		pathTraceShaderProgram.SetUniform1f("materialRoughness", materialRoughness);

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

	vec3 cameraPos;
	vec3 cameraTarget;
	vec3 cameraUp;
};

int main()
{
	Chapter7 chapter;

	if (!chapter.Create(SCR_WIDTH, SCR_HEIGHT))
		return -1;

	chapter.Start();

	chapter.Destroy();

	return 0;
}