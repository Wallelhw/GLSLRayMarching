#ifndef _FrameBuffer_h_
#define _FrameBuffer_h_

#include "Platform.h"

#include "Texture.h"

class FrameBufferImpl;

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

	enum PixelStorage
	{
		Store = 0,
		DontCare
	};

	FrameBuffer();
	virtual ~FrameBuffer();
	
	virtual bool Create();
	virtual void Destroy();
	
	bool Bind();
	void UnBind();

	void SetColorAttachment(FrameBuffer::Attachment attachment_, Texture* texture_, PixelStorage pixelStorage_ = FrameBuffer::PixelStorage::Store);
	void SetDepthAttachment(Texture* texture_, PixelStorage pixelStorage_ = FrameBuffer::PixelStorage::Store);
	void SetStencilAttachment(Texture* texture_, PixelStorage pixelStorage_ = FrameBuffer::PixelStorage::Store);
	void SetDepthStencilAttachment(Texture* texture_, PixelStorage pixelStorage_ = FrameBuffer::PixelStorage::Store);
	void Invalidate(int x=-1, int y = -1, int w = -1, int h = -1) const;

	virtual const Texture* GetColorAttachment(FrameBuffer::Attachment attachment) const;
	virtual const Texture* GetDepthAttachment() const;
	virtual const Texture* GetStencilAttachment() const;
	virtual const Texture* GetDepthStencilAttachment() const;
protected:
	bool IsframeBufferComplete() const;
private:
private:
	FrameBufferImpl* impl;
};

class Texture2DFrameBuffer : public FrameBuffer
{
public:
	Texture2DFrameBuffer();
	virtual ~Texture2DFrameBuffer();

	virtual bool Create(unsigned int width, unsigned int height, unsigned int nrComponents, bool isHDR);
	virtual void Destroy();
	
	Texture* GetTexture();
	const Texture* GetTexture() const;
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
	const Texture* GetTexture() const;
private:
	TextureCubeMap texture;
};

#endif