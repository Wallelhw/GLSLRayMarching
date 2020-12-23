#include "FrameWork.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "VertexArrayObject.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 400

class GussianBlurDemo : public FrameWork
{
public:
	GussianBlurDemo()
	: FrameWork("GussianBlurDemo")
	{
	}

	virtual ~GussianBlurDemo()
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

		if (!vertexArrayObject.Create(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0])))
		{
			return false;
		}

		if (!simpleMap.Create("../assets/bilateral.bmp"))
		{
			return false;
		}

		if (!simpleShaderProgram.Create("SimpleVS.glsl", "SimplePS.glsl"))
		{
			return false;
		}

		for (int i = 0; i < 2; i++)
		{
			if (!pingpongTexture[i].Create(SCR_WIDTH, SCR_HEIGHT, 4, true))
			{
				return false;
			}
		}		

		if (!gussianFilterShaderProgram.Create("GussianFilterVS.glsl", "GussianFilterPS.glsl"))
		{
			return false;
		}

		if (!bilateralFilterShaderProgram.Create("BilateralFilterVS.glsl", "BilateralFilterPS.glsl"))
		{
			return false;
		}

		if (!blitShaderProgram.Create("BlitVS.glsl", "BlitPS.glsl"))
		{
			return false;
		}

		return true;
	}

	void CalcSigma(float *kernel, int kernelSize, float sigma)
	{
		float total = 0;
		for (int x = 0; x < kernelSize; x++)
		{
			kernel[x] = exp(-(x * x) / (2 * sigma * sigma)) / sqrt(2 * 3.1415 * sigma * sigma);
			if (x != 0)
				total += kernel[x] * 2;
			else
				total += kernel[x];
		}

		for (int x = 0; x < kernelSize; x++)
		{
			kernel[x] /= total;
		}
	}

	void SetGuassianKernel(int kernelSize, float sigma)
	{
		std::vector<float> kernel(kernelSize);
		CalcSigma(&kernel[0], kernelSize, sigma);

		gussianFilterShaderProgram.SetUniform1i("kernelSize", kernelSize);
		gussianFilterShaderProgram.SetUniform1fv("kernel", kernelSize, &kernel[0]);
	}

	void SetGuassianKernel()
	{
		static int kernelSize = 5;
		static int kernelSizeDir = 1;
		if (IsKeyPressed('G'))
		{
			kernelSize += kernelSizeDir;
			if (kernelSize > 32)
			{
				kernelSize = 32;
				kernelSizeDir = -1;
			}
			if (kernelSize < 1)
			{
				kernelSize = 1;
				kernelSizeDir = +1;
			}

			printf("kernelSize = %d\n", kernelSize);
		}

		static float sigma = 1.75;
		static float sigmaDir = 0.01;
		if (IsKeyPressed('T'))
		{
			sigma += sigmaDir;
			if (sigma > 20.0)
			{
				sigma = 20.0;
				sigmaDir = -1;
			}
			if (sigma < 0)
			{
				sigma = 0;
				sigmaDir = +1;
			}

			printf("sigma = %f\n", sigma);
		}
		
		SetGuassianKernel(kernelSize, sigma);
	}

	void SetBilateralKernel(int kernelSize, float spaceSigma, float colorSigma)
	{
		bilateralFilterShaderProgram.SetUniform1i("kernelSize", kernelSize);
		bilateralFilterShaderProgram.SetUniform1f("spaceSigma", spaceSigma);
		bilateralFilterShaderProgram.SetUniform1f("colorSigma", colorSigma);
	}

	void SetBilateralKernel()
	{
		static int kernelSize = 15;
		static int kernelSizeDir = 1;
		if (IsKeyPressed('R'))
		{
			kernelSize += kernelSizeDir;
			if (kernelSize > 100)
			{
				kernelSize = 100;
				kernelSizeDir = -1;
			}
			if (kernelSize < 1)
			{
				kernelSize = 1;
				kernelSizeDir = +1;
			}

			printf("kernelSize = %d\n", kernelSize);
		}

		static float spaceSigma = 0.1;
		if (IsKeyPressed('T'))
		{
			spaceSigma += 0.1;
			if (spaceSigma > 100.0)
			{
				spaceSigma = 100.0;
			}
			printf("spaceSigma = %f\n", spaceSigma);
		}
		if (IsKeyPressed('G'))
		{
			spaceSigma -= 0.1;
			if (spaceSigma < 0)
			{
				spaceSigma = 0;
			}
			printf("spaceSigma = %f\n", spaceSigma);
		}

		static float colorSigma = 0.5;
		if (IsKeyPressed('Y'))
		{
			colorSigma += 0.01;
			if (colorSigma > 100.0)
			{
				colorSigma = 100.0;
			}

			printf("colorSigma = %f\n", colorSigma);
		}
		if (IsKeyPressed('H'))
		{
			colorSigma -= 0.01;
			if (colorSigma < 0)
			{
				colorSigma = 0;
			}

			printf("colorSigma = %f\n", colorSigma);
		}

		SetBilateralKernel(kernelSize, spaceSigma, colorSigma);
	}
	
	virtual bool OnUpdate() override
	{
		static int blur = 1;
		static int blurdir = 1;
		if (IsKeyPressed('B'))
		{
			blur += blurdir;
			if (blur > 300)
			{
				blur = 300;
				blurdir = -1;
			}
			if (blur < 0)
			{
				blur = 0;
				blurdir = +1;
			}

			printf("blur = %d\n", blur);
		}

		//////////////////////////////////////////////////////
		glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		frameBufferTexture.BindFrameBuffer();
		simpleMap.Bind(0);
		simpleShaderProgram.Bind();
		simpleShaderProgram.SetUniform2f("screenSize", SCR_WIDTH, SCR_HEIGHT);
		simpleShaderProgram.SetUniform1i("frameBufferTexture", 0);
		vertexArrayObject.Bind();
		vertexArrayObject.Draw(GL_TRIANGLES, 6);

		//////////////////////////////////////////////////////
		int last = 0;
		for (int i = 0; i < blur*2; i++)
		{
			int ping = (i+0)%2;
			int pong = (i+1)%2;

			pingpongTexture[pong].BindFrameBuffer();
			if (i == 0)
			{
				frameBufferTexture.Bind(0);

				frameBufferTexture.SetWarpS(GL_CLAMP);
				frameBufferTexture.SetWarpT(GL_CLAMP);
				frameBufferTexture.SetWarpR(GL_CLAMP);
			}
			else
			{
				pingpongTexture[ping].Bind(0);

				pingpongTexture[ping].SetWarpS(GL_CLAMP);
				pingpongTexture[ping].SetWarpT(GL_CLAMP);
				pingpongTexture[ping].SetWarpR(GL_CLAMP);
			}

#ifdef GUASSIAN
			gussianFilterShaderProgram.Bind();
			gussianFilterShaderProgram.SetUniform2f("screenSize", SCR_WIDTH, SCR_HEIGHT);
			gussianFilterShaderProgram.SetUniform1i("frameBufferTexture", 0);
			gussianFilterShaderProgram.SetUniform1i("horizontal", i % 2);
			SetGuassianKernel();
#else
			bilateralFilterShaderProgram.Bind();
			bilateralFilterShaderProgram.SetUniform2f("screenSize", SCR_WIDTH, SCR_HEIGHT);
			bilateralFilterShaderProgram.SetUniform1i("frameBufferTexture", 0);
			bilateralFilterShaderProgram.SetUniform1i("horizontal", i % 2);
			SetBilateralKernel();
#endif

			vertexArrayObject.Bind();
			vertexArrayObject.Draw(GL_TRIANGLES, 6);

			last = pong;
		}

		pingpongTexture[last].UnBindFrameBuffer();
		pingpongTexture[last].Bind(0);
		blitShaderProgram.Bind();
		blitShaderProgram.SetUniform2f("screenSize", SCR_WIDTH, SCR_HEIGHT);
		blitShaderProgram.SetUniform1i("frameBufferTexture", 0);
		vertexArrayObject.Bind();
		vertexArrayObject.Draw(GL_TRIANGLES, 6);

		return true;
	}

	void OnDestroy() override
	{
		frameBufferTexture.Destroy();
		vertexArrayObject.Destroy();

		simpleMap.Destroy();
		simpleShaderProgram.Destroy();

		for (int i = 0; i < 2; i++)
			pingpongTexture[i].Destroy();
		
		gussianFilterShaderProgram.Destroy();
		bilateralFilterShaderProgram.Destroy();

		blitShaderProgram.Destroy();
	}
private:
	RenderTarget2D frameBufferTexture;
	VertexArrayObject vertexArrayObject;

	Texture2D simpleMap;
	ShaderProgram simpleShaderProgram;

	RenderTarget2D pingpongTexture[2];
	ShaderProgram gussianFilterShaderProgram;
	ShaderProgram bilateralFilterShaderProgram;

	ShaderProgram blitShaderProgram;
};

int main()
{
	GussianBlurDemo chapter;

	if (!chapter.Create(SCR_WIDTH, SCR_HEIGHT))
		return -1;

	chapter.Start();

	chapter.Destroy();

	return 0;
}