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
	//GL_DEPTH_ATTACHMENT,
	//GL_STENCIL_ATTACHMENT
};

/*
////////////////////////////////////////////
Frame Buffer
2.0
// glGenFramebuffers
// glDeleteFramebuffers
// glBindFramebuffer
// glIsFramebuffer
// glCheckFramebufferStatus
// glFramebufferTexture2D
glBindRenderbuffer
glFramebufferRenderbuffer
3.0
glDrawBuffers
// glFramebufferTextureLayer
// glInvalidateFramebuffer
// glInvalidateSubFramebuffer
glBlitFramebuffer
3.1
glFramebufferParameteri
3.2
//glFramebufferTexture
getter
glGetFramebufferAttachmentParameteriv
glGetFramebufferParameteriv
*/

class FrameBufferImpl
{
public:
	FrameBufferImpl()
		: fbo(0)
		, colorAttachments()
		, depthAttachment()
		, stencilAttachment()
	{
	}

	class TextureAndDirty
	{
	public:
		TextureAndDirty()
			: texture(nullptr)
			, dirty(true)
			, pixelStorage(FrameBuffer::PixelStorage::Store)
		{
		}

		Texture* texture;
		bool dirty;
		FrameBuffer::PixelStorage pixelStorage;
	};

	unsigned int fbo;
	std::map<FrameBuffer::Attachment, TextureAndDirty> colorAttachments;
	TextureAndDirty depthAttachment;
	TextureAndDirty stencilAttachment;
	TextureAndDirty depthStencilAttachment;
};

FrameBuffer::FrameBuffer()
{
	impl = new FrameBufferImpl();
	Assert(impl);
}

FrameBuffer::~FrameBuffer()
{
	Assert(impl);

	Destroy();

	if (impl)
	{
		delete impl;
		impl = nullptr;
	}
}

bool FrameBuffer::Create()
{
	Assert(impl);

	glGenFramebuffers(1, &impl->fbo);
	//glBindFramebuffer(GL_FRAMEBUFFER, impl->fbo);

	return impl->fbo != 0;
}

void FrameBuffer::Destroy()
{
	Assert(impl);

	if (impl->fbo)
	{
		impl->colorAttachments.clear();
		impl->depthAttachment = FrameBufferImpl::TextureAndDirty();
		impl->stencilAttachment = FrameBufferImpl::TextureAndDirty();
		impl->depthStencilAttachment = FrameBufferImpl::TextureAndDirty();

		glDeleteFramebuffers(1, &impl->fbo);
		impl->fbo = 0;
	}
}

bool FrameBuffer::Bind()
{
	Assert(impl);

	glBindFramebuffer(GL_FRAMEBUFFER, impl->fbo);

	///////////////////////////////////////////////////////////////
	for (auto& attachmentmentItr : impl->colorAttachments)
	{
		FrameBufferImpl::TextureAndDirty& attachment = attachmentmentItr.second;

		if (attachment.dirty)
		{
			if (attachment.texture)
			{
				glFramebufferTexture2D(GL_FRAMEBUFFER, texGLAttachments[(int)attachmentmentItr.first], GL_TEXTURE_2D, attachment.texture->GetHandle(), 0);
				attachment.dirty = false;

				return IsframeBufferComplete();
			}
		}
	}

	{
		FrameBufferImpl::TextureAndDirty& attachment = impl->depthAttachment;

		if (attachment.dirty)
		{
			if (attachment.texture)
			{
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, attachment.texture->GetHandle(), 0);
				attachment.dirty = false;

				return IsframeBufferComplete();
			}
		}
	}

	{
		FrameBufferImpl::TextureAndDirty& attachment = impl->stencilAttachment;

		if (attachment.dirty)
		{
			if (attachment.texture)
			{
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, attachment.texture->GetHandle(), 0);
				attachment.dirty = false;

				return IsframeBufferComplete();
			}
		}
	}

	{
		FrameBufferImpl::TextureAndDirty& attachment = impl->depthStencilAttachment;

		if (attachment.dirty)
		{
			if (attachment.texture)
			{
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, attachment.texture->GetHandle(), 0);
				attachment.dirty = false;

				return IsframeBufferComplete();
			}
		}
	}

	return true;
}

void FrameBuffer::UnBind()
{
	Assert(impl);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Invalidate(int x, int y, int w, int h) const
{
	Assert(impl);

	std::vector<unsigned int > attachmentEnums;
	///////////////////////////////////////////////////////////////
	for (auto& attachmentmentItr : impl->colorAttachments)
	{
		FrameBufferImpl::TextureAndDirty& attachment = attachmentmentItr.second;

		attachmentEnums.push_back((int)attachmentmentItr.first);
	}

	{
		FrameBufferImpl::TextureAndDirty& attachment = impl->depthAttachment;
		if (attachment.texture)
		{
			attachmentEnums.push_back(GL_DEPTH_ATTACHMENT);
		}
	}

	{
		FrameBufferImpl::TextureAndDirty& attachment = impl->depthAttachment;
		if (attachment.texture)
		{
			attachmentEnums.push_back(GL_STENCIL_ATTACHMENT);
		}
	}

	if (x < 0 || y < 0 || w < 0 || h < 0)
		glInvalidateSubFramebuffer(GL_FRAMEBUFFER, attachmentEnums.size(), &attachmentEnums[0], x, y, w, h);
	else
		glInvalidateFramebuffer(GL_FRAMEBUFFER, attachmentEnums.size(), &attachmentEnums[0]);
}

void FrameBuffer::SetColorAttachment(FrameBuffer::Attachment attachment_, Texture* texture_, PixelStorage pixelStorage_)
{
	Assert(impl);

	impl->colorAttachments[attachment_].texture = texture_;
	impl->colorAttachments[attachment_].dirty = true;
	impl->colorAttachments[attachment_].pixelStorage = pixelStorage_;
}

void FrameBuffer::SetDepthAttachment(Texture* texture_, PixelStorage pixelStorage_)
{
	Assert(impl);

	impl->depthAttachment.texture = texture_;
	impl->depthAttachment.dirty = true;
	impl->depthAttachment.pixelStorage = pixelStorage_;
}

void FrameBuffer::SetStencilAttachment(Texture* texture_, PixelStorage pixelStorage_)
{
	Assert(impl);

	impl->stencilAttachment.texture = texture_;
	impl->stencilAttachment.dirty = true;
	impl->depthAttachment.pixelStorage = pixelStorage_;
}

void FrameBuffer::SetDepthStencilAttachment(Texture* texture_, PixelStorage pixelStorage_)
{
	Assert(impl);

	impl->depthStencilAttachment.texture = texture_;
	impl->depthStencilAttachment.dirty = true;
	impl->depthStencilAttachment.pixelStorage = pixelStorage_;
}

const Texture* FrameBuffer::GetColorAttachment(FrameBuffer::Attachment attachment) const
{
	Assert(impl);

	std::map<FrameBuffer::Attachment, FrameBufferImpl::TextureAndDirty>::const_iterator
		itr = impl->colorAttachments.find(attachment);

	if (itr != impl->colorAttachments.end())
		return itr->second.texture;
	else
		return nullptr;
}

const Texture* FrameBuffer::GetDepthAttachment() const
{
	Assert(impl);

	return impl->depthAttachment.texture;
}

const Texture* FrameBuffer::GetStencilAttachment() const
{
	Assert(impl);

	return impl->stencilAttachment.texture;
}

const Texture* FrameBuffer::GetDepthStencilAttachment() const
{
	Assert(impl);

	return impl->depthStencilAttachment.texture;
}

bool FrameBuffer::IsframeBufferComplete() const
{
	Assert(impl);

	return glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE;
}

////////////////////////////////////////////////////////////
Texture2DFrameBuffer::Texture2DFrameBuffer()
	: FrameBuffer()
{
}

Texture2DFrameBuffer::~Texture2DFrameBuffer()
{
	Destroy();
}

bool Texture2DFrameBuffer::Create(unsigned int width, unsigned int height, unsigned int nrComponents, bool isHDR)
{
	if (!FrameBuffer::Create())
		return false;

	if (!texture.Create(width, height, nrComponents, isHDR, nullptr))
		return false;

	SetColorAttachment(FrameBuffer::Attachment::COLOR_ATTACHMENT0, &texture, FrameBuffer::PixelStorage::Store);

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

const Texture* Texture2DFrameBuffer::GetTexture() const
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
	Destroy();
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

const Texture* TextureCubeMapFrameBuffer::GetTexture() const
{
	return &texture;
}