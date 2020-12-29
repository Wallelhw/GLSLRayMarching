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

		void SetWorldTransform(const mat4& worldTransform_)
		{
			worldTransform = worldTransform_;

			viewTransform = worldTransform.Inverse();
		}

		const mat4& GetWorldTransform() const
		{
			return worldTransform;
		}

		const mat4& GetViewTransform() const
		{
			return viewTransform;
		}

		void SetProjectionTransform(const mat4& projectionTransform_)
		{
			projectionTransform = projectionTransform_;
		}

		const mat4& GetProjectionTransform() const
		{
			return projectionTransform;
		}

		mat4 worldTransform;
		mat4 viewTransform;
		mat4 projectionTransform;
	};

	GeometryTexture()
	: FrameWork("GeometryTexture")
	{
	}

	virtual ~GeometryTexture()
	{
	}

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

		bool success = vertexArrayObject
			.Begin()
			.FillVertices(0, 2, VertexAttribute::FLOAT, false, 0, 0, &vertices[0], sizeof(vertices)/sizeof(vertices[0]))
			.End();
		if (!success)
		{
			return false;
		}

		if (!geometryTexture.Create("bunny.p65.gim256.fmp.bmp", false))
		{
			return false;
		}
		geometryTexture.SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
		geometryTexture.SetMagFilter(GL_LINEAR);
		geometryTexture.SetWarpS(GL_REPEAT);
		geometryTexture.SetWarpR(GL_REPEAT);
		geometryTexture.SetWarpT(GL_REPEAT);

		if (!normalTexture.Create("bunny.p65.nim512.bmp", false))
		{
			return false;
		}
		normalTexture.SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
		normalTexture.SetMagFilter(GL_LINEAR);
		normalTexture.SetWarpS(GL_REPEAT);
		normalTexture.SetWarpR(GL_REPEAT);
		normalTexture.SetWarpT(GL_REPEAT);

		if (!geometryTextureShaderProgram.Create("BlitVS.glsl", "BlitPS.glsl"))
		{
			return false;
		}

		return true;
	}

	virtual bool OnUpdate() override
	{
		static int start = 0;

		static float lod = 0;
		if (IsKeyPressed('A'))
		{
			lod -= 0.016f;
			if (lod < 0)
				lod = 0;

			printf("%f\n", lod);
		}
		if (IsKeyPressed('D'))
		{
			lod += 0.016f;
			if (lod > 7)
				lod = 7;

			printf("%f\n", lod);
		}

		static float test1 = 0.0f;
		test1 += 1;
		//worldTransform.SetTranslate(test1, 0, 0);
		worldTransform.SetTranslateRotXYZScale(0, 0, 0, 0, test1, 0, 6.0);
		camera.SetWorldTransform(worldTransform);

		mat4 cameraTransform;
		cameraTransform.SetLookAt(vec3(5, 5, 5), vec3(0, 0, 0), vec3(0, 1, 0));
		camera.SetWorldTransform(cameraTransform);
		
		mat4 projectionTransform;
		projectionTransform.SetPerspectiveFov(45.0f, float(SCR_WIDTH) / SCR_HEIGHT, 1.0f, 1000.0f);
		camera.SetProjectionTransform(projectionTransform);

		//////////////////////////////////////////////////////
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		geometryTexture.Bind(0);
		normalTexture.Bind(1);

		geometryTextureShaderProgram.Bind();
		geometryTextureShaderProgram.SetUniform1i("geometryTexture", 0);
		geometryTextureShaderProgram.SetUniform1i("normalTexture", 1);
		geometryTextureShaderProgram.SetUniformMatrix4fv("worldTransform", 1, worldTransform);
		geometryTextureShaderProgram.SetUniformMatrix4fv("viewTransform", 1, camera.GetViewTransform());
		geometryTextureShaderProgram.SetUniformMatrix4fv("projTransform", 1, camera.GetProjectionTransform());

		geometryTextureShaderProgram.SetUniform1i("lod", floor(lod));

		float scale = pow(2.0, floor(lod));
		int triangleCount = GEOMETRY_TEXTURE_SIZE * GEOMETRY_TEXTURE_SIZE / (scale) / (scale);
		printf("%f: %f %f %d\n", lod, floor(lod), scale, triangleCount);

		vertexArrayObject.Bind();
		vertexArrayObject.DrawInstanced(GL_TRIANGLES, triangleCount);

		return true;
	}

	void OnDestroy() override
	{
		geometryTexture.Destroy();
		normalTexture.Destroy();

		geometryTextureShaderProgram.Destroy();
		vertexArrayObject.Destroy();
	}
private:
	Texture2DFile geometryTexture;
	Texture2DFile normalTexture;

	ShaderProgram geometryTextureShaderProgram;

	mat4 worldTransform;
	Camera camera;
	
	VertexArrayObject vertexArrayObject;
};

int main()
{
	GeometryTexture chapter;

	if (!chapter.Create(SCR_WIDTH, SCR_HEIGHT))
		return -1;

	chapter.Start();

	chapter.Destroy();

	return 0;
}