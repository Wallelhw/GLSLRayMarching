#ifndef _Texture_h_
#define _Texture_h_

#include "Platform.h"
#include "Vector3.h"

class Texture
{
public:
	enum class Type
	{
		TEXTURE_1D = 0,
		TEXTURE_2D,
		TEXTURE_3D,
		TEXTURE_CUBE_MAP
	};

	enum class Wrap
	{
		REPEAT = 0,
		CLAMP
	};

	enum class MinFilter
	{
		NEAREST = 0,
		LINEAR,
		NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR
	};

	enum class MagFilter
	{
		NEAREST = 0,
		LINEAR,
		NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR
	};

	enum class Format
	{
		RED = 0,
		RG,
		RGB,
		BGR,
		RGBA,
		BGRA,
		RED_INTEGER,
		RG_INTEGER,
		RGB_INTEGER,
		BGR_INTEGER,
		RGBA_INTEGER,
		BGRA_INTEGER,
		STENCIL_INDEX,
		DEPTH_COMPONENT,
		DEPTH_STENCIL
	};

	enum class PixelFormat
	{
		UNSIGNED_BYTE = 0,
		BYTE,
		UNSIGNED_SHORT,
		SHORT,
		UNSIGNED_INT,
		INT,
		HALF_FLOAT,
		FLOAT,
		UNSIGNED_BYTE_3_3_2,
		UNSIGNED_BYTE_2_3_3_REV,
		UNSIGNED_SHORT_5_6_5,
		UNSIGNED_SHORT_5_6_5_REV,
		UNSIGNED_SHORT_4_4_4_4,
		UNSIGNED_SHORT_4_4_4_4_REV,
		UNSIGNED_SHORT_5_5_5_1,
		UNSIGNED_SHORT_1_5_5_5_REV,
		UNSIGNED_INT_8_8_8_8,
		UNSIGNED_INT_8_8_8_8_REV,
		UNSIGNED_INT_10_10_10_2,
		UNSIGNED_INT_2_10_10_10_REV
	};

	Texture(Texture::Type type_);
	virtual ~Texture();
	
	virtual bool Create();
	virtual void Destroy();

	void Bind(unsigned int texStage_) const;
	void Unbind() const;

	virtual void Update() = 0;

	void SetWarpS(Texture::Wrap warpS_);
	void SetWarpT(Texture::Wrap warpT_);
	void SetWarpR(Texture::Wrap warpR_);
	void SetMinFilter(Texture::MinFilter minFilter_);
	void SetMagFilter(Texture::MagFilter magFilter_);
	Texture::Wrap GetWarpS() const;
	Texture::Wrap GetWarpT() const;
	Texture::Wrap GetWarpR() const;
	Texture::MinFilter GetMinFilter() const;
	Texture::MagFilter GetMagFilter() const;
	Texture::Type GetType()  const;
	Texture::Format GetFormat()  const;
	Texture::PixelFormat GetPixelFormat()  const;

	unsigned int GetHandle() const;

	virtual void GetResolution(Vector3& resolution) const = 0;
public:
protected:
	void SetFormat(unsigned int internalformat, Texture::Format format, Texture::Type type);
	void SetFormat(unsigned int nrComponents, bool isHDR);
private:
public:
protected:
	unsigned int handle;

	Texture::Type type;
	Texture::Format format;
	unsigned internalformat;
	Texture::PixelFormat pixelFormat;

	Texture::Wrap warpS;
	Texture::Wrap warpR;
	Texture::Wrap warpT;
	Texture::MinFilter minFilter;
	Texture::MagFilter magFilter;
private:
};

class Texture2D : public Texture
{
public:
	Texture2D();
	virtual ~Texture2D();
	
	bool Create(unsigned int width, unsigned int height, unsigned int internalformat, Texture::Format format, Texture::Type type, void* data);
	bool Create(unsigned int width, unsigned int height, unsigned int nrComponents, bool isHDR, void* data, bool vflip = false);
	void Destroy();
	
	virtual void Update();
	void UpdateData(void* data);
	
	virtual void GetResolution(Vector3& resolution) const;
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
protected:
private:

public:
protected:
	unsigned int width;
	unsigned int height;
private:
};

class Texture2DFile : public Texture2D
{
public:
	Texture2DFile();
	virtual ~Texture2DFile();

	bool Create(const std::string& path_, bool vflip_);
	void Destroy();
protected:
private:

public:
protected:
private:
};

class TextureCubeMap : public Texture
{
public:
	TextureCubeMap();
	virtual ~TextureCubeMap();

	virtual bool Create(unsigned int size, unsigned int nrComponents, bool isHDR, void* data, bool vflip = false);
	void Destroy();
	
	virtual void Update();
	virtual void UpdateData(void* data);
	
	virtual void GetResolution(Vector3& resolution) const;
	unsigned int GetSize() const;
private:

public:
protected:
private:
	unsigned int size;
	unsigned int faceDataSize;
};

class TextureCubeMapFile : public TextureCubeMap
{
public:
	TextureCubeMapFile();
	virtual ~TextureCubeMapFile();

	bool Create(const std::string& path_, bool vflip_);
	void Destroy();
protected:
private:


public:
protected:
private:
};

#endif