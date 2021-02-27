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
		Patch(std::vector<T>& vertices, unsigned int width_, unsigned int height_, bool coarseLeft_, bool coarseRight_, bool coarseTop_, bool coarseBottom_)
		{
			width = width_;
			height = height_;
			baseVertexIndex = vertices.size();

			for (int y = 0; y < height; y += 2)
			{
				for (int x = 0; x < width; x += 2)
				{
					bool lodLeft = coarseLeft_ && (x == 0);
					bool lodRight = coarseRight_ && (x == width - 2);
					bool lodTop = coarseTop_ && (y == 0);
					bool lodBottom = coarseBottom_ && (y == height - 2);

					AddBigQuad(vertices, x, y, lodLeft, lodRight, lodTop, lodBottom);
				}
			}

			vertexCount = vertices.size() - baseVertexIndex;
		}

		~Patch()
		{
		}

		unsigned int GetWidth() const
		{
			return width;
		}

		unsigned int GetHeight() const
		{
			return height;
		}

		unsigned int GetBaseVertexIndex() const
		{
			return baseVertexIndex;
		}

		unsigned int GetVertexCount() const
		{
			return vertexCount;
		}
	private:
		void AddBigQuad(std::vector<T>& vertices_, unsigned int x_, unsigned int y_, bool L, bool R, bool T, bool B)
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


			///////////////////////////////////////////////////////////////////////
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
					AddTriangleSouthWest(vertices_, x_ + 0, y_ + 0);
				}
			}
			else
			{
				if (L)
				{
					AddTriangleNorthEast(vertices_, x_ + 0, y_ + 0);
				}
				else
				{
					AddTriangleNorthWest(vertices_, x_ + 0, y_ + 0);
					AddTriangleSouthEast(vertices_, x_ + 0, y_ + 0);
				}
			}

			///////////////////////////////////////////////////////////////////////
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
					AddTriangleSouthEast(vertices_, x_ + 1, y_ + 0);
				}
			}
			else
			{
				if (R)
				{
					AddTriangleNorthWest(vertices_, x_ + 1, y_ + 0);
				}
				else
				{
					AddTriangleNorthEast(vertices_, x_ + 1, y_ + 0);
					AddTriangleSouthWest(vertices_, x_ + 1, y_ + 0);
				}
			}

			///////////////////////////////////////////////////////////////////////
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
					AddTriangleNorthWest(vertices_, x_ + 0, y_ + 1);
				}
			}
			else
			{
				if (L)
				{
					AddTriangleSouthEast(vertices_, x_ + 0, y_ + 1);
				}
				else
				{
					AddTriangleNorthEast(vertices_, x_ + 0, y_ + 1);
					AddTriangleSouthWest(vertices_, x_ + 0, y_ + 1);
				}
			}

			///////////////////////////////////////////////////////////////////////
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
					AddTriangleNorthEast(vertices_, x_ + 1, y_ + 1);
				}
			}
			else
			{
				if (R)
				{
					AddTriangleSouthWest(vertices_, x_ + 1, y_ + 1);
				}
				else
				{
					AddTriangleSouthEast(vertices_, x_ + 1, y_ + 1);
					AddTriangleNorthWest(vertices_, x_ + 1, y_ + 1);
				}
			}

			///////////////////////////////////////////////////////////////////////
			if (T)
			{
				AddTriangleTop(vertices_, x_ + 0, y_ + 0);
			}

			if (B)
			{
				AddTriangleBottom(vertices_, x_ + 0, y_ + 0);
			}

			if (L)
			{
				AddTriangleLeft(vertices_, x_ + 0, y_ + 0);
			}

			if (R)
			{
				AddTriangleRight(vertices_, x_ + 0, y_ + 0);
			}
		}

		void AddTriangleNorthWest(std::vector<T>& vertices_, unsigned int x_, unsigned int y_)
		{
			AddVertex(vertices_, x_ + 0, y_ + 0);
			AddVertex(vertices_, x_ + 0, y_ + 1);
			AddVertex(vertices_, x_ + 1, y_ + 0);
		}

		void AddTriangleNorthEast(std::vector<T>& vertices_, unsigned int x_, unsigned int y_)
		{
			AddVertex(vertices_, x_ + 1, y_ + 1);
			AddVertex(vertices_, x_ + 1, y_ + 0);
			AddVertex(vertices_, x_ + 0, y_ + 0);
		}

		void AddTriangleSouthEast(std::vector<T>& vertices_, unsigned int x_, unsigned int y_)
		{
			AddVertex(vertices_, x_ + 0, y_ + 1);
			AddVertex(vertices_, x_ + 1, y_ + 1);
			AddVertex(vertices_, x_ + 1, y_ + 0);
		}

		void AddTriangleSouthWest(std::vector<T>& vertices_, unsigned int x_, unsigned int y_)
		{
			AddVertex(vertices_, x_ + 0, y_ + 1);
			AddVertex(vertices_, x_ + 1, y_ + 1);
			AddVertex(vertices_, x_ + 0, y_ + 0);
		}

		void AddTriangleLeft(std::vector<T>& vertices_, unsigned int x_, unsigned int y_)
		{
			AddVertex(vertices_, x_ + 0, y_ + 0);
			AddVertex(vertices_, x_ + 0, y_ + 2);
			AddVertex(vertices_, x_ + 1, y_ + 1);
		}

		void AddTriangleRight(std::vector<T>& vertices_, unsigned int x_, unsigned int y_)
		{
			AddVertex(vertices_, x_ + 2, y_ + 0);
			AddVertex(vertices_, x_ + 1, y_ + 1);
			AddVertex(vertices_, x_ + 2, y_ + 2);
		}

		void AddTriangleTop(std::vector<T>& vertices_, unsigned int x_, unsigned int y_)
		{
			AddVertex(vertices_, x_ + 0, y_ + 0);
			AddVertex(vertices_, x_ + 1, y_ + 1);
			AddVertex(vertices_, x_ + 2, y_ + 0);
		}

		void AddTriangleBottom(std::vector<T>& vertices_, unsigned int x_, unsigned int y_)
		{
			AddVertex(vertices_, x_ + 0, y_ + 2);
			AddVertex(vertices_, x_ + 2, y_ + 2);
			AddVertex(vertices_, x_ + 1, y_ + 1);
		}

		void AddVertex(std::vector<T>& vertices_, unsigned int x_, unsigned int y_)
		{
			vertices_.push_back(Vector2(x_, y_));
		}

		unsigned int width;
		unsigned int height;
		unsigned int baseVertexIndex;
		unsigned int vertexCount;
	};

	class Level
	{
	public:
		Level(std::vector<T>& vertices, unsigned int width_, unsigned int height_)
		{
			// 0
			AddPatch(vertices, width_, height_, false, false, false, false);

			// 1
			AddPatch(vertices, width_, height_, true, false, false, false);
			AddPatch(vertices, width_, height_, false, true, false, false);
			AddPatch(vertices, width_, height_, false, false, true, false);
			AddPatch(vertices, width_, height_, false, false, false, true);

			// 2
			AddPatch(vertices, width_, height_, true, true, false, false);
			AddPatch(vertices, width_, height_, true, false, true, false); //o
			AddPatch(vertices, width_, height_, true, false, false, true);
			AddPatch(vertices, width_, height_, false, true, true, false);
			AddPatch(vertices, width_, height_, false, true, false, true); //o
			AddPatch(vertices, width_, height_, false, false, true, true);

			// 3
			AddPatch(vertices, width_, height_, false, true, true, true);
			AddPatch(vertices, width_, height_, true, false, true, true);
			AddPatch(vertices, width_, height_, true, true, false, true);
			AddPatch(vertices, width_, height_, true, true, true, false);

			//4
			AddPatch(vertices, width_, height_, true, true, true, true);
		}

		~Level()
		{
		}

		unsigned int GetPatchCount() const
		{
			return patches.size();
		}

		const Patch& GetPatch(int i) const
		{
			return patches[i];
		}
	private:
		void AddPatch(std::vector<T>& vertices, unsigned int width_, unsigned int height_, bool coarseLeft_, bool coarseRight_, bool coarseTop_, bool coarseBottom_)
		{
			patches.push_back(Patch(vertices, width_, height_, coarseLeft_, coarseRight_, coarseTop_, coarseBottom_));
		}

		std::vector<Patch> patches;
	};
public:
	GeoMipmap()
	{
	}

	~GeoMipmap()
	{
	}

	bool Create(unsigned int width_, unsigned int height_)
	{
		while (width_ >= 2)
		{
			AddLevel(width_, height_);

			width_ >>= 1;
			height_ >>= 1;
		}

		return true;
	}

	unsigned int GetLevelsCount() const
	{
		return levels.size();
	}

	const Level& GetLevel(int i) const
	{
		return levels[i];
	}

	unsigned int GetVerticesCount() const
	{
		return vertices.size();
	}

	const std::vector<T>& GetVertices() const
	{
		return vertices;
	}
private:
	void AddLevel(unsigned int width_, unsigned int height_)
	{
		levels.push_back(Level(vertices, width_, height_));
	}

	std::vector<Level> levels;
	std::vector<T> vertices;
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
	class TransformData
	{
	public:
		Matrix4 viewTransform;
		Matrix4 projTransform;
	};

	GeoMipmapDemo()
	: FrameWork("GeoMipmapDemo")
	{
	}

	virtual ~GeoMipmapDemo()
	{
	}

	virtual bool OnCreate() override
	{
		if (!geoMipmap.Create(64, 64))
		{
			return false;
		}

		////////////////////////////////////////////////////////////
		bool success = primitives
			.Begin()
			.FillVertices(0, 2, VertexAttribute::DataType::FLOAT, false, 0, 0, &geoMipmap.GetVertices()[0], geoMipmap.GetVerticesCount())
			.End();
		if (!success)
		{
			return false;
		}

		////////////////////////////////////////////////////////////
		if (!heightMap.Create("heightMap.bmp", false))
		{
			return false;
		}
		heightMap.SetMinFilter(Texture::MinFilter::Nearest);
		heightMap.SetMagFilter(Texture::MagFilter::Nearest);
		heightMap.SetWarpS(Texture::Wrap::Clamp);
		heightMap.SetWarpR(Texture::Wrap::Clamp);
		heightMap.SetWarpT(Texture::Wrap::Clamp);

		////////////////////////////////////////////////////////////
		if (!heightMapShaderProgram.Create("TerrainVS.glsl", "TerrainPS.glsl"))
		{
			return false;
		}

		/*
		if (!shaderStorageBlockBuffer
			.Begin(Buffer::Type::SHADER_STORAGE_BUFFER, Buffer::Usage::STATIC_DRAW)
			.Fill(&geoMipmaps.GetIndices()[0], geoMipmaps.GetIndicesCount() * sizeof(geoMipmaps.GetIndices()[0]))
			.End()
			)
		{
			return false;
		}
		//shaderStorageBlockBuffer.BindShaderStorage(heightMapShaderProgram, 0);
		heightMapShaderProgram.BindShaderStorageBuffer(shaderStorageBlockBuffer, "HeightMap", 0);
		*/

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

		heightMapShaderProgram.BindUniformBlock(uniformBlockBuffer, "TransformData", 0);
		// heightMapShaderProgram.BindUniformBlock(uniformBlockBuffer, 0);

		return true;
	}

	virtual bool OnUpdate() override
	{
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
		renderStates.polygonModeState.mode = PolygonModeState::Mode::LINE;

		renderStates.depthTestState.depthTestEnabled = true;
		renderStates.depthTestState.depthWriteEnabled = true;
		renderStates.depthTestState.func = DepthTestState::Func::LEQUAL;
		renderStates.Apply();

		///////////////////////////////////////////////////////
		heightMap.Bind(0);

		///////////////////////////////////////////////////////
		static float t = 0.0;
		t += 0.001f;

		Matrix4 cameraTransform;
		cameraTransform.SetLookAt(Vector3(50 + 300 * Math::Sin(t), 50, 50), Vector3(300 * Math::Sin(t), 0, 50), Vector3(0, 1, 0));
		camera.SetWorldTransform(cameraTransform);

		Matrix4 projectionTransform;
		projectionTransform.SetPerspectiveFov(90.0f, float(SCR_WIDTH) / SCR_HEIGHT, 1.0f, 1000.0f);
		camera.SetProjectionTransform(projectionTransform);

		ColorRGBA colors[] =
		{
			ColorRGBA(0.2, 0.2, 0.5, 1.0),
			ColorRGBA(0.2, 0.5, 0.2, 1.0),
			ColorRGBA(0.2, 0.5, 0.5, 1.0),
			ColorRGBA(0.5, 0.2, 0.2, 1.0),
			ColorRGBA(0.5, 0.2, 0.5, 1.0),
			ColorRGBA(0.5, 0.5, 0.2, 1.0),
			ColorRGBA(0.5, 0.5, 0.5, 1.0),
			ColorRGBA(0.2, 0.2, 1.0, 1.0),
			ColorRGBA(0.2, 1.0, 0.2, 1.0),
			ColorRGBA(0.2, 1.0, 1.0, 1.0),
			ColorRGBA(1.0, 0.2, 0.2, 1.0),
			ColorRGBA(1.0, 0.2, 1.0, 1.0),
			ColorRGBA(1.0, 1.0, 0.2, 1.0),
			ColorRGBA(1.0, 1.0, 1.0, 1.0)
		};
		///////////////////////////////////////////////////////
		primitives.Bind();
		for (int i = 0; i <geoMipmap.GetLevelsCount(); i++)
		{
			const GeoMipmap<Vector2>::Level& level = geoMipmap.GetLevel(i);
			for (int j = 0; j < level.GetPatchCount(); j++)
			{
				const GeoMipmap<Vector2>::Patch& patch = level.GetPatch(j);
				
				Vector3 p(i * 64, 0, j*64);
				worldTransform.SetTranslateRotXYZScale(p.X(), p.Y(), p.Z(), 0, 0, 0, 1.0);

				heightMapShaderProgram.Bind();
				heightMapShaderProgram.SetUniform1i("heightMap", 0);
				heightMapShaderProgram.SetUniform4f("colors", colors[j][0], colors[j][1], colors[j][2], colors[j][3]);

				int lod = pow(2, (geoMipmap.GetLevelsCount() - i));
				heightMapShaderProgram.SetUniform1i("lod", lod);
				heightMapShaderProgram.SetUniformMatrix4x4fv("worldTransform", 1, worldTransform);

#define USE_UNIFORM_BLOCK
#ifdef USE_UNIFORM_BLOCK
				TransformData transformData;
				transformData.viewTransform = camera.GetViewTransform().Transpose();
				transformData.projTransform = camera.GetProjectionTransform().Transpose();
				uniformBlockBuffer.Update(0, &transformData, sizeof(TransformData));
#else
				heightMapShaderProgram.SetUniformMatrix4x4fv("viewTransform", 1, camera.GetViewTransform());
				heightMapShaderProgram.SetUniformMatrix4x4fv("projTransform", 1, camera.GetProjectionTransform());
#endif

				primitives.DrawArray(Primitives::Mode::TRIANGLES, patch.GetBaseVertexIndex(), patch.GetVertexCount());
			}		
		}

		return true;
	}

	void OnDestroy() override
	{
		heightMap.Destroy();
		shaderStorageBlockBuffer.Destroy();

		uniformBlockBuffer.Destroy();
		heightMapShaderProgram.Destroy();
		renderStates.Destroy();

		primitives.Destroy();
	}
private:
	Matrix4 worldTransform;
	Camera camera;

	Texture2DFile heightMap;
	Buffer shaderStorageBlockBuffer;

	Buffer uniformBlockBuffer;
	ShaderProgram heightMapShaderProgram;
	RenderStates renderStates;

	GeoMipmap<Vector2> geoMipmap;
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