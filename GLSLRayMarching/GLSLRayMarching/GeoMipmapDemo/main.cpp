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
#include "AABB3.h"

#define SCR_WIDTH (800*2)
#define SCR_HEIGHT (400*2)


template<class T>
class GeoMipmap
{
public:
	#define MAX_LOD 6
	#define PATCH_SIZE (64)

	class Patch
	{
	public:
		enum class Edge
		{
			Left = 8,
			Right = 4,
			Top = 2,
			Bottom = 1
		};

		Patch(std::vector<T>& vertices, unsigned int size_, unsigned int stride_, unsigned int edge_)
		{
			size = size_;
			stride = stride_;

			baseVertexIndex = vertices.size();

			for (int y = 0; y < size; y += stride * 2)
			{
				for (int x = 0; x < size; x += stride * 2)
				{
					if ((x == size - stride * 2))
						x = x;
					if ((y == size - stride * 2))
						y = y;

					bool lodLeft = ((edge_ & (int)Edge::Left) && (x == 0));
					bool lodRight = ((edge_ & (int)Edge::Right) && (x == size - stride * 2));
					bool lodTop = ((edge_ & (int)Edge::Top) && (y == 0));
					bool lodBottom = ((edge_ & (int)Edge::Bottom) && (y == size - stride * 2));
					AddBigQuad(vertices, x, y, lodLeft, lodRight, lodTop, lodBottom);
				}
			}

			vertexCount = vertices.size() - baseVertexIndex;
		}

		~Patch()
		{
		}

		Patch(const Patch& other)
		{
			size = other.size;
			stride = other.stride;
			baseVertexIndex = other.baseVertexIndex;
			vertexCount = other.vertexCount;
		}

		Patch& operator = (const Patch& other)
		{
			size = other.size;
			stride = other.stride;
			baseVertexIndex = other.baseVertexIndex;
			vertexCount = other.vertexCount;

			return *this;
		}

		unsigned int GetSize() const
		{
			return size;
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
#define X0 ((x_))
#define X1 ((x_) + stride)
#define X2 ((x_) + stride + stride)
#define Y0 ((y_))
#define Y1 ((y_) + stride)
#define Y2 ((y_) + stride + stride)
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
					AddTriangleSouthWest(vertices_, X0, Y0);
				}
			}
			else
			{
				if (L)
				{
					AddTriangleNorthEast(vertices_, X0, Y0);
				}
				else
				{
					AddTriangleNorthWest(vertices_, X0, Y0);
					AddTriangleSouthEast(vertices_, X0, Y0);
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
					AddTriangleSouthEast(vertices_, X1, Y0);
				}
			}
			else
			{
				if (R)
				{
					AddTriangleNorthWest(vertices_, X1, Y0);
				}
				else
				{
					AddTriangleNorthEast(vertices_, X1, Y0);
					AddTriangleSouthWest(vertices_, X1, Y0);
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
					AddTriangleNorthWest(vertices_, X0, Y1);
				}
			}
			else
			{
				if (L)
				{
					AddTriangleSouthEast(vertices_, X0, Y1);
				}
				else
				{
					AddTriangleNorthEast(vertices_, X0, Y1);
					AddTriangleSouthWest(vertices_, X0, Y1);
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
					AddTriangleNorthEast(vertices_, X1, Y1);
				}
			}
			else
			{
				if (R)
				{
					AddTriangleSouthWest(vertices_, X1, Y1);
				}
				else
				{
					AddTriangleSouthEast(vertices_, X1, Y1);
					AddTriangleNorthWest(vertices_, X1, Y1);
				}
			}

			///////////////////////////////////////////////////////////////////////
			if (T)
			{
				AddTriangleTop(vertices_, X0, Y0);
			}

			if (B)
			{
				AddTriangleBottom(vertices_, X0, Y0);
			}

			if (L)
			{
				AddTriangleLeft(vertices_, X0, Y0);
			}

			if (R)
			{
				AddTriangleRight(vertices_, X0, Y0);
			}
		}

		void AddTriangleNorthWest(std::vector<T>& vertices_, unsigned int x_, unsigned int y_)
		{
			AddVertex(vertices_, X0, Y0);
			AddVertex(vertices_, X0, Y1);
			AddVertex(vertices_, X1, Y0);
		}

		void AddTriangleNorthEast(std::vector<T>& vertices_, unsigned int x_, unsigned int y_)
		{
			AddVertex(vertices_, X1, Y1);
			AddVertex(vertices_, X1, Y0);
			AddVertex(vertices_, X0, Y0);
		}

		void AddTriangleSouthEast(std::vector<T>& vertices_, unsigned int x_, unsigned int y_)
		{
			AddVertex(vertices_, X0, Y1);
			AddVertex(vertices_, X1, Y1);
			AddVertex(vertices_, X1, Y0);
		}

		void AddTriangleSouthWest(std::vector<T>& vertices_, unsigned int x_, unsigned int y_)
		{
			AddVertex(vertices_, X0, Y1);
			AddVertex(vertices_, X1, Y1);
			AddVertex(vertices_, X0, Y0);
		}

		void AddTriangleLeft(std::vector<T>& vertices_, unsigned int x_, unsigned int y_)
		{
			AddVertex(vertices_, X0, Y0);
			AddVertex(vertices_, X0, Y2);
			AddVertex(vertices_, X1, Y1);
		}

		void AddTriangleRight(std::vector<T>& vertices_, unsigned int x_, unsigned int y_)
		{
			AddVertex(vertices_, X2, Y0);
			AddVertex(vertices_, X1, Y1);
			AddVertex(vertices_, X2, Y2);
		}

		void AddTriangleTop(std::vector<T>& vertices_, unsigned int x_, unsigned int y_)
		{
			AddVertex(vertices_, X0, Y0);
			AddVertex(vertices_, X1, Y1);
			AddVertex(vertices_, X2, Y0);
		}

		void AddTriangleBottom(std::vector<T>& vertices_, unsigned int x_, unsigned int y_)
		{
			AddVertex(vertices_, X0, Y2);
			AddVertex(vertices_, X2, Y2);
			AddVertex(vertices_, X1, Y1);
		}

		void AddVertex(std::vector<T>& vertices_, unsigned int x_, unsigned int y_)
		{
			vertices_.push_back(Vector2(x_, y_));
		}

		unsigned int size;
		unsigned int stride;
		unsigned int baseVertexIndex;
		unsigned int vertexCount;
	};

	class Level
	{
	public:
		Level(std::vector<T>& vertices, unsigned int size_, unsigned int stride_)
		{
			AddPatch(vertices, size_, stride_, GetPatchIndex(false, false, false, false));
			AddPatch(vertices, size_, stride_, GetPatchIndex(false, false, false, true));
			AddPatch(vertices, size_, stride_, GetPatchIndex(false, false, true, false));
			AddPatch(vertices, size_, stride_, GetPatchIndex(false, false, true, true));
			AddPatch(vertices, size_, stride_, GetPatchIndex(false, true, false, false));
			AddPatch(vertices, size_, stride_, GetPatchIndex(false, true, false, true));
			AddPatch(vertices, size_, stride_, GetPatchIndex(false, true, true, false));
			AddPatch(vertices, size_, stride_, GetPatchIndex(false, true, true, true));

			AddPatch(vertices, size_, stride_, GetPatchIndex(true, false, false, false));
			AddPatch(vertices, size_, stride_, GetPatchIndex(true, false, false, true));
			AddPatch(vertices, size_, stride_, GetPatchIndex(true, false, true, false));
			AddPatch(vertices, size_, stride_, GetPatchIndex(true, false, true, true));
			AddPatch(vertices, size_, stride_, GetPatchIndex(true, true, false, false));
			AddPatch(vertices, size_, stride_, GetPatchIndex(true, true, false, true));
			AddPatch(vertices, size_, stride_, GetPatchIndex(true, true, true, false));
			AddPatch(vertices, size_, stride_, GetPatchIndex(true, true, true, true));
		}

		~Level()
		{
		}

		Level(const Level& other)
		{
			patches = other.patches;
		}

		Level& operator = (const Level& other)
		{
			patches = other.patches;

			return *this;
		}

		unsigned int GetPatchCount() const
		{
			return patches.size();
		}

		static unsigned int GetPatchIndex(bool coarseLeft_, bool coarseRight_, bool coarseTop_, bool coarseBottom_)
		{
			unsigned int result = 0;
			if (coarseLeft_)
				result |= (int)Patch::Edge::Left;

			if (coarseRight_)
				result |= (int)Patch::Edge::Right;

			if (coarseTop_)
				result |= (int)Patch::Edge::Top;

			if (coarseBottom_)
				result |= (int)Patch::Edge::Bottom;

			return result;
		}

		const Patch& GetPatch(int i) const
		{
			return patches[i];
		}
	private:
		void AddPatch(std::vector<T>& vertices, unsigned int size_, unsigned int stride_, unsigned int edge_)
		{
			patches.push_back(Patch(vertices, size_, stride_, edge_));
		}

		std::vector<Patch> patches;
	};
public:
	class TransformData
	{
	public:
		Matrix4 viewTransform;
		Matrix4 projTransform;
	};

	class RenderInfo
	{
	public:
		Vector3 offset;
		unsigned int lodLevel;
		unsigned int patchID;
	};

	GeoMipmap()
	{
	}

	~GeoMipmap()
	{
	}

	bool Create(unsigned int size_)
	{
		unsigned int mipLevelCount = log(size_) / log(2);

		std::vector<Vector2> vertices;
		for (unsigned int mipLevel_ = 0; mipLevel_ < mipLevelCount; mipLevel_++)
		{
			AddLevel(vertices, size_, mipLevel_);
		}

		////////////////////////////////////////////////////////////
		bool success = primitives
			.Begin()
			.FillVertices(0, 2, VertexAttribute::DataType::FLOAT, false, 0, 0, &vertices[0], vertices.size())
			.End();
		if (!success)
		{
			return false;
		}

		////////////////////////////////////////////////////////////
		if (!heightMap.Create("heightMap.png", false))
		{
			return false;
		}
		heightMap.SetMinFilter(Texture::MinFilter::Nearest);
		heightMap.SetMagFilter(Texture::MagFilter::Nearest);
		heightMap.SetWarpS(Texture::Wrap::Clamp);
		heightMap.SetWarpR(Texture::Wrap::Clamp);
		heightMap.SetWarpT(Texture::Wrap::Clamp);

		////////////////////////////////////////////////////////////
		if (!shaderProgram.Create("GeoMipMapVS.glsl", "GeoMipMapPS.glsl"))
		{
			return false;
		}

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

		shaderProgram.BindUniformBlock(uniformBlockBuffer, "TransformData", 0);

		/*
		if (!shaderStorageBlockBuffer
			.Begin(Buffer::Type::SHADER_STORAGE_BUFFER, Buffer::Usage::STATIC_DRAW)
			.Fill(&geoMipmaps.GetIndices()[0], geoMipmaps.GetIndicesCount() * sizeof(geoMipmaps.GetIndices()[0]))
			.End()
			)
		{
			return false;
		}
		//shaderStorageBlockBuffer.BindShaderStorage(shaderProgram, 0);
		shaderProgram.BindShaderStorageBuffer(shaderStorageBlockBuffer, "HeightMap", 0);
		*/
		// shaderProgram.BindUniformBlock(uniformBlockBuffer, 0);

		return true;
	}

	unsigned int EstimateLOD(Camera& camera, Vector3 worldpos)
	{
		Vector3 viewpos = camera.GetViewTransform() * Vector3(worldpos.X(), worldpos.Y(), worldpos.Z());
		viewpos.Y() = 0;
		float dist = viewpos.Length();

#define SCALE 10
		float maxdist = 10;
		for (int i = 0; i < MAX_LOD; i++)
		{
			if (dist < maxdist * SCALE)
			{
				return i;
			}
			maxdist *= 2;
		}

		return MAX_LOD - 1;
	}

	bool IntersectAABBFrustum(Frustum& f, AABB3& aabb)
	{
		float m, n; 
		int i;
		int result = true;

		Vector3 bm = aabb.Center();
		Vector3 bd = aabb.Extent();

		for (i = 0; i < f.GetPlaneCount(); i++)
		{
			Plane3& p = f.GetPlane(i);

			m = (bm.X() * p.Normal().X()) + (bm.Y() * p.Normal().Y()) + (bm.Z() * p.Normal().Z()) + p.Constant();
			n = (bd.X() * Math::FAbs(p.Normal().X())) + (bd.Y() * Math::FAbs(p.Normal().Y())) + (bm.Z() * Math::FAbs(p.Normal().Z()));

			if (m + n < 0)
				return false;
			if (m - n < 0)
				result = true;
		} 

		return result;
	}

	void CalculatePatches(std::vector<RenderInfo>& terrainRenderInfos_, Camera& camera)
	{
		Frustum f = camera.GetFrustum();
		terrainRenderInfos_.resize(heightMap.GetWidth() / PATCH_SIZE * heightMap.GetHeight() / PATCH_SIZE);

		for (int z = 0; z < heightMap.GetHeight() / PATCH_SIZE; z++)
		{
			for (int x = 0; x < heightMap.GetWidth() / PATCH_SIZE; x++)
			{
				int idx = z * (heightMap.GetWidth() / PATCH_SIZE) + x;
				RenderInfo& info = terrainRenderInfos_[idx];

				info.offset = Vector3(x * PATCH_SIZE, 0, z * PATCH_SIZE);
				info.lodLevel = EstimateLOD(camera, info.offset + Vector3(PATCH_SIZE / 2, 0, PATCH_SIZE / 2));
				info.patchID = 0;
			}
		}

		for (int z = 0; z < heightMap.GetHeight() / PATCH_SIZE; z++)
		{
			for (int x = 0; x < heightMap.GetWidth() / PATCH_SIZE; x++)
			{
				int idxC = (z + 0) * (heightMap.GetWidth() / PATCH_SIZE) + (x + 0);
				int idxL = (z + 0) * (heightMap.GetWidth() / PATCH_SIZE) + (x - 1);
				int idxR = (z + 0) * (heightMap.GetWidth() / PATCH_SIZE) + (x + 1);
				int idxT = (z - 1) * (heightMap.GetWidth() / PATCH_SIZE) + (x + 0);
				int idxB = (z + 1) * (heightMap.GetWidth() / PATCH_SIZE) + (x + 0);

				bool coarseL = ((x - 1) >= 0) ? (terrainRenderInfos_[idxC].lodLevel < terrainRenderInfos_[idxL].lodLevel) : false;
				bool coarseR = ((x + 1) <= (heightMap.GetWidth() / PATCH_SIZE) - 1) ? (terrainRenderInfos_[idxC].lodLevel < terrainRenderInfos_[idxR].lodLevel) : false;
				bool coarseT = ((z - 1) >= 0) ? (terrainRenderInfos_[idxC].lodLevel < terrainRenderInfos_[idxT].lodLevel) : false;
				bool coarseB = ((z + 1) <= (heightMap.GetHeight() / PATCH_SIZE) - 1) ? (terrainRenderInfos_[idxC].lodLevel < terrainRenderInfos_[idxB].lodLevel) : false;

				terrainRenderInfos_[idxC].patchID = GeoMipmap<IVector2>::Level::GetPatchIndex(coarseL, coarseR, coarseT, coarseB);
			}
		}
	}

	void Update(Camera& camera)
	{
		//////////////////////////////////////////////////////
		// RS
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

		renderStates.cullFaceState.enabled = true;
		renderStates.cullFaceState.mode = CullFaceState::Mode::BACK;

		renderStates.depthTestState.depthTestEnabled = true;
		renderStates.depthTestState.depthWriteEnabled = true;
		renderStates.depthTestState.func = DepthTestState::Func::LEQUAL;
		renderStates.Apply();

		///////////////////////////////////////////////////////
		// TexMap
		heightMap.Bind(0);

		///////////////////////////////////////////////////////
		primitives.Bind();

		// frustum culling
		// area = b * sqrt(b*b + h*h)
		std::vector<RenderInfo> terrainRenderInfos;
		CalculatePatches(terrainRenderInfos, camera);

		shaderProgram.Bind();
		shaderProgram.SetUniform1i("heightMap", 0);
		Matrix4 worldTransform;
		worldTransform.SetTranslateRotXYZScale(0, 0, 0, 0, 0, 0, 1.0);
		shaderProgram.SetUniformMatrix4x4fv("worldTransform", 1, worldTransform);
#define USE_UNIFORM_BLOCK
#ifdef USE_UNIFORM_BLOCK
		TransformData transformData;
		transformData.viewTransform = camera.GetViewTransform().Transpose();
		transformData.projTransform = camera.GetProjectionTransform().Transpose();
		uniformBlockBuffer.Update(0, &transformData, sizeof(TransformData));
#else
		shaderProgram.SetUniformMatrix4x4fv("viewTransform", 1, camera.GetViewTransform());
		shaderProgram.SetUniformMatrix4x4fv("projTransform", 1, camera.GetProjectionTransform());
#endif

		renderStates.polygonModeState.face = PolygonModeState::Face::FRONT_AND_BACK;
		renderStates.polygonModeState.mode = PolygonModeState::Mode::LINE;
		renderStates.Apply();
		for (int i = 0; i < terrainRenderInfos.size(); i++)
		{
			const GeoMipmap<Vector2>::Patch& patch = GetLevel(terrainRenderInfos[i].lodLevel).GetPatch(terrainRenderInfos[i].patchID);
			float c = ((float)(MAX_LOD - terrainRenderInfos[i].lodLevel)) / MAX_LOD;
			shaderProgram.SetUniform4f("colors", c, c, c, 1.0f);
			shaderProgram.SetUniform2i("offset", terrainRenderInfos[i].offset.X(), terrainRenderInfos[i].offset.Z());

			primitives.DrawArray(Primitives::Mode::TRIANGLES, patch.GetBaseVertexIndex(), patch.GetVertexCount());
		}
	}

	void Destroy()
	{
		heightMap.Destroy();
		shaderStorageBlockBuffer.Destroy();

		uniformBlockBuffer.Destroy();
		shaderProgram.Destroy();
		renderStates.Destroy();

		primitives.Destroy();
	}

	unsigned int GetLevelsCount() const
	{
		return levels.size();
	}

	const Level& GetLevel(int i) const
	{
		return levels[i];
	}
private:
	void AddLevel(std::vector<T>& vertices_, unsigned int size_, unsigned int mipLevel_)
	{
		unsigned int stride = pow(2, mipLevel_);
		levels.push_back(Level(vertices_, size_, stride));
	}

	Texture2DFile heightMap;
	Buffer shaderStorageBlockBuffer;

	std::vector<Level> levels;

	RenderStates renderStates;
	ShaderProgram shaderProgram;
	Buffer uniformBlockBuffer;
	Primitives primitives;
};

class MorphQuad
{
public:
	MorphQuad()
	{
	}

	~MorphQuad()
	{
	}

	bool Create()
	{
		std::vector<Vector4> vertices;
		//for (unsigned int mipLevel_ = 0; mipLevel_ < mipLevelCount; mipLevel_++)
		//{
			//AddLevel(vertices, size_, mipLevel_);
		//}

		////////////////////////////////////////////////////////////
		bool success = primitives
			.Begin()
			.FillVertices(0, 4, VertexAttribute::DataType::FLOAT, false, 0, 0, &vertices[0], vertices.size())
			.End();
		if (!success)
		{
			return false;
		}

		////////////////////////////////////////////////////////////
		if (!heightMap.Create("heightMap.hdr", false))
		{
			return false;
		}
		heightMap.SetMinFilter(Texture::MinFilter::Nearest);
		heightMap.SetMagFilter(Texture::MagFilter::Nearest);
		heightMap.SetWarpS(Texture::Wrap::Clamp);
		heightMap.SetWarpR(Texture::Wrap::Clamp);
		heightMap.SetWarpT(Texture::Wrap::Clamp);

		////////////////////////////////////////////////////////////
		if (!shaderProgram.Create("MorphQuadVS.glsl", "MorphQuadPS.glsl"))
		{
			return false;
		}

		return true;
	}

	void Update()
	{
	}

	void Destroy()
	{
	}
private:
	Texture2DFile heightMap;

	RenderStates renderStates;
	ShaderProgram shaderProgram;
	Primitives primitives;
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

	virtual bool OnCreate() override
	{
		pos = Vector3(100, 100, 100);
		dir = Vector3(0.57735, -0.57735, 0.57735);

		if (!geoMipmap.Create(PATCH_SIZE))
			return false;

		return true;
	}

	virtual bool OnUpdate() override
	{
		UpdateCamera();

		geoMipmap.Update(camera);

		return true;
	}

	void OnDestroy() override
	{
		geoMipmap.Destroy();
	}

	void UpdateCamera()
	{
		static float phi = 0.7343;
		static float theta = -0.5733;

		Vector4 mouse = GetMouse();
		phi = (mouse.X() - (SCR_WIDTH / 2)) / (SCR_WIDTH / 2) * 180 * Math::Degree2Radian;
		if (phi > 180 * Math::Degree2Radian)
			phi = 180 * Math::Degree2Radian;
		if (phi < -180 * Math::Degree2Radian)
			phi = -180 * Math::Degree2Radian;

		theta = (mouse.Y() - (SCR_HEIGHT / 2)) / (SCR_HEIGHT / 2) * 90 * Math::Degree2Radian;
		if (theta > 70 * Math::Degree2Radian)
			theta = 70 * Math::Degree2Radian;
		if (theta < -70 * Math::Degree2Radian)
			theta = -70 * Math::Degree2Radian;

		dir = Vector3(Math::Cos(theta) * Math::Cos(phi), Math::Sin(theta), Math::Cos(theta) * Math::Sin(phi)); dir.Normalize();
		Vector3 xAxis = dir.Cross(Vector3::UnitY); xAxis.Normalize();
		
		Platform::Debug("%f %f\n", theta, phi);

		if (IsKeyPressed('W'))
		{
			pos += dir;
		}

		if (IsKeyPressed('S'))
		{
			pos -= dir;
		}

		if (IsKeyPressed('A'))
		{
			pos -= xAxis;
		}

		if (IsKeyPressed('D'))
		{
			pos += xAxis;
		}
		Vector3 obj = pos + dir;

		Matrix4 cameraTransform;
		cameraTransform.SetLookAt(pos, obj, Vector3::UnitY);
		camera.SetWorldTransform(cameraTransform);
		camera.SetPerspectiveFov(90.0f, float(SCR_WIDTH) / SCR_HEIGHT, 1.0f, 1000.0f);
	}
private:
	Camera camera;
	Vector3 pos;
	Vector3 dir;

	GeoMipmap<Vector2> geoMipmap;
	//MorphQuad<Vector2> morphQuad;
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