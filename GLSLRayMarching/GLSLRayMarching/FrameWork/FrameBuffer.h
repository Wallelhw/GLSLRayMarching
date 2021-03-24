#ifndef _FrameBuffer_h_
#define _FrameBuffer_h_

#include "Platform.h"

#include "Texture.h"
#include "Rect2.h"
#include "ColorRGBA.h"

class FrameBufferImpl;

class FrameBuffer
{
public:
	enum class ColorAttachment
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
		COLOR_ATTACHMENT31,
//		DEPTH_ATTACHMENT,
//		STENCIL_ATTACHMENT
	};

	enum PixelStorage
	{
		Store = 0,
		DontCare
	};

	enum BlitMask
	{
		COLOR_BUFFER_BIT = 0x01, 
		DEPTH_BUFFER_BIT = 0x02,
		STENCIL_BUFFER_BIT = 0x04
	};

	FrameBuffer();
	virtual ~FrameBuffer();
	
	virtual bool Initiate();
	virtual void Terminate();
	
	friend void Copy(FrameBuffer& src_, const Rect2& srcRect_, FrameBuffer& dst_, const Rect2& dstRect_, BlitMask blitMask_, Texture::MagFilter filter);

	bool Bind();
	void UnBind();

	void SetColorAttachment(FrameBuffer::ColorAttachment colorAttachment_, Texture* texture_, PixelStorage pixelStorage_ = FrameBuffer::PixelStorage::Store);
	void SetDepthAttachment(Texture* texture_, PixelStorage pixelStorage_ = FrameBuffer::PixelStorage::Store);
	void SetStencilAttachment(Texture* texture_, PixelStorage pixelStorage_ = FrameBuffer::PixelStorage::Store);
	void SetDepthStencilAttachment(Texture* texture_, PixelStorage pixelStorage_ = FrameBuffer::PixelStorage::Store);
	const Texture* GetColorAttachment(FrameBuffer::ColorAttachment colorAttachment_) const;
	const Texture* GetDepthAttachment() const;
	const Texture* GetStencilAttachment() const;
	const Texture* GetDepthStencilAttachment() const;
	void ClearColorAttachment(FrameBuffer::ColorAttachment colorAttachment_, const ColorRGBA& color_);
	void ClearDepthAttachment(float clearDepth_);
	void ClearStencilAttachment(int clearStencil_);
	void ClearDepthStencilAttachment(float clearDepth_, int clearStencil_);

	void Invalidate(int x = -1, int y = -1, int w = -1, int h = -1) const;

	static void EnableDrawBuffers(std::vector<FrameBuffer::ColorAttachment> attachments_);
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

	virtual bool Initiate(unsigned int width, unsigned int height, unsigned int nrComponents, Texture::DynamicRange dynamicRange_);
	virtual void Terminate();
	
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

	virtual bool Initiate(unsigned int size, unsigned int nrComponents, Texture::DynamicRange dynamicRange_);
	virtual void Terminate();

	Texture* GetTexture();
	const Texture* GetTexture() const;
private:
	TextureCubeMap texture;
};

#endif