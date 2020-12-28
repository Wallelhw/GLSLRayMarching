#ifndef _Texture_h_
#define _Texture_h_

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <vector>

#include "Vector3.h"

class Texture
{
public:
	Texture(unsigned int type_)
		: handle(0)
		, type(type_)
		, format(GL_RGBA)
		, internalformat(GL_RGBA)
		, pixelFormat(GL_UNSIGNED_BYTE)

		, warpS(GL_REPEAT)
		, warpT(GL_REPEAT)
		, warpR(GL_REPEAT)

		, minFilter(GL_NEAREST)
		, magFilter(GL_NEAREST)
	{
	}

	virtual ~Texture()
	{
	}

	virtual bool Create()
	{
		return true;
	}

	virtual void Destroy()
	{
	}

	void Bind(unsigned int texStage_) const
	{
		if (handle)
		{
			glActiveTexture(GL_TEXTURE0 + texStage_);
			glBindTexture(type, handle);

			glTexParameteri(type, GL_TEXTURE_WRAP_S, warpS);
			glTexParameteri(type, GL_TEXTURE_WRAP_T, warpT);
			glTexParameteri(type, GL_TEXTURE_WRAP_R, warpR);
			glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
	}

	void Unbind() const
	{
		glBindTexture(type, 0);
	}

	virtual void Update() = 0;

	void SetWarpS(unsigned int warpS_)
	{
		warpS = warpS_;
	}

	void SetWarpT(unsigned int warpT_)
	{
		warpT = warpT_;
	}

	void SetWarpR(unsigned int warpR_)
	{
		warpR = warpR_;
	}

	void SetMinFilter(unsigned int minFilter_)
	{
		minFilter = minFilter_;
	}

	void SetMagFilter(unsigned int magFilter_)
	{
		magFilter = magFilter_;
	}

	unsigned int GetWarpS() const
	{
		return warpS;
	}

	unsigned int GetWarpT() const
	{
		return warpT;
	}

	unsigned int GetWarpR() const
	{
		return warpR;
	}

	unsigned int GetMinFilter() const
	{
		return minFilter;
	}

	unsigned int GetMagFilter() const
	{
		return magFilter;
	}

	unsigned int GetType()  const
	{
		return type;
	}

	unsigned int GetHandle() const
	{
		return handle;
	}

	virtual void GetResolution(vec3& resolution) const = 0;
public:
protected:
	void SetFormat(unsigned int internalformat, unsigned int format, unsigned int type)
	{
		this->internalformat = internalformat;
		this->format = format;
		this->type = type;
	}

	void SetFormat(unsigned int nrComponents, bool isHDR)
	{
		if (nrComponents == 1)
		{
			format = GL_RED;
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
			format = GL_RG;
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
			format = GL_RGB;
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
			format = GL_RGBA;
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
			pixelFormat = GL_FLOAT;
		else
			pixelFormat = GL_UNSIGNED_BYTE;
	}
private:


public:
protected:
	unsigned int handle;

	unsigned int type;
	unsigned int format;
	unsigned int internalformat;
	unsigned int pixelFormat;

	unsigned int warpS;
	unsigned int warpR;
	unsigned int warpT;
	unsigned int minFilter;
	unsigned int magFilter;
private:
};


class Texture2D : public Texture
{
public:
	Texture2D()
		: Texture(GL_TEXTURE_2D)
		, width(0)
		, height(0)
	{
	}

	virtual ~Texture2D()
	{
	}

	void Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	bool Create(unsigned int width, unsigned int height, unsigned int internalformat, unsigned int format, unsigned int type, void* data)
	{
		SetFormat(internalformat, format, type);

		this->width = width;
		this->height = height;
		glGenTextures(1, &this->handle);
		glBindTexture(GL_TEXTURE_2D, this->handle);
		glTexImage2D(GL_TEXTURE_2D, 0, (GLint)internalformat, width, height, 0, (GLint)format, (GLint)pixelFormat, data);

		return Texture::Create();
	}

	bool Create(unsigned int width, unsigned int height, unsigned int nrComponents, bool isHDR, void* data, bool vflip = false)
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
		glTexImage2D(GL_TEXTURE_2D, 0, (GLint)internalformat, width, height, 0, (GLint)format, (GLint)pixelFormat, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		return Texture::Create();
	}

	virtual void Update()
	{
	}

	void UpdateData(void* data)
	{
		glBindTexture(GL_TEXTURE_2D, this->handle);
		
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, (GLint)format, (GLint)pixelFormat, data);
	}

	void Destroy()
	{
		if (handle)
		{
			glDeleteTextures(1, &handle);
			handle = 0;
		}
	}

	virtual void GetResolution(vec3& resolution) const
	{
		resolution = vec3(width, height, 1.0);
	}

	unsigned int GetWidth() const
	{
		return width;
	}

	unsigned int GetHeight() const
	{
		return height;
	}
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
	Texture2DFile()
	: Texture2D()
	{
	}

	virtual ~Texture2DFile()
	{
	}

	bool Create(const std::string& path_, bool vflip_)
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

	void Destroy()
	{
		if (handle)
		{
			glDeleteTextures(1, &handle);
			handle = 0;
		}
	}
protected:
private:

public:
protected:
private:
};

class TextureCubeMap : public Texture
{
public:
	TextureCubeMap()
		: Texture(GL_TEXTURE_CUBE_MAP)
		, size(0)
		, faceDataSize(0)
	{
	}

	virtual ~TextureCubeMap()
	{
	}

	virtual bool Create(unsigned int size, unsigned int nrComponents, bool isHDR, void* data, bool vflip = false)
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
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, (GLint)internalformat, size, size, 0, (GLint)format, (GLint)pixelFormat, data ? dataPtr : data); dataPtr += faceDataSize;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, (GLint)internalformat, size, size, 0, (GLint)format, (GLint)pixelFormat, data ? dataPtr : data); dataPtr += faceDataSize;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, (GLint)internalformat, size, size, 0, (GLint)format, (GLint)pixelFormat, data ? dataPtr : data); dataPtr += faceDataSize;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, (GLint)internalformat, size, size, 0, (GLint)format, (GLint)pixelFormat, data ? dataPtr : data); dataPtr += faceDataSize;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, (GLint)internalformat, size, size, 0, (GLint)format, (GLint)pixelFormat, data ? dataPtr : data); dataPtr += faceDataSize;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, (GLint)internalformat, size, size, 0, (GLint)format, (GLint)pixelFormat, data ? dataPtr : data); dataPtr += faceDataSize;
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		return Texture::Create();
	}

	void Destroy()
	{
		if (handle)
		{
			glDeleteTextures(1, &handle);
			handle = 0;
		}
	}

	virtual void Update()
	{
	}

	virtual void UpdateData(void* data)
	{
		unsigned char* dataPtr = (unsigned char*)data;

		glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, size, size, (GLint)format, (GLint)pixelFormat, data); dataPtr += faceDataSize;
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, size, size, (GLint)format, (GLint)pixelFormat, data); dataPtr += faceDataSize;
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, 0, size, size, (GLint)format, (GLint)pixelFormat, data); dataPtr += faceDataSize;
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, 0, size, size, (GLint)format, (GLint)pixelFormat, data); dataPtr += faceDataSize;
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, 0, size, size, (GLint)format, (GLint)pixelFormat, data); dataPtr += faceDataSize;
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, 0, size, size, (GLint)format, (GLint)pixelFormat, data); dataPtr += faceDataSize;
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

	virtual void GetResolution(vec3& resolution) const
	{
		resolution = vec3(size, size, 1.0);
	}

	unsigned int GetSize() const
	{
		return size;
	}
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
	TextureCubeMapFile()
		: TextureCubeMap()
	{
	}

	virtual ~TextureCubeMapFile()
	{
	}

	bool Create(const std::string& path_, bool vflip_)
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

	void Destroy()
	{
		if (handle)
		{
			glDeleteTextures(1, &handle);
			handle = 0;
		}
	}
protected:
private:


public:
protected:
private:
};

#endif