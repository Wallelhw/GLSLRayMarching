#ifndef _Buffer_h_
#define _Buffer_h_

#include "Platform.h"
#include "ShaderProgram.h"
#include <map>

class BufferImpl;

class Buffer
{
public:
	friend class ShaderProgram;

	enum class Type	
	{
		ARRAY_BUFFER = 0,			// Vertex attributes
		ATOMIC_COUNTER_BUFFER,		// Atomic counter storage
		COPY_READ_BUFFER,			// Buffer copy source
		COPY_WRITE_BUFFER,			// Buffer copy destination
		DISPATCH_INDIRECT_BUFFER,	// Indirect compute dispatch commands
		DRAW_INDIRECT_BUFFER,		// Indirect command arguments
		ELEMENT_ARRAY_BUFFER,		// Vertex array indices
		PIXEL_PACK_BUFFER,			// Pixel read target
		PIXEL_UNPACK_BUFFER,		// Texture data source
		QUERY_BUFFER,				// Query result buffer
		SHADER_STORAGE_BUFFER,		// Read - write storage for shaders
		TEXTURE_BUFFER,				// Texture data buffer
		TRANSFORM_FEEDBACK_BUFFER,	// Transform feedback buffer
		UNIFORM_BUFFER				// Uniform Buffer
	};

	enum class Usage
	{
		STREAM_DRAW = 0,  // The data store contents will be modified once and used at most a few times. e.g. billboard mesh, 
		STREAM_READ,
		STREAM_COPY,

		STATIC_DRAW,	  // The data store contents will be modified once and used many times. e.g. simple mesh data, simple texture
		STATIC_READ,
		STATIC_COPY,

		DYNAMIC_DRAW,	  // The data store contents will be modified repeatedly and used many times. e.g. sea mesh, video texture, 
		DYNAMIC_READ,
		DYNAMIC_COPY
	};

	Buffer();
	virtual ~Buffer();

	Buffer::Type GetType() const;
	Buffer::Usage GetUsage() const;
	unsigned int GetHandle() const;

	Buffer& Begin(Buffer::Type type_, Buffer::Usage usage_);
	Buffer& Fill(const void* src_, int size_);
	Buffer& Update(int offset_, const void* src_, int size_);
	bool End();	

	friend void Copy(Buffer& src_, unsigned int srcOffset_, Buffer& dst_, unsigned int dstOffset_, unsigned int size_);

	void Terminate();
	
	void Bind() const;
	void Unbind() const;


	void BindShaderStorage(ShaderProgram& shaderProgram_, const char* name_, unsigned int bindingPoint_, unsigned int offset_ = 0, unsigned int size_ = 0);
	void BindUniformBlock(ShaderProgram& shaderProgram_, const char* name_, unsigned int bindingPoint_, unsigned int offset_ = 0, unsigned int size_ = 0);
	void BindShaderStorage(ShaderProgram& shaderProgram_, unsigned int bindingPoint_, unsigned int offset_ = 0, unsigned int size_ = 0);
	void BindUniformBlock(ShaderProgram& shaderProgram_, unsigned int bindingPoint_, unsigned int offset_ = 0, unsigned int size_ = 0);
private:
private:
	BufferImpl* impl;
};

#endif