#include "Platform.h"
#include "FrameBuffer.h"
#include "Graphics.h"
#include "Texture.h"
#include <map>

static unsigned int frameBufferGLColorAttachments[] =
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

	void Clear()
	{
		fbo = 0;
		colorAttachments.clear();
		depthAttachment = FrameBufferImpl::TextureAndDirty();
		stencilAttachment = FrameBufferImpl::TextureAndDirty();
		depthStencilAttachment = FrameBufferImpl::TextureAndDirty();
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
	std::map<FrameBuffer::ColorAttachment, TextureAndDirty> colorAttachments;
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
		glDeleteFramebuffers(1, &impl->fbo);

		impl->Clear();
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
				glFramebufferTexture2D(GL_FRAMEBUFFER,
					frameBufferGLColorAttachments[(int)attachmentmentItr.first],
					GL_TEXTURE_2D,
					attachment.texture->GetHandle(),
					0);
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

static unsigned int frameBuffer_GLMagFilters[] =
{
	GL_NEAREST,
	GL_LINEAR,
};

void Copy(FrameBuffer& src_, const Rect2& srcRect_, FrameBuffer& dst_, const Rect2& dstRect_, FrameBuffer::BlitMask blitMask_, Texture::MagFilter filter_)
{
	Assert(src_.impl);
	Assert(dst_.impl);

	unsigned int mask = 0;
	mask |= (blitMask_ & FrameBuffer::BlitMask::COLOR_BUFFER_BIT) ? GL_COLOR_BUFFER_BIT : 0;
	mask |= (blitMask_ & FrameBuffer::BlitMask::DEPTH_BUFFER_BIT) ? GL_DEPTH_BUFFER_BIT : 0;
	mask |= (blitMask_ & FrameBuffer::BlitMask::STENCIL_BUFFER_BIT) ? GL_STENCIL_BUFFER_BIT : 0;

	glBlitFramebuffer
	(
		srcRect_.X(),
		srcRect_.Y(),
		srcRect_.X() + srcRect_.Width(),
		srcRect_.Y() + srcRect_.Height(),

		dstRect_.X(),
		dstRect_.Y(),
		dstRect_.X() + dstRect_.Width(),
		dstRect_.Y() + dstRect_.Height(),

		mask,
		frameBuffer_GLMagFilters[(int)filter_]
	);
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

void FrameBuffer::SetColorAttachment(FrameBuffer::ColorAttachment colorAttachment_, Texture* texture_, PixelStorage pixelStorage_)
{
	Assert(impl);

	impl->colorAttachments[colorAttachment_].texture = texture_;
	impl->colorAttachments[colorAttachment_].dirty = true;
	impl->colorAttachments[colorAttachment_].pixelStorage = pixelStorage_;
}

void FrameBuffer::SetDepthAttachment(Texture* texture_, PixelStorage pixelStorage_)
{
	Assert(impl);
	Assert(texture_);

	impl->depthAttachment.texture = texture_;
	impl->depthAttachment.dirty = true;
	impl->depthAttachment.pixelStorage = pixelStorage_;
}

void FrameBuffer::SetStencilAttachment(Texture* texture_, PixelStorage pixelStorage_)
{
	Assert(impl);
	Assert(texture_);

	impl->stencilAttachment.texture = texture_;
	impl->stencilAttachment.dirty = true;
	impl->depthAttachment.pixelStorage = pixelStorage_;
}

void FrameBuffer::SetDepthStencilAttachment(Texture* texture_, PixelStorage pixelStorage_)
{
	Assert(impl);
	Assert(texture_);

	impl->depthStencilAttachment.texture = texture_;
	impl->depthStencilAttachment.dirty = true;
	impl->depthStencilAttachment.pixelStorage = pixelStorage_;
}

const Texture* FrameBuffer::GetColorAttachment(FrameBuffer::ColorAttachment colorAttachment_) const
{
	Assert(impl);

	std::map<FrameBuffer::ColorAttachment, FrameBufferImpl::TextureAndDirty>::const_iterator
		itr = impl->colorAttachments.find(colorAttachment_);

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

void FrameBuffer::ClearColorAttachment(FrameBuffer::ColorAttachment colorAttachment_, const ColorRGBA& color_)
{
	Assert(impl);

	int c[4] =
	{
		color_[0] * 255,
		color_[1] * 255,
		color_[2] * 255,
		color_[3] * 255,
	};

	bool attachmentIsFloat = false;
	if(attachmentIsFloat)
		glClearBufferfv(GL_COLOR, GL_DRAW_BUFFER0, &color_[0]);
	else
		glClearBufferiv(GL_COLOR, GL_DRAW_BUFFER0, c);
}

void FrameBuffer::ClearDepthAttachment(float clearDepth_)
{
	Assert(impl);

	glClearBufferfv(GL_DEPTH, 0, &clearDepth_);
}

void FrameBuffer::ClearStencilAttachment(int clearStencil_)
{
	Assert(impl);
	
	glClearBufferiv(GL_COLOR, 0, &clearStencil_);
}

void FrameBuffer::ClearDepthStencilAttachment(float clearDepth_, int clearStencil_)
{
	Assert(impl);

	glClearBufferfi(GL_DEPTH_STENCIL, 0, clearDepth_, clearStencil_);
}

void FrameBuffer::EnableDrawBuffers(std::vector<FrameBuffer::ColorAttachment> attachments_)
{
	std::vector<unsigned int> bufs(attachments_.size());
	for (int i = 0; i < attachments_.size(); i++)
	{
		bufs[i] = frameBufferGLColorAttachments[(int)attachments_[i]];
	}

	glDrawBuffers(bufs.size(), &bufs[0]);
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

bool Texture2DFrameBuffer::Create(unsigned int width, unsigned int height, unsigned int nrComponents, Texture::Precision precision_)
{
	if (!FrameBuffer::Create())
		return false;

	if (!texture.Create(width, height, nrComponents, precision_, nullptr))
		return false;

	SetColorAttachment(FrameBuffer::ColorAttachment::COLOR_ATTACHMENT0, &texture, FrameBuffer::PixelStorage::Store);

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

bool TextureCubeMapFrameBuffer::Create(unsigned int size_, unsigned int nrComponents_, Texture::Precision precision_)
{
	if (!FrameBuffer::Create())
		return false;

	if (!texture.Create(size_, nrComponents_, precision_, nullptr))
		return false;

	SetColorAttachment(FrameBuffer::ColorAttachment::COLOR_ATTACHMENT0, &texture);

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