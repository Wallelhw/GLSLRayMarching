#ifndef _FrameBuffer_h_
#define _FrameBuffer_h_

#include "Texture.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <map>

/*
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_ATTACHMENT1 0x8CE1
#define GL_COLOR_ATTACHMENT2 0x8CE2
#define GL_COLOR_ATTACHMENT3 0x8CE3
#define GL_COLOR_ATTACHMENT4 0x8CE4
#define GL_COLOR_ATTACHMENT5 0x8CE5
#define GL_COLOR_ATTACHMENT6 0x8CE6
#define GL_COLOR_ATTACHMENT7 0x8CE7
#define GL_COLOR_ATTACHMENT8 0x8CE8
#define GL_COLOR_ATTACHMENT9 0x8CE9
#define GL_COLOR_ATTACHMENT10 0x8CEA
#define GL_COLOR_ATTACHMENT11 0x8CEB
#define GL_COLOR_ATTACHMENT12 0x8CEC
#define GL_COLOR_ATTACHMENT13 0x8CED
#define GL_COLOR_ATTACHMENT14 0x8CEE
#define GL_COLOR_ATTACHMENT15 0x8CEF
#define GL_COLOR_ATTACHMENT16 0x8CF0
#define GL_COLOR_ATTACHMENT17 0x8CF1
#define GL_COLOR_ATTACHMENT18 0x8CF2
#define GL_COLOR_ATTACHMENT19 0x8CF3
#define GL_COLOR_ATTACHMENT20 0x8CF4
#define GL_COLOR_ATTACHMENT21 0x8CF5
#define GL_COLOR_ATTACHMENT22 0x8CF6
#define GL_COLOR_ATTACHMENT23 0x8CF7
#define GL_COLOR_ATTACHMENT24 0x8CF8
#define GL_COLOR_ATTACHMENT25 0x8CF9
#define GL_COLOR_ATTACHMENT26 0x8CFA
#define GL_COLOR_ATTACHMENT27 0x8CFB
#define GL_COLOR_ATTACHMENT28 0x8CFC
#define GL_COLOR_ATTACHMENT29 0x8CFD
#define GL_COLOR_ATTACHMENT30 0x8CFE
#define GL_COLOR_ATTACHMENT31 0x8CFF
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_STENCIL_ATTACHMENT 0x8D20
*/

class FrameBuffer
{
public:
	FrameBuffer()
	: fbo(0)
	{
	}

	virtual ~FrameBuffer()
	{
	}

	virtual bool Create()
	{
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		if (!glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
			return false;

		return true;
	}

	virtual void Destroy()
	{
		if (fbo)
		{
			glDeleteFramebuffers(1, &fbo);
			fbo = 0;
		}
	}

	virtual void Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		///////////////////////////////////////////////////////////////
		for (auto& colorAttachment : colorAttachments)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachment.first, GL_TEXTURE_2D, colorAttachment.second->GetHandle(), 0);
		}

		if (depthAttachment)
		{
			//glGenRenderbuffers(1, &rbo);
			//glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
			//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
			//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				//return false;
		}
	}

	static void UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	virtual void SetColorAttachment(unsigned int attachmentID, Texture* texture)
	{
		colorAttachments[attachmentID] = texture;
	}

	virtual void SetDepthAttachment(Texture* texture)
	{
		depthAttachment = texture;
	}

	virtual void SetStencilAttachment(Texture* texture)
	{
		stencilAttachment = texture;
	}

	virtual Texture* GetColorAttachment(unsigned int attachmentID)
	{
		return colorAttachments[attachmentID];
	}

	virtual Texture* SetDepthAttachment()
	{
		return depthAttachment;
	}

	virtual Texture* SetStencilAttachment()
	{
		return stencilAttachment;
	}
private:
private:
	unsigned int fbo;
	std::map<unsigned int, Texture*> colorAttachments;
	Texture* depthAttachment;
	Texture* stencilAttachment;
};

class Texture2DFrameBuffer : public FrameBuffer
{
public:
	Texture2DFrameBuffer()
	: FrameBuffer()
	{
	}

	virtual ~Texture2DFrameBuffer()
	{
	}

	virtual bool Create(unsigned int width, unsigned int height, unsigned int nrComponents, bool isHDR)
	{
		if (!FrameBuffer::Create())
			return false;

		if (!texture.Create(width, height, nrComponents, isHDR, nullptr))
			return false;

		SetColorAttachment(GL_COLOR_ATTACHMENT0, &texture);
		return true;
	}

	virtual void Destroy()
	{
		texture.Destroy();

		return FrameBuffer::Destroy();
	}

	Texture* GetTexture()
	{
		return &texture;
	}
private:
	Texture2D texture;
};

class TextureCubeMapFrameBuffer : public FrameBuffer
{
public:
	TextureCubeMapFrameBuffer()
		: FrameBuffer()
	{
	}

	virtual ~TextureCubeMapFrameBuffer()
	{
	}

	virtual bool Create(unsigned int size, unsigned int nrComponents, bool isHDR)
	{
		if (!FrameBuffer::Create())
			return false;

		if (!texture.Create(size, nrComponents, isHDR, nullptr))
			return false;

		SetColorAttachment(GL_COLOR_ATTACHMENT0, &texture);
		return true;
	}

	virtual void Destroy()
	{
		texture.Destroy();

		return FrameBuffer::Destroy();
	}

	Texture* GetTexture()
	{
		return &texture;
	}
private:
	TextureCubeMap texture;
};

#endif