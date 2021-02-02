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

	class TransformData
	{
	public:
		Matrix4 viewTransform;
		Matrix4 projTransform;
		int lod;
	};

	#define VECTOR_WIDTH  4
	struct VertexData
	{
		float px[VECTOR_WIDTH];
		float py[VECTOR_WIDTH];
	};

	virtual bool OnCreate() override
	{
		////////////////////////////////////////////////////////////
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

		////////////////////////////////////////////////////////////
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

		////////////////////////////////////////////////////////////
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

		if(	!shaderStorageBlockBuffer
			.Begin(Buffer::Type::SHADER_STORAGE_BUFFER, Buffer::Usage::STATIC_DRAW)
			.Fill(vertexData, sizeof(VertexData) * 4)
			.End()
		)
		{
			return false;
		}
		//buffer.BindShaderStorage(geometryTextureShaderProgram, 0);
		geometryTextureShaderProgram.BindShaderStorageBuffer(shaderStorageBlockBuffer, "VertexData", 0);
		// geometryTextureShaderProgram.BindShaderStorageBuffer(shaderStorageBlockBuffer, 0);

		////////////////////////////////////////////////////////////
		TransformData transformData;

		if (!uniformBlockBuffer
			.Begin(Buffer::Type::UNIFORM_BUFFER, Buffer::Usage::STATIC_DRAW)
			.Fill(&transformData, sizeof(TransformData))
			.End()
			)
		{
			return false;
		}
		
		geometryTextureShaderProgram.BindUniformBlock(uniformBlockBuffer, "TransformData", 0);
		// geometryTextureShaderProgram.BindUniformBlock(uniformBlockBuffer, 0);

		return true;
	}

	void TestGUI(bool& wireframe, int& lod, float& ratio)
	{
		static bool enabled1 = false;
		static bool enabled2 = false;
		static bool enabled3 = false;
		static Vector4 c(0, 0, 0, 0);

		if (ImGui::Begin("SuperGameObject"))
		{
			if (ImGui::CollapsingHeader("Lod"))
			{
				ImGui::SliderInt("Lod", &lod, 0, 7, "%d");
			}
			if (ImGui::CollapsingHeader("Ratio"))
			{
				ImGui::SliderFloat("Ratio", &ratio, 0.0, 100.0, "%f");
			}
			if (ImGui::CollapsingHeader("WireFrame?"))
			{
				ImGui::Checkbox("WireFrame", &wireframe);
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

		shaderStorageBlockBuffer.Update(0, vertexData, sizeof(VertexData) * 4);
	}

	virtual bool OnUpdate() override
	{
		static int lod = 0;
		static bool wireframe = true;
		static float ratio = 0.0;
		TestGUI(wireframe, lod, ratio);

		static float test1 = 0.0f;
		test1 += 1;
		//worldTransform.SetTranslate(test1, 0, 0);
		worldTransform.SetTranslateRotXYZScale(0, 0, 0, 0, test1, 0, 6.0);
		camera.SetWorldTransform(worldTransform);

		Matrix4 cameraTransform;
		cameraTransform.SetLookAt(Vector3(5, 5, 5), Vector3(0, 0, 0), Vector3(0, 1, 0));
		camera.SetWorldTransform(cameraTransform);

		Matrix4 projectionTransform;
		projectionTransform.SetPerspectiveFov(90.0f, float(SCR_WIDTH) / SCR_HEIGHT, 1.0f, 1000.0f);
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
		geometryTextureShaderProgram.SetUniform1f("ratio", ratio/100.0f);
		geometryTextureShaderProgram.SetUniformMatrix4x4fv("worldTransform", 1, worldTransform);

#define USE_UNIFORM_BLOCK
#ifdef USE_UNIFORM_BLOCK
		TransformData transformData;
		transformData.viewTransform = camera.GetViewTransform().Transpose();
		transformData.projTransform = camera.GetProjectionTransform().Transpose();
		transformData.lod = lod;
		uniformBlockBuffer.Update(0, &transformData, sizeof(TransformData));
#else
		geometryTextureShaderProgram.SetUniformMatrix4x4fv("viewTransform", 1, camera.GetViewTransform());
		geometryTextureShaderProgram.SetUniformMatrix4x4fv("projTransform", 1, camera.GetProjectionTransform());
		geometryTextureShaderProgram.SetUniform1i("lod", lod);
#endif

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
	Buffer shaderStorageBlockBuffer;
	Buffer uniformBlockBuffer;
	RenderStates renderStates;
	Primitives primitives;
};

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