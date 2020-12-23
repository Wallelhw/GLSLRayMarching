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

	void Bind(unsigned int texStage_)
	{
		if (handle)
		{
			glActiveTexture(GL_TEXTURE0 + texStage_);
			glBindTexture(type, handle);

			glTexParameteri(type, GL_TEXTURE_WRAP_S, warpS);
			glTexParameteri(type, GL_TEXTURE_WRAP_T, warpT);
			glTexParameteri(type, GL_TEXTURE_WRAP_R, warpR);
			glTexParameteri(type, GL_TEXTURE_MIN_FILTER, minFilter);
			glTexParameteri(type, GL_TEXTURE_MAG_FILTER, magFilter);
		}
	}

	void Unbind()
	{
		glBindTexture(type, 0);
	}

	void Update(int xoffset, int yoffset, int width, int height, void* data)
	{
		glTexSubImage2D(type, 0, xoffset, yoffset, width, height, format, pixelFormat, data);
	}

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

	unsigned int GetMinFilter()
	{
		return minFilter;
	}

	unsigned int GetMagFilter()
	{
		return magFilter;
	}

	unsigned int GetType()
	{
		return type;
	}

	virtual void GetResolution(vec3& resolution) = 0;
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

class RenderTarget : public Texture
{
public:
	RenderTarget(unsigned int type_)
		: Texture(type_)
		, fbo(0)
	{
	}

	virtual ~RenderTarget()
	{
	}

	bool Create()
	{
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		if (!glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
			return false;

		if (type == GL_TEXTURE_2D)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, handle, 0);
		}
		else
		{
			assert(0); // not implement
		}

		//glGenRenderbuffers(1, &rbo);
		//glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			//return false;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return Texture::Create();
	}

	void Destroy()
	{
		if (fbo)
		{
			glDeleteFramebuffers(1, &fbo);
			fbo = 0;
		}
	}

	void BindFrameBuffer()
	{
		if (fbo)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		}
	}

	static void UnBindFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
private:
private:
	unsigned int fbo;
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

	bool Create(const std::string& path, bool vflip = false)
	{
		bool isHDR = stbi_is_hdr(path.c_str());

		int width, height, nrComponents;

		void* data = nullptr;
		if (isHDR)
		{
			data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
			if(vflip)
				stbi__vertical_flip(data, width, height, nrComponents * 4);
		}
		else
		{
			data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
			if (vflip)
				stbi__vertical_flip(data, width, height, nrComponents * 1);
		}

		if (data)
		{
			bool result = Create(width, height, nrComponents, isHDR, data);

			stbi_image_free(data);

			return result;
		}
		else
		{
			return false;
		}
	}

	bool Update(unsigned int width, unsigned int height, void* data)
	{
		this->width = width;
		this->height = height;
		glGenTextures(1, &this->handle);
		glBindTexture(GL_TEXTURE_2D, this->handle);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, (GLint)format, (GLint)pixelFormat, data);

		return true;
	}

	void Destroy()
	{
		if (handle)
		{
			glDeleteTextures(1, &handle);
			handle = 0;
		}
	}

	virtual void GetResolution(vec3& resolution)
	{
		resolution = vec3(width, height, 0.0);
	}
protected:
private:

private:
	unsigned int width;
	unsigned int height;
};

class TextureCubeMap : public Texture
{
public:
	TextureCubeMap()
		: Texture(GL_TEXTURE_CUBE_MAP)
	{
	}

	virtual ~TextureCubeMap()
	{
	}

	bool Create(unsigned int size, unsigned int nrComponents, bool isHDR, void* data)
	{
		this->size = size;
		SetFormat(nrComponents, isHDR);

		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
		int dataSize;
		if (isHDR)
			dataSize = size * size * nrComponents * sizeof(float);
		else
			dataSize = size * size * nrComponents * sizeof(unsigned char);

		unsigned char* dataPtr = (unsigned char*)data;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, (GLint)internalformat, size, size, 0, (GLint)format, (GLint)pixelFormat, dataPtr); dataPtr += dataSize;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, (GLint)internalformat, size, size, 0, (GLint)format, (GLint)pixelFormat, dataPtr); dataPtr += dataSize;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, (GLint)internalformat, size, size, 0, (GLint)format, (GLint)pixelFormat, dataPtr); dataPtr += dataSize;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, (GLint)internalformat, size, size, 0, (GLint)format, (GLint)pixelFormat, dataPtr); dataPtr += dataSize;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, (GLint)internalformat, size, size, 0, (GLint)format, (GLint)pixelFormat, dataPtr); dataPtr += dataSize;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, (GLint)internalformat, size, size, 0, (GLint)format, (GLint)pixelFormat, dataPtr); dataPtr += dataSize;
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		warpS = GL_CLAMP_TO_EDGE;
		warpT = GL_CLAMP_TO_EDGE;
		warpR = GL_CLAMP_TO_EDGE;

		return Texture::Create();
	}

	bool Create(const std::string& path)
	{
		bool isHDR = stbi_is_hdr(path.c_str());

		int width, height, nrComponents;
		void* data = nullptr;
		if (isHDR)
			data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
		else
			data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

		if (data)
		{
			bool result = Create(width, nrComponents, isHDR, data);

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

	virtual void GetResolution(vec3& resolution)
	{
		resolution = vec3(size, size, size);
	}
private:
private:
	unsigned int size;
};

class RenderTarget2D : public RenderTarget
{
public:
	RenderTarget2D()
		: RenderTarget(GL_TEXTURE_2D)
		, width(0)
		, height(0)
	{
	}

	virtual ~RenderTarget2D()
	{
	}

	bool Create(unsigned int width, unsigned int height, unsigned int nrComponents, bool isHDR)
	{
		this->width = width;
		this->height = height;

		SetFormat(nrComponents, isHDR);

		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexImage2D(GL_TEXTURE_2D, 0, (GLint)internalformat, width, height, 0, (GLint)format, (GLint)pixelFormat, nullptr);
		glGenerateMipmap(GL_TEXTURE_2D);

		warpS = GL_CLAMP_TO_EDGE;
		warpT = GL_CLAMP_TO_EDGE;
		warpR = GL_CLAMP_TO_EDGE;

		return RenderTarget::Create();
	}

	void Destroy()
	{
		RenderTarget::Destroy();
	}

	virtual void GetResolution(vec3& resolution)
	{
		resolution = vec3(width, height, 0.0);
	}
private:
private:
	unsigned int width;
	unsigned int height;
};

class RenderTargetCubeMap : public RenderTarget
{
public:
	RenderTargetCubeMap()
		: RenderTarget(GL_TEXTURE_CUBE_MAP)
		, size(0)
	{
	}

	virtual ~RenderTargetCubeMap()
	{
	}

	bool Create(unsigned int size, unsigned int nrComponents, bool isHDR)
	{
		this->size = size;
		SetFormat(nrComponents, isHDR);

		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
		int dataSize;
		if (isHDR)
			dataSize = size * size * nrComponents * sizeof(float);
		else
			dataSize = size * size * nrComponents * sizeof(unsigned char);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, (GLint)internalformat, size, size, 0, (GLint)format, (GLint)pixelFormat, nullptr);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, (GLint)internalformat, size, size, 0, (GLint)format, (GLint)pixelFormat, nullptr);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, (GLint)internalformat, size, size, 0, (GLint)format, (GLint)pixelFormat, nullptr);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, (GLint)internalformat, size, size, 0, (GLint)format, (GLint)pixelFormat, nullptr);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, (GLint)internalformat, size, size, 0, (GLint)format, (GLint)pixelFormat, nullptr);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, (GLint)internalformat, size, size, 0, (GLint)format, (GLint)pixelFormat, nullptr);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		warpS = GL_CLAMP_TO_EDGE;
		warpT = GL_CLAMP_TO_EDGE;
		warpR = GL_CLAMP_TO_EDGE;

		return RenderTarget::Create();
	}

	void Destroy()
	{
		RenderTarget::Destroy();
	}

	virtual void GetResolution(vec3& resolution)
	{
		resolution = vec3(size, size, size);
	}
private:
private:
	unsigned int size;
};

#endif