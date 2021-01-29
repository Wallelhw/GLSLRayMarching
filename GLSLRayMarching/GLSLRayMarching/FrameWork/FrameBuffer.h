#ifndef _FrameBuffer_h_
#define _FrameBuffer_h_

#include "Platform.h"

#include "Texture.h"

class FrameBuffer
{
public:
	enum class Attachment
	{
		COLOR_ATTACHMENT0 = 0,
		COLOR_ATTACHMENT1,
		COLOR_ATTACHMENT2,
		COLOR_ATTACHMENT3,
		COLOR_ATTACHMENT4,
		COLOR_ATTACHMENT5,
		COLOR_ATTACHMENT6,
		COLOR_ATTACHMENT7,
		COLOR_ATTACHMENT8,
		COLOR_ATTACHMENT9,
		COLOR_ATTACHMENT10,
		COLOR_ATTACHMENT11,
		COLOR_ATTACHMENT12,
		COLOR_ATTACHMENT13,
		COLOR_ATTACHMENT14,
		COLOR_ATTACHMENT15,
		COLOR_ATTACHMENT16,
		COLOR_ATTACHMENT17,
		COLOR_ATTACHMENT18,
		COLOR_ATTACHMENT19,
		COLOR_ATTACHMENT20,
		COLOR_ATTACHMENT21,
		COLOR_ATTACHMENT22,
		COLOR_ATTACHMENT23,
		COLOR_ATTACHMENT24,
		COLOR_ATTACHMENT25,
		COLOR_ATTACHMENT26,
		COLOR_ATTACHMENT27,
		COLOR_ATTACHMENT28,
		COLOR_ATTACHMENT29,
		COLOR_ATTACHMENT30,
		COLOR_ATTACHMENT31
//		DEPTH_ATTACHMENT,
//		STENCIL_ATTACHMENT
	};

	FrameBuffer();
	virtual ~FrameBuffer();
	
	virtual bool Create();
	virtual void Destroy();
	
	virtual void Bind();
	static void UnBind();

	virtual void SetColorAttachment(FrameBuffer::Attachment attachment, Texture* texture);
	virtual void SetDepthAttachment(Texture* texture);
	virtual void SetStencilAttachment(Texture* texture);
	
	virtual Texture* GetColorAttachment(FrameBuffer::Attachment attachment);
	virtual Texture* GetDepthAttachment();
	virtual Texture* GetStencilAttachment();
private:
private:
	unsigned int fbo;
	std::map<FrameBuffer::Attachment, Texture*> colorAttachments;
	Texture* depthAttachment;
	Texture* stencilAttachment;
};

class Texture2DFrameBuffer : public FrameBuffer
{
public:
	Texture2DFrameBuffer();
	virtual ~Texture2DFrameBuffer();

	virtual bool Create(unsigned int width, unsigned int height, unsigned int nrComponents, bool isHDR);
	virtual void Destroy();
	
	Texture* GetTexture();
private:
	Texture2D texture;
};

class TextureCubeMapFrameBuffer : public FrameBuffer
{
public:
	TextureCubeMapFrameBuffer();
	virtual ~TextureCubeMapFrameBuffer();

	virtual bool Create(unsigned int size, unsigned int nrComponents, bool isHDR);
	virtual void Destroy();

	Texture* GetTexture();
private:
	TextureCubeMap texture;
};

#endif