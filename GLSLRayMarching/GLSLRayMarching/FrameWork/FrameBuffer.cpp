#include "Platform.h"
#include "FrameBuffer.h"
#include "Graphics.h"
#include "Texture.h"
#include <map>

static unsigned int texGLAttachments[] =
{
	GL_COLOR_ATTACHMENT0,
	GL_COLOR_ATTACHMENT1,
	GL_COLOR_ATTACHMENT2,
	GL_COLOR_ATTACHMENT3,
	GL_COLOR_ATTACHMENT4,
	GL_COLOR_ATTACHMENT5,
	GL_COLOR_ATTACHMENT6,
	GL_COLOR_ATTACHMENT7,
	GL_COLOR_ATTACHMENT8,
	GL_COLOR_ATTACHMENT9,
	GL_COLOR_ATTACHMENT10,
	GL_COLOR_ATTACHMENT11,
	GL_COLOR_ATTACHMENT12,
	GL_COLOR_ATTACHMENT13,
	GL_COLOR_ATTACHMENT14,
	GL_COLOR_ATTACHMENT15,
	GL_COLOR_ATTACHMENT16,
	GL_COLOR_ATTACHMENT17,
	GL_COLOR_ATTACHMENT18,
	GL_COLOR_ATTACHMENT19,
	GL_COLOR_ATTACHMENT20,
	GL_COLOR_ATTACHMENT21,
	GL_COLOR_ATTACHMENT22,
	GL_COLOR_ATTACHMENT23,
	GL_COLOR_ATTACHMENT24,
	GL_COLOR_ATTACHMENT25,
	GL_COLOR_ATTACHMENT26,
	GL_COLOR_ATTACHMENT27,
	GL_COLOR_ATTACHMENT28,
	GL_COLOR_ATTACHMENT29,
	GL_COLOR_ATTACHMENT30,
	GL_COLOR_ATTACHMENT31,
	GL_DEPTH_ATTACHMENT,
	GL_STENCIL_ATTACHMENT
};

FrameBuffer::FrameBuffer()
	: fbo(0)
	, colorAttachments()
	, depthAttachment(nullptr)
	, stencilAttachment(nullptr)
{
}

FrameBuffer::~FrameBuffer()
{
}

bool FrameBuffer::Create()
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	return fbo != 0;
}

void FrameBuffer::Destroy()
{
	if (fbo)
	{
		glDeleteFramebuffers(1, &fbo);
		fbo = 0;
	}
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	///////////////////////////////////////////////////////////////
	for (auto& colorAttachment : colorAttachments)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, texGLAttachments[(int)colorAttachment.first], GL_TEXTURE_2D, colorAttachment.second->GetHandle(), 0);
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

void FrameBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::SetColorAttachment(FrameBuffer::Attachment attachment, Texture* texture)
{
	colorAttachments[attachment] = texture;
}

void FrameBuffer::SetDepthAttachment(Texture* texture)
{
	depthAttachment = texture;
}

void FrameBuffer::SetStencilAttachment(Texture* texture)
{
	stencilAttachment = texture;
}

Texture* FrameBuffer::GetColorAttachment(FrameBuffer::Attachment attachment)
{
	std::map<FrameBuffer::Attachment, Texture*>::iterator itr = colorAttachments.find(attachment);
	if (itr != colorAttachments.end())
		return colorAttachments[attachment];
	else
		return nullptr;
}

Texture* FrameBuffer::GetDepthAttachment()
{
	return depthAttachment;
}

Texture* FrameBuffer::GetStencilAttachment()
{
	return stencilAttachment;
}

////////////////////////////////////////////////////////////
Texture2DFrameBuffer::Texture2DFrameBuffer()
	: FrameBuffer()
{
}

Texture2DFrameBuffer::~Texture2DFrameBuffer()
{
}

bool Texture2DFrameBuffer::Create(unsigned int width, unsigned int height, unsigned int nrComponents, bool isHDR)
{
	if (!FrameBuffer::Create())
		return false;

	if (!texture.Create(width, height, nrComponents, isHDR, nullptr))
		return false;

	SetColorAttachment(FrameBuffer::Attachment::COLOR_ATTACHMENT0, &texture);

	return true;
}

void Texture2DFrameBuffer::Destroy()
{
	texture.Destroy();

	return FrameBuffer::Destroy();
}

Texture* Texture2DFrameBuffer::GetTexture()
{
	return &texture;
}


//////////////////////////////////////////////////////////
TextureCubeMapFrameBuffer::TextureCubeMapFrameBuffer()
	: FrameBuffer()
{
}

TextureCubeMapFrameBuffer::~TextureCubeMapFrameBuffer()
{
}

bool TextureCubeMapFrameBuffer::Create(unsigned int size, unsigned int nrComponents, bool isHDR)
{
	if (!FrameBuffer::Create())
		return false;

	if (!texture.Create(size, nrComponents, isHDR, nullptr))
		return false;

	SetColorAttachment(FrameBuffer::Attachment::COLOR_ATTACHMENT0, &texture);

	return true;
}

void TextureCubeMapFrameBuffer::Destroy()
{
	texture.Destroy();

	return FrameBuffer::Destroy();
}

Texture* TextureCubeMapFrameBuffer::GetTexture()
{
	return &texture;
}