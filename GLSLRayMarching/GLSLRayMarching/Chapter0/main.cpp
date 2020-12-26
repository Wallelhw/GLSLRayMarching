#include "FrameWork.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "VertexArrayObject.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 400

class Chapter0 : public FrameWork
{
public:
	Chapter0()
	: FrameWork("Chapter0")
	{
	}

	virtual ~Chapter0()
	{
	}

	void TestMonteCarlo()
	{

	}

	virtual bool OnCreate() override
	{
		if (!simpleMap.Create("../assets/alpha.png", false))
		{
			return false;
		}

		if (!proprocessingShaderProgram.Create("BlitVS.glsl", "BlitPS.glsl"))
		{
			return false;
		}

		axisCount = 10;

		TestMonteCarlo();
		return true;
	}

	virtual bool OnUpdate() override
	{
		if (IsKeyPressed(' '))
		{
			axisCount++;
			if (axisCount > 1000)
				axisCount = 1;

			printf("%d\n", axisCount);
		}

		//////////////////////////////////////////////////////
		glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		simpleMap.Bind(0);
		proprocessingShaderProgram.Bind();
		proprocessingShaderProgram.SetUniform2f("screenSize", SCR_WIDTH, SCR_HEIGHT);
		proprocessingShaderProgram.SetUniform1i("frameBufferTexture", 0);

		if (!UpdateKDOP(axisCount))
			return false;

		vertexArrayObject.Bind();
		vertexArrayObject.Draw(GL_TRIANGLES, 6);

		return true;
	}

	bool UpdateKDOP(int axisCount)
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

		vertexArrayObject.Destroy();
		if (!vertexArrayObject.Create(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0])))
		{
			return false;
		}

		return true;
	}

	void OnDestroy() override
	{
		simpleMap.Destroy();
		proprocessingShaderProgram.Destroy();
		vertexArrayObject.Destroy();
	}
private:
	Texture2DFile simpleMap;
	ShaderProgram proprocessingShaderProgram;
	VertexArrayObject vertexArrayObject;

	int axisCount;
};

int main()
{
	Chapter0 chapter;

	if (!chapter.Create(SCR_WIDTH, SCR_HEIGHT))
		return -1;

	chapter.Start();

	chapter.Destroy();

	return 0;
}