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

class TextureImpl
{
public:
	TextureImpl(Texture::Type type_)
		: type(type_)
		, format(Texture::Format::RGBA)
		, internalformat(GL_RGBA)
		, pixelFormat(Texture::PixelFormat::UNSIGNED_BYTE)
		, handle(0)

		, warpS(Texture::Wrap::REPEAT)
		, warpT(Texture::Wrap::REPEAT)
		, warpR(Texture::Wrap::REPEAT)
		, minFilter(Texture::MinFilter::NEAREST)
		, magFilter(Texture::MagFilter::NEAREST)
	{
	}

	Texture::Type type;
	Texture::Format format;
	unsigned internalformat;
	Texture::PixelFormat pixelFormat;
	unsigned int handle;

	Texture::Wrap warpS;
	Texture::Wrap warpR;
	Texture::Wrap warpT;
	Texture::MinFilter minFilter;
	Texture::MagFilter magFilter;
};

////////////////////////////////////////////////////////////////////////
Texture::Texture(Texture::Type type_)
: impl(nullptr)
{
	impl = new TextureImpl(type_);
}

Texture::~Texture()
{
	Destroy();

	if (impl)
	{
		delete impl;
		impl = nullptr;
	}
}

bool Texture::Create()
{
	Assert(impl);

	return true;
}

void Texture::Destroy()
{
	Assert(impl);

	if (impl->handle)
	{
		glDeleteTextures(1, &impl->handle);
		impl->handle = 0;

		Platform::MemSet(impl, 0, sizeof(*impl));
	}
}

void Texture::Bind(unsigned int texStage_)
{
	Assert(impl);

	if (impl->handle)
	{
		glActiveTexture(GL_TEXTURE0 + texStage_);
		glBindTexture(texGLTypes[(int)impl->type], impl->handle);

		glTexParameteri(texGLTypes[(int)impl->type], GL_TEXTURE_WRAP_S, texGLWraps[(int)impl->warpS]);
		glTexParameteri(texGLTypes[(int)impl->type], GL_TEXTURE_WRAP_T, texGLWraps[(int)impl->warpT]);
		glTexParameteri(texGLTypes[(int)impl->type], GL_TEXTURE_WRAP_R, texGLWraps[(int)impl->warpR]);
		glTexParameteri(texGLTypes[(int)impl->type], GL_TEXTURE_MIN_FILTER, texGLMinFilters[(int)impl->minFilter]);
		glTexParameteri(texGLTypes[(int)impl->type], GL_TEXTURE_MAG_FILTER, texGLMagFilters[(int)impl->magFilter]);
	}
}

void Texture::Unbind()
{
	Assert(impl);

	glBindTexture(texGLTypes[(int)impl->type], 0);
}

Texture::Type Texture::GetType()  const
{
	Assert(impl);

	return impl->type;
}

Texture::Format Texture::GetFormat()  const
{
	Assert(impl);

	return impl->format;
}

Texture::PixelFormat Texture::GetPixelFormat()  const
{
	Assert(impl);

	return impl->pixelFormat;
}

unsigned int Texture::GetHandle() const
{
	Assert(impl);

	return impl->handle;
}

void Texture::SetWarpS(Texture::Wrap warpS_)
{
	Assert(impl);

	impl->warpS = warpS_;
}

void Texture::SetWarpT(Texture::Wrap warpT_)
{
	Assert(impl);

	impl->warpT = warpT_;
}

void Texture::SetWarpR(Texture::Wrap warpR_)
{
	Assert(impl);

	impl->warpR = warpR_;
}

void Texture::SetMinFilter(Texture::MinFilter minFilter_)
{
	Assert(impl);

	impl->minFilter = minFilter_;
}

void Texture::SetMagFilter(Texture::MagFilter magFilter_)
{
	Assert(impl);

	impl->magFilter = magFilter_;
}

Texture::Wrap Texture::GetWarpS() const
{
	Assert(impl);

	return impl->warpS;
}

Texture::Wrap Texture::GetWarpT() const
{
	Assert(impl);

	return impl->warpT;
}

Texture::Wrap Texture::GetWarpR() const
{
	Assert(impl);

	return impl->warpR;
}

Texture::MinFilter Texture::GetMinFilter() const
{
	Assert(impl);

	return impl->minFilter;
}

Texture::MagFilter Texture::GetMagFilter() const
{
	Assert(impl);

	return impl->magFilter;
}

void Texture::SetFormat(unsigned int internalformat, Texture::Format format, Texture::Type type)
{
	Assert(impl);

	impl->internalformat = internalformat;
	impl->format = format;
	impl->type = type;
}

void Texture::SetFormat(unsigned int nrComponents, bool isHDR)
{
	Assert(impl);

	if (nrComponents == 1)
	{
		impl->format = Texture::Format::RED;
		if (isHDR)
		{
			impl->internalformat = GL_R32F;
		}
		else
		{
			impl->internalformat = GL_R16;
		}
	}
	else if (nrComponents == 2)
	{
		impl->format = Texture::Format::RG;
		if (isHDR)
		{
			impl->internalformat = GL_RG32F;
		}
		else
		{
			impl->internalformat = GL_RG16;
		}
	}
	else if (nrComponents == 3)
	{
		impl->format = Texture::Format::RGB;
		if (isHDR)
		{
			impl->internalformat = GL_RGB32F;
		}
		else
		{
			impl->internalformat = GL_RGB;
		}
	}
	else if (nrComponents == 4)
	{
		impl->format = Texture::Format::RGBA;
		if (isHDR)
		{
			impl->internalformat = GL_RGBA32F;
		}
		else
		{
			impl->internalformat = GL_RGBA;
		}
	}

	if (isHDR)
		impl->pixelFormat = Texture::PixelFormat::FLOAT;
	else
		impl->pixelFormat = Texture::PixelFormat::UNSIGNED_BYTE;
}

////////////////////////////////////////////////////////////////////////////////////
Texture2D::Texture2D()
	: Texture(Texture::Type::TEXTURE_2D)
	, width(0)
	, height(0)
{
	Assert(impl);
}

Texture2D::~Texture2D()
{
	Assert(impl);

	Destroy();
}

bool Texture2D::Create(unsigned int width_, unsigned int height_, unsigned int internalformat_, Texture::Format format_, Texture::Type type_, void* data_)
{
	Assert(impl);

	SetFormat(internalformat_, format_, impl->type);

	width = width_;
	height = height_;
	glGenTextures(1, &impl->handle);
	glBindTexture(GL_TEXTURE_2D, impl->handle);
	glTexImage2D(GL_TEXTURE_2D, 0, internalformat_, width_, height_, 0, texGLFormats[(int)format_], texGLPixelFormats[(int)impl->pixelFormat], data_);

	return Texture::Create();
}

bool Texture2D::Create(unsigned int width, unsigned int height, unsigned int nrComponents, bool isHDR, void* data, bool vflip)
{
	Assert(impl);

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
	glGenTextures(1, &this->impl->handle);
	glBindTexture(GL_TEXTURE_2D, this->impl->handle);
	glTexImage2D(GL_TEXTURE_2D, 0, impl->internalformat, width, height, 0, texGLFormats[(int)impl->format], texGLPixelFormats[(int)impl->pixelFormat], data);
	glGenerateMipmap(GL_TEXTURE_2D);

	return Texture::Create();
}

void Texture2D::Update()
{
	Assert(impl);
}

void Texture2D::UpdateData(void* data)
{
	Assert(impl);

	glBindTexture(GL_TEXTURE_2D, impl->handle);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, texGLFormats[(int)impl->format], texGLPixelFormats[(int)impl->pixelFormat], data);
}

void Texture2D::Destroy()
{
	Assert(impl);

	return Texture::Destroy();
}

void Texture2D::GetResolution(Vector3& resolution) const
{
	Assert(impl);

	resolution = Vector3((float)width, (float)height, 1.0f);
}

unsigned int Texture2D::GetWidth() const
{
	Assert(impl);

	return width;
}

unsigned int Texture2D::GetHeight() const
{
	Assert(impl);

	return height;
}

////////////////////////////////////////////////////////////////////////////////////
Texture2DFile::Texture2DFile()
: Texture2D()
{
}

Texture2DFile::~Texture2DFile()
{
	Assert(impl);

	Destroy();
}

bool Texture2DFile::Create(const std::string& path_, bool vflip_)
{
	Assert(impl);

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
	Assert(impl);

	return Texture::Destroy();
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
	Assert(impl);

	Destroy();
}

bool TextureCubeMap::Create(unsigned int size, unsigned int nrComponents, bool isHDR, void* data, bool vflip)
{
	Assert(impl);

	this->size = size;
	SetFormat(nrComponents, isHDR);
	if (isHDR)
		faceDataSize = size * size * nrComponents * sizeof(float);
	else
		faceDataSize = size * size * nrComponents * sizeof(unsigned char);

	unsigned char* dataPtr = (unsigned char*)data;

	glGenTextures(1, &impl->handle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, impl->handle);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, impl->internalformat, size, size, 0, texGLFormats[(int)impl->format], texGLPixelFormats[(int)impl->pixelFormat], data ? dataPtr : data); dataPtr += faceDataSize;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, impl->internalformat, size, size, 0, texGLFormats[(int)impl->format], texGLPixelFormats[(int)impl->pixelFormat], data ? dataPtr : data); dataPtr += faceDataSize;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, impl->internalformat, size, size, 0, texGLFormats[(int)impl->format], texGLPixelFormats[(int)impl->pixelFormat], data ? dataPtr : data); dataPtr += faceDataSize;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, impl->internalformat, size, size, 0, texGLFormats[(int)impl->format], texGLPixelFormats[(int)impl->pixelFormat], data ? dataPtr : data); dataPtr += faceDataSize;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, impl->internalformat, size, size, 0, texGLFormats[(int)impl->format], texGLPixelFormats[(int)impl->pixelFormat], data ? dataPtr : data); dataPtr += faceDataSize;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, impl->internalformat, size, size, 0, texGLFormats[(int)impl->format], texGLPixelFormats[(int)impl->pixelFormat], data ? dataPtr : data); dataPtr += faceDataSize;
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	return Texture::Create();
}

void TextureCubeMap::Destroy()
{
	Assert(impl);

	return Texture::Destroy();
}

void TextureCubeMap::Update()
{
	Assert(impl);
}

void TextureCubeMap::UpdateData(void* data)
{
	Assert(impl);

	unsigned char* dataPtr = (unsigned char*)data;

	glBindTexture(GL_TEXTURE_CUBE_MAP, impl->handle);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, size, size, texGLFormats[(int)impl->format], texGLPixelFormats[(int)impl->pixelFormat], data); dataPtr += faceDataSize;
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, size, size, texGLFormats[(int)impl->format], texGLPixelFormats[(int)impl->pixelFormat], data); dataPtr += faceDataSize;
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, 0, size, size, texGLFormats[(int)impl->format], texGLPixelFormats[(int)impl->pixelFormat], data); dataPtr += faceDataSize;
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, 0, size, size, texGLFormats[(int)impl->format], texGLPixelFormats[(int)impl->pixelFormat], data); dataPtr += faceDataSize;
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, 0, size, size, texGLFormats[(int)impl->format], texGLPixelFormats[(int)impl->pixelFormat], data); dataPtr += faceDataSize;
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, 0, size, size, texGLFormats[(int)impl->format], texGLPixelFormats[(int)impl->pixelFormat], data); dataPtr += faceDataSize;
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void TextureCubeMap::GetResolution(Vector3& resolution) const
{
	Assert(impl);

	resolution = Vector3(size, size, 1.0);
}

unsigned int TextureCubeMap::GetSize() const
{
	Assert(impl);

	return size;
}

TextureCubeMapFile::TextureCubeMapFile()
	: TextureCubeMap()
{
}

TextureCubeMapFile::~TextureCubeMapFile()
{
	Assert(impl);

	Destroy();
}

bool TextureCubeMapFile::Create(const std::string& path_, bool vflip_)
{
	Assert(impl);

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
	Assert(impl);

	return Texture::Destroy();
}