#include "Texture.h"
#include "Graphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

static unsigned int texGLTypes[] =
{
	GL_TEXTURE_1D,
	GL_TEXTURE_2D,
	GL_TEXTURE_3D,
	GL_TEXTURE_CUBE_MAP
};

static unsigned int texGLWraps[] =
{
	GL_REPEAT,
	GL_CLAMP
};

static unsigned int texGLMinFilters[] =
{
	GL_NEAREST,
	GL_LINEAR,
	GL_NEAREST_MIPMAP_NEAREST,
	GL_LINEAR_MIPMAP_NEAREST,
	GL_NEAREST_MIPMAP_LINEAR,
	GL_LINEAR_MIPMAP_LINEAR
};

static unsigned int texGLMagFilters[] =
{
	GL_NEAREST,
	GL_LINEAR,
	GL_NEAREST_MIPMAP_NEAREST,
	GL_LINEAR_MIPMAP_NEAREST,
	GL_NEAREST_MIPMAP_LINEAR,
	GL_LINEAR_MIPMAP_LINEAR
};

static unsigned int texGLFormats[] =
{
	GL_RED,
	GL_RG,
	GL_RGB,
	GL_BGR,
	GL_RGBA,
	GL_BGRA,
	GL_RED_INTEGER,
	GL_RG_INTEGER,
	GL_RGB_INTEGER,
	GL_BGR_INTEGER,
	GL_RGBA_INTEGER,
	GL_BGRA_INTEGER,
	GL_STENCIL_INDEX,
	GL_DEPTH_COMPONENT,
	GL_DEPTH_STENCIL
};

static unsigned int texGLPixelFormats[] =
{
	GL_UNSIGNED_BYTE,
	GL_BYTE,
	GL_UNSIGNED_SHORT,
	GL_SHORT,
	GL_UNSIGNED_INT,
	GL_INT,
	GL_HALF_FLOAT,
	GL_FLOAT,
	GL_UNSIGNED_BYTE_3_3_2,
	GL_UNSIGNED_BYTE_2_3_3_REV,
	GL_UNSIGNED_SHORT_5_6_5,
	GL_UNSIGNED_SHORT_5_6_5_REV,
	GL_UNSIGNED_SHORT_4_4_4_4,
	GL_UNSIGNED_SHORT_4_4_4_4_REV,
	GL_UNSIGNED_SHORT_5_5_5_1,
	GL_UNSIGNED_SHORT_1_5_5_5_REV,
	GL_UNSIGNED_INT_8_8_8_8,
	GL_UNSIGNED_INT_8_8_8_8_REV,
	GL_UNSIGNED_INT_10_10_10_2,
	GL_UNSIGNED_INT_2_10_10_10_REV
};

////////////////////////////////////////////////////////////////////////
Texture::Texture(Texture::Type type_)
	: handle(0)
	, type(type_)
	, format(Texture::Format::RGBA)
	, internalformat(GL_RGBA)
	, pixelFormat(Texture::PixelFormat::UNSIGNED_BYTE)

	, warpS(Texture::Wrap::REPEAT)
	, warpT(Texture::Wrap::REPEAT)
	, warpR(Texture::Wrap::REPEAT)

	, minFilter(Texture::MinFilter::NEAREST)
	, magFilter(Texture::MagFilter::NEAREST)
{
}

Texture::~Texture()
{
}

bool Texture::Create()
{
	return true;
}

void Texture::Destroy()
{
}

void Texture::Bind(unsigned int texStage_) const
{
	if (handle)
	{
		glActiveTexture(GL_TEXTURE0 + texStage_);
		glBindTexture(texGLTypes[(int)type], handle);

		glTexParameteri(texGLTypes[(int)type], GL_TEXTURE_WRAP_S, texGLWraps[(int)warpS]);
		glTexParameteri(texGLTypes[(int)type], GL_TEXTURE_WRAP_T, texGLWraps[(int)warpT]);
		glTexParameteri(texGLTypes[(int)type], GL_TEXTURE_WRAP_R, texGLWraps[(int)warpR]);
		glTexParameteri(texGLTypes[(int)type], GL_TEXTURE_MIN_FILTER, texGLMinFilters[(int)minFilter]);
		glTexParameteri(texGLTypes[(int)type], GL_TEXTURE_MAG_FILTER, texGLMagFilters[(int)magFilter]);
	}
}

void Texture::Unbind() const
{
	glBindTexture(texGLTypes[(int)type], 0);
}

void Texture::SetWarpS(Texture::Wrap warpS_)
{
	warpS = warpS_;
}

void Texture::SetWarpT(Texture::Wrap warpT_)
{
	warpT = warpT_;
}

void Texture::SetWarpR(Texture::Wrap warpR_)
{
	warpR = warpR_;
}

void Texture::SetMinFilter(Texture::MinFilter minFilter_)
{
	minFilter = minFilter_;
}

void Texture::SetMagFilter(Texture::MagFilter magFilter_)
{
	magFilter = magFilter_;
}

Texture::Wrap Texture::GetWarpS() const
{
	return warpS;
}

Texture::Wrap Texture::GetWarpT() const
{
	return warpT;
}

Texture::Wrap Texture::GetWarpR() const
{
	return warpR;
}

Texture::MinFilter Texture::GetMinFilter() const
{
	return minFilter;
}

Texture::MagFilter Texture::GetMagFilter() const
{
	return magFilter;
}

Texture::Type Texture::GetType()  const
{
	return type;
}

Texture::Format Texture::GetFormat()  const
{
	return format;
}

Texture::PixelFormat Texture::GetPixelFormat()  const
{
	return pixelFormat;
}

unsigned int Texture::GetHandle() const
{
	return handle;
}

void Texture::SetFormat(unsigned int internalformat, Texture::Format format, Texture::Type type)
{
	this->internalformat = internalformat;
	this->format = format;
	this->type = type;
}

void Texture::SetFormat(unsigned int nrComponents, bool isHDR)
{
	if (nrComponents == 1)
	{
		format = Texture::Format::RED;
		if (isHDR)
		{
			internalformat = GL_R32F;
		}
		else
		{
			internalformat = GL_R16;
		}
	}
	else if (nrComponents == 2)
	{
		format = Texture::Format::RG;
		if (isHDR)
		{
			internalformat = GL_RG32F;
		}
		else
		{
			internalformat = GL_RG16;
		}
	}
	else if (nrComponents == 3)
	{
		format = Texture::Format::RGB;
		if (isHDR)
		{
			internalformat = GL_RGB32F;
		}
		else
		{
			internalformat = GL_RGB;
		}
	}
	else if (nrComponents == 4)
	{
		format = Texture::Format::RGBA;
		if (isHDR)
		{
			internalformat = GL_RGBA32F;
		}
		else
		{
			internalformat = GL_RGBA;
		}
	}

	if (isHDR)
		pixelFormat = Texture::PixelFormat::FLOAT;
	else
		pixelFormat = Texture::PixelFormat::UNSIGNED_BYTE;
}

////////////////////////////////////////////////////////////////////////////////////
Texture2D::Texture2D()
	: Texture(Texture::Type::TEXTURE_2D)
	, width(0)
	, height(0)
{
}

Texture2D::~Texture2D()
{
}

bool Texture2D::Create(unsigned int width, unsigned int height, unsigned int internalformat, Texture::Format format, Texture::Type type, void* data)
{
	SetFormat(internalformat, format, type);

	this->width = width;
	this->height = height;
	glGenTextures(1, &this->handle);
	glBindTexture(GL_TEXTURE_2D, this->handle);
	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, texGLFormats[(int)format], texGLPixelFormats[(int)pixelFormat], data);

	return Texture::Create();
}

bool Texture2D::Create(unsigned int width, unsigned int height, unsigned int nrComponents, bool isHDR, void* data, bool vflip)
{
	SetFormat(nrComponents, isHDR);

	if (isHDR)
	{
		if (vflip)
			stbi__vertical_flip(data, width, height, nrComponents * 4);
	}
	else
	{
		if (vflip)
			stbi__vertical_flip(data, width, height, nrComponents * 1);
	}

	this->width = width;
	this->height = height;
	glGenTextures(1, &this->handle);
	glBindTexture(GL_TEXTURE_2D, this->handle);
	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, texGLFormats[(int)format], texGLPixelFormats[(int)pixelFormat], data);
	glGenerateMipmap(GL_TEXTURE_2D);

	return Texture::Create();
}

void Texture2D::Update()
{
}

void Texture2D::UpdateData(void* data)
{
	glBindTexture(GL_TEXTURE_2D, this->handle);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, texGLFormats[(int)format], texGLPixelFormats[(int)pixelFormat], data);
}

void Texture2D::Destroy()
{
	if (handle)
	{
		glDeleteTextures(1, &handle);
		handle = 0;
	}
}

void Texture2D::GetResolution(Vector3& resolution) const
{
	resolution = Vector3(width, height, 1.0);
}

unsigned int Texture2D::GetWidth() const
{
	return width;
}

unsigned int Texture2D::GetHeight() const
{
	return height;
}

////////////////////////////////////////////////////////////////////////////////////
Texture2DFile::Texture2DFile()
: Texture2D()
{
}

Texture2DFile::~Texture2DFile()
{
}

bool Texture2DFile::Create(const std::string& path_, bool vflip_)
{
	bool isHDR = stbi_is_hdr(path_.c_str());

	int width, height, nrComponents;

	void* data = nullptr;
	if (isHDR)
	{
		data = stbi_loadf(path_.c_str(), &width, &height, &nrComponents, 0);
	}
	else
	{
		data = stbi_load(path_.c_str(), &width, &height, &nrComponents, 0);
	}

	if (data)
	{
		bool result = Texture2D::Create(width, height, nrComponents, isHDR, data, vflip_);

		stbi_image_free(data);

		return result;
	}
	else
	{
		return false;
	}
}

void Texture2DFile::Destroy()
{
	if (handle)
	{
		glDeleteTextures(1, &handle);
		handle = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////
TextureCubeMap::TextureCubeMap()
	: Texture(Texture::Type::TEXTURE_CUBE_MAP)
	, size(0)
	, faceDataSize(0)
{
}

TextureCubeMap::~TextureCubeMap()
{
}

bool TextureCubeMap::Create(unsigned int size, unsigned int nrComponents, bool isHDR, void* data, bool vflip)
{
	this->size = size;
	SetFormat(nrComponents, isHDR);
	if (isHDR)
		faceDataSize = size * size * nrComponents * sizeof(float);
	else
		faceDataSize = size * size * nrComponents * sizeof(unsigned char);

	unsigned char* dataPtr = (unsigned char*)data;

	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalformat, size, size, 0, texGLFormats[(int)format], texGLPixelFormats[(int)pixelFormat], data ? dataPtr : data); dataPtr += faceDataSize;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalformat, size, size, 0, texGLFormats[(int)format], texGLPixelFormats[(int)pixelFormat], data ? dataPtr : data); dataPtr += faceDataSize;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalformat, size, size, 0, texGLFormats[(int)format], texGLPixelFormats[(int)pixelFormat], data ? dataPtr : data); dataPtr += faceDataSize;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalformat, size, size, 0, texGLFormats[(int)format], texGLPixelFormats[(int)pixelFormat], data ? dataPtr : data); dataPtr += faceDataSize;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalformat, size, size, 0, texGLFormats[(int)format], texGLPixelFormats[(int)pixelFormat], data ? dataPtr : data); dataPtr += faceDataSize;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalformat, size, size, 0, texGLFormats[(int)format], texGLPixelFormats[(int)pixelFormat], data ? dataPtr : data); dataPtr += faceDataSize;
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	return Texture::Create();
}

void TextureCubeMap::Destroy()
{
	if (handle)
	{
		glDeleteTextures(1, &handle);
		handle = 0;
	}
}

void TextureCubeMap::Update()
{
}

void TextureCubeMap::UpdateData(void* data)
{
	unsigned char* dataPtr = (unsigned char*)data;

	glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, size, size, texGLFormats[(int)format], texGLPixelFormats[(int)pixelFormat], data); dataPtr += faceDataSize;
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, size, size, texGLFormats[(int)format], texGLPixelFormats[(int)pixelFormat], data); dataPtr += faceDataSize;
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, 0, size, size, texGLFormats[(int)format], texGLPixelFormats[(int)pixelFormat], data); dataPtr += faceDataSize;
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, 0, size, size, texGLFormats[(int)format], texGLPixelFormats[(int)pixelFormat], data); dataPtr += faceDataSize;
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, 0, size, size, texGLFormats[(int)format], texGLPixelFormats[(int)pixelFormat], data); dataPtr += faceDataSize;
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, 0, size, size, texGLFormats[(int)format], texGLPixelFormats[(int)pixelFormat], data); dataPtr += faceDataSize;
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void TextureCubeMap::GetResolution(Vector3& resolution) const
{
	resolution = Vector3(size, size, 1.0);
}

unsigned int TextureCubeMap::GetSize() const
{
	return size;
}

TextureCubeMapFile::TextureCubeMapFile()
	: TextureCubeMap()
{
}

TextureCubeMapFile::~TextureCubeMapFile()
{
}

bool TextureCubeMapFile::Create(const std::string& path_, bool vflip_)
{
	bool isHDR = stbi_is_hdr(path_.c_str());

	int width, height, nrComponents;
	void* data = nullptr;
	if (isHDR)
		data = stbi_loadf(path_.c_str(), &width, &height, &nrComponents, 0);
	else
		data = stbi_load(path_.c_str(), &width, &height, &nrComponents, 0);

	if (data)
	{
		bool result = TextureCubeMap::Create(width, nrComponents, isHDR, data, vflip_);

		stbi_image_free(data);

		return result;
	}
	else
	{
		return false;
	}
}

void TextureCubeMapFile::Destroy()
{
	if (handle)
	{
		glDeleteTextures(1, &handle);
		handle = 0;
	}
}