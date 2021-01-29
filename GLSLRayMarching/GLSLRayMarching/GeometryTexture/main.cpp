#include "FrameWork.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Primitives.h"
#include "RenderStates.h"
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
		}

		ImGui::End();
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
	Matrix4 worldTransform;
	Camera camera;

	Texture2DFile geometryTexture;
	Texture2DFile normalTexture;
	ShaderProgram geometryTextureShaderProgram;
	RenderStates renderStates;
	Primitives primitives;
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