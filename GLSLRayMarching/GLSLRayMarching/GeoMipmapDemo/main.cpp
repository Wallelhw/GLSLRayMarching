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
#include "GeoMipmap.h"

template<class T>
class Array2D
{
public:
	Array2D()
	: size(0, 0)
	{
	}

	Array2D(const IVector2& size_)
	: size(size_)
	{
		Reserve(size_);
	}

	~Array2D()
	{
	}

	void Reserve(const IVector2& size_)
	{
		size = size_;

		m.resize(GetCount());
	}

	const T& operator [] (const IVector2& coord_) const
	{
		return m[I(coord_)];
	}

	const T& operator [] (unsigned int i_) const
	{
		Assert(i_ < GetCount());

		return m[i_];
	}

	T& operator [] (const IVector2& coord_)
	{
		return m[I(coord_)];
	}

	T& operator [] (unsigned int i_)
	{
		Assert(i_ < GetCount());

		return m[i_];
	}

	const IVector2& GetSize() const
	{
		return size;
	}

	unsigned int GetCount() const
	{
		return size.X() * size.Y();
	}
private:
	unsigned int I(const IVector2& coord_) const
	{
		Assert(coord_.X() < size.X() && coord_.Y() < size.Y());

		return coord_.Y() * size.X() + coord_.X();
	}

	IVector2 size;
	std::vector<T> m;
};

class BlendGeoMipmap
{
public:
	class Section
	{
	public:
		Section()
		: numQuadPerSections(IVector2::Zero)
		{
		}

		~Section()
		{
		}

		bool Create(const IVector2& numQuadPerSections_)
		{
			numQuadPerSections = numQuadPerSections_;
		}

		IVector2 GetNumSectionPerComponents() const
		{
			return numQuadPerSections;
		}

		IVector2 numQuadPerSections;
	};

	class Component
	{
	public:
		Component()
		{
		}

		~Component()
		{
		}

		bool Create(const IVector2& numSectionPerComponents, const IVector2& numQuadPerSections)
		{
			sections.Reserve(numSectionPerComponents);
			for (int i = 0; i < sections.GetCount(); i++)
			{
				sections[i];
			}
		}

		IVector2 GetNumSectionPerComponents() const
		{
			return sections.GetSize();
		}

		const Section& GetSection(IVector2 i) const
		{
			return sections[i];
		}

		Section& GetSection(IVector2 i)
		{
			return sections[i];
		}
	private:
		Array2D<Section> sections;
	};

	BlendGeoMipmap()
		: numComponents(IVector2::Zero)
		, numSectionPerComponents(IVector2::Zero)
		, numQuadPerSections(IVector2::Zero)
	{
	}

	~BlendGeoMipmap()
	{
	}

	bool Create(const IVector2& numComponents, const IVector2& numSectionPerComponents, const IVector2& numQuadPerSections)
	{
		this->numComponents = numComponents;
		this->numSectionPerComponents = numSectionPerComponents;
		this->numQuadPerSections = numQuadPerSections;
		this->resolution = numComponents * numSectionPerComponents * numQuadPerSections + IVector2(1, 1);
	}

	IVector2 GetNumComponents() const
	{
		return numComponents;
	}

	IVector2 GetNumSectionPerComponents() const
	{
		return numSectionPerComponents;
	}

	IVector2 GetQuadPerSections() const
	{
		return numQuadPerSections;
	}

	IVector2 GetResolution() const
	{
		return resolution;
	}

	void GetQuads(std::vector<IVector2>& quads, const Camera& c) const
	{

	}

	IVector2 numComponents;
	IVector2 numSectionPerComponents;
	IVector2 numQuadPerSections;
	IVector2 resolution;
};

//////////////////////////////////////////////////////////////////////
class GeoMipmapDemo : public FrameWork
{
public:
	GeoMipmapDemo()
		: FrameWork("GeoMipmapDemo")
		, pos(100, 600, 100)
		, phi(0.7343)
		, theta(-0.5733)
	{
	}

	virtual ~GeoMipmapDemo()
	{
	}

	virtual bool OnCreate() override
	{
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
		Vector2 mousedelta = GetMouseDelta();
		Platform::Debug("%f %f\n", mousedelta.X(), mousedelta.Y());

		if (GetMouseRightButtonStatus())
		{
			phi += mousedelta.X() / (SCR_WIDTH / 2) * 180 * Math::Degree2Radian;
			//if (phi > 180 * Math::Degree2Radian)
				//phi = 180 * Math::Degree2Radian;
			//if (phi < -180 * Math::Degree2Radian)
				//phi = -180 * Math::Degree2Radian;

			theta += mousedelta.Y() / (SCR_HEIGHT / 2) * 180 * Math::Degree2Radian;
			if (theta > 89.9 * Math::Degree2Radian)
				theta = 89.9 * Math::Degree2Radian;
			if (theta < -89.9 * Math::Degree2Radian)
				theta = -89.9 * Math::Degree2Radian;
		}

		Vector3 dir = Vector3(Math::Cos(theta) * Math::Cos(phi), Math::Sin(theta), Math::Cos(theta) * Math::Sin(phi)); dir.Normalize();
		Vector3 xAxis = dir.Cross(Vector3::UnitY); xAxis.Normalize();
//		Platform::Debug("%f %f\n", theta, phi);

		if (IsKeyPressed('W'))
		{
			pos += dir;
		}

		if (IsKeyPressed('S'))
		{
			pos -= dir ;
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
		camera.SetPerspectiveFov(90.0f, float(SCR_WIDTH) / SCR_HEIGHT, 1.0f, 5000.0f);
	}
private:
	Camera camera;
	Vector3 pos;
	float phi;
	float theta;

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