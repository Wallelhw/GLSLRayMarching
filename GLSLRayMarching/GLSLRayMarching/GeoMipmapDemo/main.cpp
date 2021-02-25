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

template<class T>
class GeoMipmap
{
public:
	class Patch
	{
	public:
		Patch(unsigned int width_, unsigned int height_, bool coarseLeft_, bool coarseRight_, bool coarseTop_, bool coarseBottom_)
		{
			width = width_;
			height = height_;

			for (int y = 0; y < height; y += 2)
			{
				for (int x = 0; x < width; x += 2)
				{
					AddBigQuad(x, y,
						coarseLeft_ && (x == 0),
						coarseRight_ && (x == width - 2),
						coarseTop_ && (y == 0),
						coarseBottom_ && (y == height - 2));
				}
			}
		}

		~Patch()
		{
		}

		const std::vector<T>& GetIndices() const
		{
			return indices;
		}

		unsigned int GetIndicesCount()
		{
			unsigned int total = 0;

			total += indices.size();

			return total;
		}
	private:

#define AddVertex(x_, y_) (indices.push_back((y_) * width + (x_)))

		void AddBigQuad(unsigned int x_, unsigned int y_, bool L, bool R, bool T, bool B)
		{
			// http://tma.main.jp/logic/index_en.html
			// TL
			// L R T B    NE NW SE SW
			// 0 x 0 x    0  1  1  0
			// 0 x 1 x    0  0  0  1
			// 1 x 0 x    1  0  0  0
			// 1 x 1 x    0  0  0  0
			// NE = L && !T
			// NW = !(L && T)
			// SE = !(L && T)
			// SW = !L && T

			// TR
			// L R T B    NE NW SE SW
			// x 0 0 x    1  0  0  1
			// x 0 1 x    0  0  1  0
			// x 1 0 x    0  1  0  0
			// x 1 1 x    0  0  0  0
			// NE = !(R && T)
			// NW = R && !T
			// SE = !R && T
			// SW = !(R && T)

			// BL
			// L R T B    NE NW SE SW
			// 0 x x 0    1  0  0  1
			// 0 x x 1    0  1  0  0
			// 1 x x 0    0  0  1  0
			// 1 x x 1    0  0  0  0
			// NE = !(L && B)
			// NW = !L && B
			// SE = L && !B
			// SW = !(L && B)

			// BR
			// L R T B    NE NW SE SW
			// x 0 x 0    0  1  1  0
			// x 0 x 1    1  0  0  0
			// x 1 x 0    0  0  0  1
			// x 1 x 1    0  0  0  0
			// NE = !R && B
			// NW = !(R && B)
			// SE = !(R && B)
			// SW = R && !B



			// NE = L && !T
			// NW = !(L && T)
			// SE = !(L && T)
			// SW = !L && T
			if (T)
			{
				if (L)
				{
				}
				else
				{
					AddTriangleSouthWest(x_ + 0, y_ + 0);
				}
			}
			else
			{
				if (L)
				{
					AddTriangleNorthEast(x_ + 0, y_ + 0);
				}
				else
				{
					AddTriangleNorthWest(x_ + 0, y_ + 0);
					AddTriangleSouthEast(x_ + 0, y_ + 0);
				}
			}

			// NE = !(R && T)
			// NW = R && !T
			// SE = !R && T
			// SW = !(R && T)
			if (T)
			{
				if (R)
				{
				}
				else
				{
					AddTriangleSouthEast(x_ + 1, y_ + 0);
				}
			}
			else
			{
				if (R)
				{
					AddTriangleNorthWest(x_ + 1, y_ + 0);
				}
				else
				{
					AddTriangleNorthEast(x_ + 1, y_ + 0);
					AddTriangleSouthWest(x_ + 1, y_ + 0);
				}
			}


			// NE = !(L && B)
			// NW = !L && B
			// SE = L && !B
			// SW = !(L && B)
			if (B)
			{
				if (L)
				{
				}
				else
				{
					AddTriangleNorthWest(x_ + 0, y_ + 1);
				}
			}
			else
			{
				if (L)
				{
					AddTriangleSouthEast(x_ + 0, y_ + 1);
				}
				else
				{
					AddTriangleNorthEast(x_ + 0, y_ + 1);
					AddTriangleSouthWest(x_ + 0, y_ + 1);
				}
			}


			// NE = !R && B
			// NW = !(R && B)
			// SE = !(R && B)
			// SW = R && !B
			if (B)
			{
				if (R)
				{
				}
				else
				{
					AddTriangleNorthEast(x_ + 1, y_ + 1);
				}
			}
			else
			{
				if (R)
				{
					AddTriangleSouthWest(x_ + 1, y_ + 1);
				}
				else
				{
					AddTriangleSouthEast(x_ + 1, y_ + 1);
					AddTriangleNorthWest(x_ + 1, y_ + 1);
				}
			}

			if (T)
			{
				AddTriangleTop(x_ + 0, y_ + 0);
			}

			if (B)
			{
				AddTriangleBottom(x_ + 0, y_ + 0);
			}

			if (L)
			{
				AddTriangleLeft(x_ + 0, y_ + 0);
			}

			if (R)
			{
				AddTriangleRight(x_ + 0, y_ + 0);
			}
		}

		void AddTriangleNorthWest(unsigned int x_, unsigned int y_)
		{
			AddVertex(x_ + 0, y_ + 0);
			AddVertex(x_ + 1, y_ + 0);
			AddVertex(x_ + 0, y_ + 1);
		}

		void AddTriangleNorthEast(unsigned int x_, unsigned int y_)
		{
			AddVertex(x_ + 1, y_ + 1);
			AddVertex(x_ + 0, y_ + 0);
			AddVertex(x_ + 1, y_ + 0);
		}

		void AddTriangleSouthEast(unsigned int x_, unsigned int y_)
		{
			AddVertex(x_ + 0, y_ + 1);
			AddVertex(x_ + 1, y_ + 0);
			AddVertex(x_ + 1, y_ + 1);
		}

		void AddTriangleSouthWest(unsigned int x_, unsigned int y_)
		{
			AddVertex(x_ + 0, y_ + 1);
			AddVertex(x_ + 1, y_ + 1);
			AddVertex(x_ + 0, y_ + 0);
		}

		void AddTriangleLeft(unsigned int x_, unsigned int y_)
		{
			AddVertex(x_ + 0, y_ + 0);
			AddVertex(x_ + 1, y_ + 1);
			AddVertex(x_ + 0, y_ + 2);
		}

		void AddTriangleRight(unsigned int x_, unsigned int y_)
		{
			AddVertex(x_ + 2, y_ + 0);
			AddVertex(x_ + 2, y_ + 2);
			AddVertex(x_ + 1, y_ + 1);
		}

		void AddTriangleTop(unsigned int x_, unsigned int y_)
		{
			AddVertex(x_ + 0, y_ + 0);
			AddVertex(x_ + 2, y_ + 0);
			AddVertex(x_ + 1, y_ + 1);
		}

		void AddTriangleBottom(unsigned int x_, unsigned int y_)
		{
			AddVertex(x_ + 0, y_ + 2);
			AddVertex(x_ + 1, y_ + 1);
			AddVertex(x_ + 2, y_ + 2);
		}

		unsigned int width;
		unsigned int height;
		std::vector<unsigned short> indices;
	};

	class Level
	{
	public:
		Level(unsigned int width_, unsigned int height_)
		{
			// 0
			AddPatch(width_, height_, false, false, false, false);

			// 1
			AddPatch(width_, height_, true, false, false, false);
			//AddPatch(width_, height_, false, true, false, false);
			//AddPatch(width_, height_, false, false, true, false);
			//AddPatch(width_, height_, false, false, false, true);

			// 2
			AddPatch(width_, height_, true, true, false, false);
			AddPatch(width_, height_, true, false, true, false); //o
			//AddPatch(width_, height_, true, false, false, true);
			//AddPatch(width_, height_, false, true, true, false);
			//AddPatch(width_, height_, false, true, false, true); //o
			//AddPatch(width_, height_, false, false, true, true);

			// 3
			AddPatch(width_, height_, false, true, true, true);
			//AddPatch(width_, height_, true, false, true, true);
			//AddPatch(width_, height_, true, true, false, true);
			//AddPatch(width_, height_, true, true, true, false);

			//4
			AddPatch(width_, height_, true, true, true, true);
		}

		~Level()
		{
		}

		unsigned int GetPatchesCount()
		{
			return patches.size();
		}

		const Patch& GetPatch(int i) const
		{
			return patches[i];
		}

		unsigned int GetIndicesCount()
		{
			unsigned int total = 0;

			for (auto& patch : patches)
			{
				total += patch.GetIndicesCount();
			}

			return total;
		}
	private:
		void AddPatch(unsigned int width_, unsigned int height_, bool coarseLeft_, bool coarseRight_, bool coarseTop_, bool coarseBottom_)
		{
			patches.push_back(Patch(width_, height_, coarseLeft_, coarseRight_, coarseTop_, coarseBottom_));
		}

		std::vector<Patch> patches;
	};
public:
	GeoMipmap(unsigned int width_, unsigned int height_)
	{
		while (width_ >= 2)
		{
			AddLevel(width_, height_);

			width_ >>= 1;
			height_ >>= 1;
		}
	}

	~GeoMipmap()
	{
	}

	unsigned int GetLevelsCount()
	{
		return levels.size();
	}

	const Level& GetLevel(int i) const
	{
		return levels[i];
	}

	unsigned int GetIndicesCount()
	{
		unsigned int total = 0;

		for (auto& level : levels)
		{
			total += level.GetIndicesCount();
		}

		return total;
	}
private:
	void AddLevel(unsigned int width_, unsigned int height_)
	{
		levels.push_back(Level(width_, height_));
	}

	std::vector<Level> levels;
};




class Terrain
{
public:
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

//////////////////////////////////////////////////////////////////////
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
		GeoMipmap<unsigned short> geoMipmaps(32, 32);

		Platform::Debug("%d\n", geoMipmaps.GetIndicesCount());

		////////////////////////////////////////////////////////////
		float vertices[] =
		{
			0.0,
			0.0,
			1.0,

			1.0,
			0.0,
			1.0,
		};

		bool success = primitives
			.Begin()
			.FillVertices(0, 1, VertexAttribute::DataType::FLOAT, false, 0, 0, &vertices[0], sizeof(vertices) / sizeof(vertices[0]))
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