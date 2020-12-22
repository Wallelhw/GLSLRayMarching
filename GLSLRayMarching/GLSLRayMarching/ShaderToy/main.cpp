#include "FrameWork.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "VertexArrayObject.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 400

class ShaderToy : public FrameWork
{
public:
	ShaderToy()
		: FrameWork("ShaderToy")
	{
	}

	virtual ~ShaderToy()
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

		if (!shaderToyTraceShaderProgram.Create("PathTraceVS.glsl", "PathTracePS.glsl"))
		{
			return false;
		}

		if (!blitShaderProgram.Create("BlitVS.glsl", "BlitPS.glsl"))
		{
			return false;
		}

		if (!vertexArrayObject.Create(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0])))
		{
			return false;
		}

		return true;
	}

	virtual bool OnUpdate() override
	{
		glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//////////////////////////////////////////////////////
		frameBufferTexture.BindFrameBuffer();

		shaderToyTraceShaderProgram.Bind();

		vertexArrayObject.Bind();
		vertexArrayObject.Draw(GL_TRIANGLES, 6);

		//////////////////////////////////////////////////////
		frameBufferTexture.UnBindFrameBuffer();

		frameBufferTexture.Bind(0);
		blitShaderProgram.Bind();
		blitShaderProgram.SetUniform2f("screenSize", SCR_WIDTH, SCR_HEIGHT);
		blitShaderProgram.SetUniform1i("frameBufferTexture", 0);

		vertexArrayObject.Bind();
		vertexArrayObject.Draw(GL_TRIANGLES, 6);

		return true;
	}

	void OnDestroy() override
	{
		shaderToyTraceShaderProgram.Destroy();

		frameBufferTexture.Destroy();
		blitShaderProgram.Destroy();
		vertexArrayObject.Destroy();
	}
private:
	FrameBufferTexture2D frameBufferTexture;

	ShaderProgram shaderToyTraceShaderProgram;

	ShaderProgram blitShaderProgram;
	VertexArrayObject vertexArrayObject;
};

int main()
{
	ShaderToy chapter;

	if (!chapter.Create(SCR_WIDTH, SCR_HEIGHT))
		return -1;

	chapter.Start();

	chapter.Destroy();

	return 0;
}