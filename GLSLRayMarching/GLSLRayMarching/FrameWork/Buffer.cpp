#include "Platform.h"
#include "Buffer.h"
#include "Graphics.h"

//////////////////////////////////////////////////////////////////////////////////
static int buffer_type_GL[] =
{
	GL_ARRAY_BUFFER,				// Vertex attributes.
	GL_ATOMIC_COUNTER_BUFFER,		// Atomic counter storage
	GL_COPY_READ_BUFFER,			// Buffer copy source
	GL_COPY_WRITE_BUFFER,			// Buffer copy destination
	GL_DISPATCH_INDIRECT_BUFFER,	// Indirect compute dispatch commands
	GL_DRAW_INDIRECT_BUFFER,		// Indirect command arguments
	GL_ELEMENT_ARRAY_BUFFER,		// Vertex array indices.
	GL_PIXEL_PACK_BUFFER,			// Pixel read target.
	GL_PIXEL_UNPACK_BUFFER,			// Texture data source.
	GL_QUERY_BUFFER,				// Query result buffer
	GL_SHADER_STORAGE_BUFFER,		// Read - write storage for shaders.
	GL_TEXTURE_BUFFER,				// Texture data buffer.
	GL_TRANSFORM_FEEDBACK_BUFFER,	// Transform feedback buffer
	GL_UNIFORM_BUFFER				// Uniform Buffer
};

static int buffer_type1_GL[] =
{
	GL_UNIFORM,
	GL_UNIFORM_BLOCK,
	GL_PROGRAM_INPUT,
	GL_PROGRAM_OUTPUT,
	GL_VERTEX_SUBROUTINE,
	GL_TESS_CONTROL_SUBROUTINE,
	GL_TESS_EVALUATION_SUBROUTINE,
	GL_GEOMETRY_SUBROUTINE,
	GL_FRAGMENT_SUBROUTINE,
	GL_COMPUTE_SUBROUTINE,
	GL_VERTEX_SUBROUTINE_UNIFORM,
	GL_TESS_CONTROL_SUBROUTINE_UNIFORM,
	GL_TESS_EVALUATION_SUBROUTINE_UNIFORM,
	GL_GEOMETRY_SUBROUTINE_UNIFORM,
	GL_FRAGMENT_SUBROUTINE_UNIFORM,
	GL_COMPUTE_SUBROUTINE_UNIFORM,
	GL_TRANSFORM_FEEDBACK_VARYING,
	GL_TRANSFORM_FEEDBACK_BUFFER,
	GL_BUFFER_VARIABLE,
	GL_SHADER_STORAGE_BLOCK
};

static int buffer_usage_GL[] =
{
	GL_STREAM_DRAW,
	GL_STREAM_READ,
	GL_STREAM_COPY,

	GL_STATIC_DRAW,
	GL_STATIC_READ,
	GL_STATIC_COPY,

	GL_DYNAMIC_DRAW,
	GL_DYNAMIC_READ,
	GL_DYNAMIC_COPY
};

class BufferImpl
{
public:
	BufferImpl()
	: handle(0)
	, type(Buffer::Type::ARRAY_BUFFER)
	, usage(Buffer::Usage::STATIC_DRAW)
	{
	}

	void Clear()
	{
		Platform::MemSet(this, 0, sizeof(*this));
	}

	Buffer::Type type;
	Buffer::Usage usage;
	unsigned int size;
	unsigned int handle;
};

Buffer::Buffer()
: impl(0)
{
	{
		impl = new BufferImpl();
		Assert(impl);
	}
}

Buffer::~Buffer()
{
	Assert(impl);

	Destroy();

	if (impl)
	{
		delete impl;
		impl = nullptr;
	}
}

Buffer::Type Buffer::GetType() const
{
	Assert(impl);

	return impl->type;
}

Buffer::Usage Buffer::GetUsage() const
{
	Assert(impl);

	return impl->usage;
}

unsigned int Buffer::GetHandle() const
{
	Assert(impl);

	return impl->handle;
}

Buffer& Buffer::Begin(Buffer::Type type_, Buffer::Usage usage_)
{
	Assert(impl);

	Destroy();

	impl->type = type_;
	impl->usage = usage_;

	glGenBuffers(1, &impl->handle);
	Assert(impl->handle);

	return *this;
}

Buffer& Buffer::Fill(void* data_, int dataSize_)
{
	Assert(impl);

	/*
	int err = 0;
	glBindBuffer(buffer_type_GL[(int)type], handle);
	err = glGetError();
	void* data = glMapBuffer(buffer_type_GL[(int)type], GL_READ_WRITE);
	err = glGetError();
	memcpy(data, data_, dataSize_);
	glUnmapBuffer(buffer_type_GL[(int)type]);
	err = glGetError();
	*/

	glBindBuffer(buffer_type_GL[(int)impl->type], impl->handle);
	glBufferData(buffer_type_GL[(int)impl->type], dataSize_, data_, buffer_usage_GL[(int)impl->usage]);

	impl->size = dataSize_;
	return *this;
}

Buffer& Buffer::Update(int offset_, void* data_, int dataSize_)
{
	Assert(impl);

	glBindBuffer(buffer_type_GL[(int)impl->type], impl->handle);
	glBufferSubData(buffer_type_GL[(int)impl->type], offset_, dataSize_, data_);

	return *this;
}

bool Buffer::End()
{
	Assert(impl);

	glBindBuffer(buffer_type_GL[(int)impl->type], 0);

	return impl->handle!=0;
}

void Buffer::Destroy()
{
	Assert(impl);

	if (impl->handle)
	{
		glDeleteBuffers(1, &impl->handle);

		impl->Clear();
	}
}

void Buffer::Bind() const
{
	Assert(impl);
	Assert(impl->handle);

	glBindBuffer(buffer_type_GL[(int)impl->type], impl->handle);
}

void Buffer::Unbind() const
{
	Assert(impl);

	glBindBuffer(buffer_type_GL[(int)impl->type], 0);
}

void Buffer::BindShaderStorage(ShaderProgram& shaderProgram_, /*const char* name_, */unsigned int bindingPoint_, unsigned int offset_, unsigned int size_)
{
	Assert(impl);
	Assert(impl->type == Type::SHADER_STORAGE_BUFFER);

	//unsigned int block_index = glGetProgramResourceIndex(shaderProgram_.GetHandle(), GL_SHADER_STORAGE_BLOCK, name_);
	//glShaderStorageBlockBinding(shaderProgram_.GetHandle(), block_index, bindingPoint_);

	if(size_==0)
		glBindBufferBase(buffer_type_GL[(int)impl->type], bindingPoint_, impl->handle);
	else
		glBindBufferRange(buffer_type_GL[(int)impl->type], bindingPoint_, impl->handle, offset_, size_);
}

void Copy(Buffer& src_, unsigned int srcOffset_, Buffer& dst_, unsigned int dstOffset_, unsigned int size_)
{
	Assert(src_.impl);
	Assert(dst_.impl);

	glCopyBufferSubData(buffer_type_GL[(int)(src_.impl->type)],
						buffer_type_GL[(int)(dst_.impl->type)],
						srcOffset_,
						dstOffset_,
						size_);
}