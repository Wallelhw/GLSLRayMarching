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

class Terrain
{
public:
	class GeoMipmap
	{
	public:
		GeoMipmap()
		{
		}

		~GeoMipmap()
		{
		}

		void Create(unsigned int width_, unsigned int height_, bool coarseLeft_, bool coarseRight_, bool coarseTop_, bool coarseBottom_)
		{
			width = width_;
			height = height_;

			AddCenter();
			AddLeftSide(coarseLeft_, coarseTop_, coarseBottom_);
			AddRightSide(coarseRight_, coarseTop_, coarseBottom_);
			AddTopSide(coarseTop_, coarseLeft_, coarseRight_);
			AddBottomSide(coarseBottom_, coarseLeft_, coarseRight_);
		}
	private:
		void AddCenter()
		{
			AddGrid(1, 1, width - 2, height - 2);
		}

		void AddLeftSide(bool coarseLeft_, bool coarseTop_, bool coarseBottom_)
		{
			if (coarseLeft_)
			{
			}
			else
			{
				AddGrid(0, 1, 1, height - 2);
			}
		}

		void AddRightSide(bool coarseRight_, bool coarseTop_, bool coarseBottom_)
		{
			if (coarseRight_)
			{
			}
			else
			{
				AddGrid(width - 1, 1, 1, height - 2);
			}
		}

		void AddTopSide(bool coarseTop_, bool coarseLeft_, bool coarseRight_)
		{
			if (coarseTop_)
			{
			}
			else
			{
				AddGrid(1, 0, width - 2, 1);
			}
		}

		void AddBottomSide(bool coarseBottom_, bool coarseLeft_, bool coarseRight_)
		{
			if (coarseBottom_)
			{
			}
			else
			{
				AddGrid(1, height - 1, width - 2, 1);
			}
		}

		void AddLODQuadLeft(unsigned int x_, unsigned int y_, bool triangle1_, bool triangle2_)
		{
			if (triangle1_)
			{
				AddVertex(x_ + 1, y_ + 0);
				AddVertex(x_ + 0, y_ + 0);
				AddVertex(x_ + 1, y_ + 1);
			}

			if (triangle2_)
			{
				AddVertex(x_ + 1, y_ + 1);
				AddVertex(x_ + 0, y_ + 2);
				AddVertex(x_ + 1, y_ + 2);
			}

			AddVertex(x_ + 0, y_ + 0);
			AddVertex(x_ + 0, y_ + 2);
			AddVertex(x_ + 1, y_ + 1);
		}

		void AddLODQuadRight(unsigned int x_, unsigned int y_, bool triangle1_, bool triangle2_)
		{
			if (triangle1_)
			{
				AddVertex(x_ + 0, y_ + 0);
				AddVertex(x_ + 1, y_ + 0);
				AddVertex(x_ + 0, y_ + 1);
			}

			if (triangle2_)
			{
				AddVertex(x_ + 0, y_ + 1);
				AddVertex(x_ + 1, y_ + 2);
				AddVertex(x_ + 0, y_ + 2);
			}

			AddVertex(x_ + 0, y_ + 1);
			AddVertex(x_ + 1, y_ + 0);
			AddVertex(x_ + 1, y_ + 2);
		}

		void AddLODQuadTop(unsigned int x_, unsigned int y_, bool triangle1_, bool triangle2_)
		{
			if (triangle1_)
			{
				AddVertex(x_ + 0, y_ + 1);
				AddVertex(x_ + 0, y_ + 0);
				AddVertex(x_ + 1, y_ + 1);
			}

			if (triangle2_)
			{
				AddVertex(x_ + 1, y_ + 1);
				AddVertex(x_ + 2, y_ + 0);
				AddVertex(x_ + 2, y_ + 1);
			}

			AddVertex(x_ + 0, y_ + 0);
			AddVertex(x_ + 2, y_ + 0);
			AddVertex(x_ + 1, y_ + 1);
		}

		void AddLODQuadBottom(unsigned int x_, unsigned int y_, bool triangle1_, bool triangle2_)
		{
			if (triangle1_)
			{
				AddVertex(x_ + 0, y_ + 0);
				AddVertex(x_ + 0, y_ + 1);
				AddVertex(x_ + 1, y_ + 0);
			}

			AddVertex(x_ + 1, y_ + 0);
			AddVertex(x_ + 0, y_ + 1);
			AddVertex(x_ + 2, y_ + 1);

			if (triangle2_)
			{
				AddVertex(x_ + 1, y_ + 0);
				AddVertex(x_ + 2, y_ + 1);
				AddVertex(x_ + 2, y_ + 0);
			}
		}

		void AddGrid(int xstart, int ystart, int width, int height)
		{
			for (int y = ystart; y < ystart + height; y++)
			{
				for (int x = xstart; x < xstart + width; x++)
				{
					AddQuad(x, y, (((x+y) % 2) == 0));
				}
			}
		}

		void AddQuad(unsigned int x_, unsigned int y_, bool even_)
		{
			if (even_)
			{
				AddVertex(x_ + 0, y_ + 1);
				AddVertex(x_ + 0, y_ + 0);
				AddVertex(x_ + 1, y_ + 1);
				AddVertex(x_ + 1, y_ + 1);
				AddVertex(x_ + 0, y_ + 0);
				AddVertex(x_ + 1, y_ + 0);
			}
			else
			{
				AddVertex(x_ + 0, y_ + 0);
				AddVertex(x_ + 0, y_ + 1);
				AddVertex(x_ + 1, y_ + 0);
				AddVertex(x_ + 1, y_ + 0);
				AddVertex(x_ + 0, y_ + 1);
				AddVertex(x_ + 1, y_ + 1);
			}
		}

		void AddVertex(unsigned int x_, unsigned int y_)
		{
			indices.push_back(y_ * width + x_);
		}

		unsigned int width;
		unsigned int height;
		std::vector<unsigned short> indices;
	};

	class GeoMipmaps
	{
	public:
		GeoMipmaps()
		{
		}

		~GeoMipmaps()
		{
		}

		void Create(unsigned int width, unsigned int height, bool up, bool down, bool left, bool right)
		{
		}
	private:
		void Add(unsigned int width, unsigned int height, bool up, bool down, bool left, bool right)
		{

		}
	};

	class Section
	{
	public:
		Section()
			: sectionsSize(0, 0)
			, heightField(nullptr)
			, vbOffset(0)
		{
		}

		~Section()
		{
		}

		bool Create(const IVector2& sectionsSize_, unsigned int& vbOffset_)
		{
			sectionsSize = sectionsSize_;
			vbOffset = vbOffset_;

			vbOffset_ += GetDataCount();

			return true;
		}

		void Destroy()
		{
			sectionsSize = IVector2(0, 0);

			vbOffset = 0;
		}

		const IVector2& GetSectionsSize() const
		{
			return sectionsSize;
		}

		unsigned int GetDataCount()
		{
			return (sectionsSize.X() * sectionsSize.Y());
		}

		unsigned int GetVBOffset()
		{
			return vbOffset;
		}

		unsigned int I(int x, int y) const
		{
			return y * sectionsSize.X() + x;
		}

		IVector2 sectionsSize;
		Buffer* heightField;

		unsigned int vbOffset;
	};

	class Component
	{
	public:
		Component()
			: sectionsPerComponent(0, 0)
			, heightField(nullptr)
			, sections()
		{
		}

		~Component()
		{
		}

		bool Create(const IVector2& sectionsPerComponent_, const IVector2& sectionsSize_, unsigned int& offset_)
		{
			sectionsPerComponent = sectionsPerComponent_;

			sections.resize(GetSectionCount());

			for (int y = 0; y < sectionsPerComponent.Y(); y++)
			{
				for (int x = 0; x < sectionsPerComponent.X(); x++)
				{
					if (!GetSection(x, y).Create(sectionsSize_, offset_))
					{
						return false;
					}
				}
			}

			return true;
		}

		void Destroy()
		{
			sectionsPerComponent = IVector2(0, 0);
			heightField = nullptr;

			for (auto& section : sections)
			{
				section.Destroy();
			}

			sections.clear();
		}

		const IVector2& GetSectionsPerComponent() const
		{
			return sectionsPerComponent;
		}

		unsigned int GetSectionCount()
		{
			return (sectionsPerComponent.X() * sectionsPerComponent.Y());
		}

		Section& GetSection(int x, int y)
		{
			return sections[I(x, y)];
		}

		const Section& GetSection(int x, int y) const
		{
			return sections[I(x, y)];
		}

		unsigned int I(int x, int y) const
		{
			return y * sectionsPerComponent.X() + x;
		}

		IVector2 sectionsPerComponent;
		Buffer* heightField;

		std::vector<Section> sections;
	};

	Terrain()
		: componentSize(0, 0)
		, heightField()
		, components()
	{
	}

	~Terrain()
	{
	}

	bool Create(const IVector2& componentSize_,
		const IVector2& sectionsPerComponent_,
		const IVector2& sectionsSize_,
		const float* heightData_)
	{
		componentSize = componentSize_;

		unsigned int heightDataSize = GetHeightDataSize(componentSize_, sectionsPerComponent_, sectionsSize_);
		if (!heightField
			.Begin(Buffer::Type::SHADER_STORAGE_BUFFER, Buffer::Usage::STATIC_DRAW)
			.Fill((void*)heightData_, sizeof(float) * heightDataSize)
			.End()
			)
		{
			return false;
		}

		unsigned int offset = 0;
		components.resize(GetComponentCount());
		for (int y = 0; y < componentSize.Y(); y++)
		{
			for (int x = 0; x < componentSize.X(); x++)
			{
				if (!GetComponent(x, y).Create(sectionsPerComponent_, sectionsSize_, offset))
				{
					return false;
				}
			}
		}

		return true;
	}

	void Destroy()
	{
		componentSize = IVector2(0, 0);

		for (auto& component : components)
		{
			component.Destroy();
		}
		components.clear();
	}

	const IVector2& GetComponentSize() const
	{
		return componentSize;
	}

	unsigned int GetComponentCount()
	{
		return (componentSize.X() * componentSize.Y());
	}

	Component& GetComponent(int x, int y)
	{
		return components[I(x, y)];
	}

	const Component& GetComponent(int x, int y) const
	{
		return components[I(x, y)];
	}

	unsigned int I(int x, int y) const
	{
		return y * componentSize.X() + x;
	}

	static bool RearrangeData(std::vector<float> dst_,
		const IVector2& componentSize_, const IVector2& sectionsPerComponent_, const IVector2& sectionsSize_,
		std::vector<float> src_)
	{
		unsigned int heightDataSize = GetHeightDataSize(componentSize_, sectionsPerComponent_, sectionsSize_);

		return true;
	}

	static unsigned int GetHeightDataSize(const IVector2& componentCount_, const IVector2& sectionsPerComponent_, const IVector2& sectionsSize_)
	{
		unsigned int components = componentCount_.X() * componentCount_.Y();
		unsigned int sectionsPerComponents = sectionsPerComponent_.X() * sectionsPerComponent_.Y();
		unsigned int pixelPerSection = sectionsSize_.X() * sectionsSize_.Y();

		return pixelPerSection * sectionsPerComponents * components;
	}

	IVector2 componentSize;
	Buffer heightField;

	std::vector<Component> components;
};


class GeoMipmapDemo : public FrameWork
{
public:
	GeoMipmapDemo()
		: FrameWork("GeoMipmapDemo")
	{
	}

	virtual ~GeoMipmapDemo()
	{
	}

	class TestTransformData
	{
	public:
		Matrix4 viewTransform;
		Matrix4 projTransform;
		int lod;
		float ratio;
	};

#define VECTOR_WIDTH  4
	struct TestShaderBufferData
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
		geometryTexture.SetMinFilter(Texture::MinFilter::LinearMipmapLinear);
		geometryTexture.SetMagFilter(Texture::MagFilter::Linear);
		geometryTexture.SetWarpS(Texture::Wrap::Clamp);
		geometryTexture.SetWarpR(Texture::Wrap::Clamp);
		geometryTexture.SetWarpT(Texture::Wrap::Clamp);

		if (!normalTexture.Create("bunny.p65.nim512.bmp", false))
		{
			return false;
		}
		normalTexture.SetMinFilter(Texture::MinFilter::LinearMipmapLinear);
		normalTexture.SetMagFilter(Texture::MagFilter::Linear);
		normalTexture.SetWarpS(Texture::Wrap::Repeat);
		normalTexture.SetWarpR(Texture::Wrap::Repeat);
		normalTexture.SetWarpT(Texture::Wrap::Repeat);

		////////////////////////////////////////////////////////////
		if (!geometryTextureShaderProgram.Create("BlitVS.glsl", "BlitPS.glsl"))
		{
			return false;
		}

		TestShaderBufferData temp[] =
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
		memcpy(testShaderBufferData, temp, sizeof(TestShaderBufferData) * 4);

		if (!shaderStorageBlockBuffer
			.Begin(Buffer::Type::SHADER_STORAGE_BUFFER, Buffer::Usage::STATIC_DRAW)
			.Fill(testShaderBufferData, sizeof(TestShaderBufferData) * 4)
			.End()
			)
		{
			return false;
		}
		//buffer.BindShaderStorage(geometryTextureShaderProgram, 0);
		geometryTextureShaderProgram.BindShaderStorageBuffer(shaderStorageBlockBuffer, "VertexData", 0);
		// geometryTextureShaderProgram.BindShaderStorageBuffer(shaderStorageBlockBuffer, 0);

		////////////////////////////////////////////////////////////
		TestTransformData transformData;
		if (!uniformBlockBuffer
			.Begin(Buffer::Type::UNIFORM_BUFFER, Buffer::Usage::STATIC_DRAW)
			.Fill(&transformData, sizeof(TestTransformData))
			.End()
			)
		{
			return false;
		}

		geometryTextureShaderProgram.BindUniformBlock(uniformBlockBuffer, "TransformData", 0);
		// geometryTextureShaderProgram.BindUniformBlock(uniformBlockBuffer, 0);

		return true;
	}

	void UpdateShader(bool& wireframe, int& lod, float& ratio)
	{
		GUI::Test2(lod, ratio, wireframe, testShaderBufferData[0].px[0]);

		geometryTextureShaderProgram.Bind();
		geometryTextureShaderProgram.SetUniform1i("geometryTexture", 0);
		geometryTextureShaderProgram.SetUniform1i("normalTexture", 1);
		geometryTextureShaderProgram.SetUniformMatrix4x4fv("worldTransform", 1, worldTransform);

#define USE_UNIFORM_BLOCK
#ifdef USE_UNIFORM_BLOCK
		TestTransformData transformData;
		transformData.viewTransform = camera.GetViewTransform().Transpose();
		transformData.projTransform = camera.GetProjectionTransform().Transpose();
		transformData.lod = lod;
		transformData.ratio = ratio / 100.0f;
		uniformBlockBuffer.Update(0, &transformData, sizeof(TestTransformData));
#else
		geometryTextureShaderProgram.SetUniformMatrix4x4fv("viewTransform", 1, camera.GetViewTransform());
		geometryTextureShaderProgram.SetUniformMatrix4x4fv("projTransform", 1, camera.GetProjectionTransform());
		geometryTextureShaderProgram.SetUniform1i("lod", lod);
		geometryTextureShaderProgram.SetUniform1f("ratio", ratio / 100.0f);
#endif

		shaderStorageBlockBuffer.Update(0, testShaderBufferData, sizeof(TestShaderBufferData) * 4);
	}

	virtual bool OnUpdate() override
	{
		unsigned int count = geometryTextureShaderProgram.GetActiveUniformCount();

		std::string name;
		UniformType uniformType;
		int size;
		for (int i = 0; i < count; i++)
		{
			geometryTextureShaderProgram.GetActiveUniformInfo(i, name, uniformType, size);
		}

		static int lod = 0;
		static bool wireframe = true;
		static float ratio = 0.0;
		UpdateShader(wireframe, lod, ratio);

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

		renderStates.depthTestState.depthTestEnabled = true;
		renderStates.depthTestState.depthWriteEnabled = true;
		renderStates.depthTestState.func = DepthTestState::Func::LEQUAL;
		renderStates.Apply();

		geometryTexture.Bind(0);
		normalTexture.Bind(1);

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

	TestShaderBufferData testShaderBufferData[sizeof(TestShaderBufferData) * 4];

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
	GeoMipmapDemo chapter;

	if (!chapter.Create(SCR_WIDTH, SCR_HEIGHT))
		return -1;

	chapter.Start();

	chapter.Destroy();

	return 0;
}