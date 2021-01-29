#include "Platform.h"
#include "Primitives.h"
#include "Graphics.h"

static int vaElementSizes[] =
{
	1, 1, 2, 2, 4, 4, 2, 4, 8, 4, 4, 4, 4
};

static int vaDataTypes[] =
{
	GL_BYTE,
	GL_UNSIGNED_BYTE,
	GL_SHORT,
	GL_UNSIGNED_SHORT,
	GL_INT,
	GL_UNSIGNED_INT,
	
	GL_HALF_FLOAT,
	GL_FLOAT,
	GL_DOUBLE,
	GL_FIXED,
	GL_INT_2_10_10_10_REV,
	GL_UNSIGNED_INT_2_10_10_10_REV,
	GL_UNSIGNED_INT_10F_11F_11F_REV
};

VertexAttribute::VertexAttribute()
: index(0)
, elementCount(3)
, dataType(DataType::FLOAT)
, normalized(true)
, stride(12)
, divisor(1)
, elementSize(0)
{
}

VertexAttribute::VertexAttribute(unsigned int index_, int elementCount_, VertexAttribute::DataType dataType_, bool normalized_, unsigned int divisor_, unsigned int stride_)
: index(index_)
, elementCount(elementCount_)
, dataType(dataType_)
, normalized(normalized_)
, stride(stride_)
, divisor(divisor_)
, elementSize(0)
{
	elementSize = vaElementSizes[(int)dataType];
	if (stride)
	{
		stride = elementCount * elementSize;
	}
}

//////////////////////////////////////////////////////////////////////////////////
static int primModes[] =
{
	GL_POINTS,
	GL_LINE_STRIP,
	GL_LINE_LOOP,
	GL_LINES,
	GL_LINE_STRIP_ADJACENCY,
	GL_LINES_ADJACENCY,
	GL_TRIANGLE_STRIP,
	GL_TRIANGLE_FAN,
	GL_TRIANGLES,
	GL_TRIANGLE_STRIP_ADJACENCY,
	GL_TRIANGLES_ADJACENCY,
	GL_PATCHES
};

Primitives::Primitives()
: initialized(false)
, vertexAttributes()
, VAO(0)
, VBO(0)
, EBO(0)
, verticesCount(0)
, indicesCount(0)
{
}

Primitives::~Primitives()
{
}

Primitives& Primitives::Begin()
{
	Destroy();

	glGenVertexArrays(1, &VAO);
	Assert(VAO);

	glBindVertexArray(VAO);

	return *this;	
}

Primitives& Primitives::FillVertices(unsigned int index_, int elementCount_, VertexAttribute::DataType type_, bool normalized_, unsigned int stride_, unsigned int divisor_, float* vertices_, int verticesCount_)
{
	glGenBuffers(1, &VBO);
	Assert(VBO);

	vertexAttributes.push_back(VertexAttribute(index_, elementCount_, type_, normalized_, divisor_, stride_));
	VertexAttribute& v = vertexAttributes.back();
		
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesCount_ * elementCount_* vaElementSizes[(int)v.dataType], vertices_, GL_STATIC_DRAW);
	glEnableVertexAttribArray(v.index);
	glVertexAttribPointer(v.index, v.elementCount, vaDataTypes[(int)v.dataType], v.normalized, v.elementCount * v.elementSize, (void*)0);
	glVertexAttribDivisor(v.index, v.divisor);

	initialized = true;
	verticesCount = verticesCount_;

	return *this;
}

Primitives& Primitives::FillIndices(unsigned int* indices_, int indicesCount_)
{
	glGenBuffers(1, &EBO);
	Assert(EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount_ * sizeof(int), indices_, GL_STATIC_DRAW);

	initialized = true;
	indicesCount = indicesCount_;

	return *this;
}

bool Primitives::End()
{
	glBindVertexArray(0);

	return initialized;
}

void Primitives::Destroy()
{
	initialized = false;
	vertexAttributes.clear();

	if (VAO)
	{
		if (VBO)
		{
			glDeleteBuffers(1, &VBO);
			VBO = 0;
		}

		if (EBO)
		{
			glDeleteBuffers(1, &EBO);
			EBO = 0;
		}

		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
}

void Primitives::Bind()
{
	if (VAO)
	{
		glBindVertexArray(VAO);
	}
}

void Primitives::Unbind()
{
	glBindVertexArray(0);
}

void Primitives::DrawArray(Primitives::Mode mode_, int first_, int count_)
{
	glDrawArrays(primModes[(int)mode_], first_, count_);
}

void Primitives::DrawArrayInstanced(Primitives::Mode mode_, int first_, int count_, unsigned int instancedCount_)
{
	/*
	if ( mode or count is invalid )
	{
		generate appropriate error
	}
	else
	{
		for (int i = 0; i < instancecount ; i++)
		{
			instanceID = i;
			glDrawArrays(mode, first, count);
		}
		instanceID = 0;
	}
	*/
	glDrawArraysInstanced(primModes[(int)mode_], first_, count_, instancedCount_);
}

void Primitives::DrawArrayInstancedBaseInstance(Primitives::Mode mode_, int first_, int count_, unsigned int instancedCount_, unsigned int baseInstance_)
{
	/*
	if ( mode or count is invalid )
	{
		generate appropriate error
	}
	else
	{
		for (int i = 0; i < instancecount ; i++)
		{
			instanceID = i;
			glDrawArrays(mode, first, count);
		}
		instanceID = 0;
	}
	*/
	glDrawArraysInstancedBaseInstance(primModes[(int)mode_], first_, count_, instancedCount_, baseInstance_);
}

////////////////////////////////////////////////////////////////////////////////////
void Primitives::DrawIndices(Primitives::Mode mode_, void* indices_, int count_)
{
	assert((EBO && indices_==0) || !indices_);

	glDrawElements(primModes[(int)mode_], count_, GL_UNSIGNED_INT, indices_);
}

void Primitives::DrawIndicesBaseVertex(Primitives::Mode mode_, void* indices_, int count_, int baseVertex_)
{
	assert((EBO && indices_ == 0) || !indices_);

	glDrawElementsBaseVertex(primModes[(int)mode_], count_, GL_UNSIGNED_INT, indices_, baseVertex_);
}

void Primitives::DrawIndicesInstanced(Primitives::Mode mode_, const void* indices_, int count_, int instancedCount_)
{
	assert((EBO && indices_ == 0) || !indices_);
		
	glDrawElementsInstanced(primModes[(int)mode_], count_, GL_UNSIGNED_INT, indices_, instancedCount_);
}

void Primitives::DrawIndicesInstancedBaseVertex(Primitives::Mode mode_, const void* indices_, int count_, int instancedCount_, int baseVertex_)
{
	/*
	if ( mode or count is invalid )
	{
		generate appropriate error
	}
	else
	{
		for (int i = 0; i < instancecount ; i++)
		{
			instanceID = i;
			glDrawArrays(mode, first, count);
		}
		instanceID = 0;
	}
	*/
	assert((EBO && indices_ == 0) || !indices_);

	glDrawElementsInstancedBaseVertex(primModes[(int)mode_], count_, GL_UNSIGNED_INT, indices_, instancedCount_, baseVertex_);
}

void Primitives::DrawIndicesInstancedBaseInstance(Primitives::Mode mode_, const void* indices_, int count_, int instancedCount_, int baseInstance_)
{
	/*
	if (mode, count, or type is invalid )
	{
		generate appropriate error
	}
	else 
	{
		for (int i = 0; i < instancecount ; i++) 
		{
			instanceID = i;
			glDrawElements(mode, count, type, indices);
		}
		instanceID = 0;
	}
	*/
	assert((EBO && indices_ == 0) || !indices_);

	glDrawElementsInstancedBaseInstance(primModes[(int)mode_], count_, GL_UNSIGNED_INT, indices_, instancedCount_, baseInstance_);
}

void Primitives::DrawIndicesInstancedBaseVertexBaseInstance(Primitives::Mode mode_, const void* indices_, int count_, int instancedCount_, int baseVertex_, int baseInstance_)
{
	/*
	if ( mode or count is invalid )
	{
		generate appropriate error
	}
	else
	{
		for (int i = 0; i < instancecount ; i++)
		{
			instanceID = i;
			glDrawArrays(mode, first, count);
		}
		instanceID = 0;
	}
	*/
	assert((EBO && indices_ == 0) || !indices_);

	glDrawElementsInstancedBaseVertexBaseInstance(primModes[(int)mode_], count_, GL_UNSIGNED_INT, indices_, instancedCount_, baseVertex_, baseInstance_);
}

void Primitives::DrawIndicesIndirect(Primitives::Mode mode_, void* indirect_)
{
	/*
	typedef  struct 
	{
		uint  count;
		uint  instancedCount;
		uint  first;
		uint  baseVertex;
		uint  baseInstance;
	} DrawElementsIndirectCommand;
	
	const DrawElementsIndirectCommand *cmd  = (const DrawElementsIndirectCommand *)indirect;
	glDrawElementsInstancedBaseVertexBaseInstance(mode,
												  cmd->count,
												  type,
												  cmd->first * size-of-type,
												  cmd->instancedCount,
												  cmd->baseVertex,
												  cmd->baseInstance);
	*/
	glDrawElementsIndirect(primModes[(int)mode_], GL_UNSIGNED_INT, indirect_);
}

//////////////////////////////////////////////////////////////////////////
void Primitives::MultiDrawArray(Primitives::Mode mode_, int* first_, int* count_, unsigned int mulitDrawCount_)
{
	glMultiDrawArrays(primModes[(int)mode_], first_, count_, mulitDrawCount_);
}

void Primitives::MultiDrawArrayIndirect(Primitives::Mode mode_, const void* indirect_, int mulitDrawCount_, int stride_)
{
	/*
	typedef  struct
	{
		uint  count;
		uint  instanceCount;
		uint  first;
		uint  baseInstance;
	} DrawArraysIndirectCommand;
	GLsizei n;
	for (n = 0; n < drawcount; n++)
	{
		const DrawArraysIndirectCommand* cmd;
		if (stride != 0)
		{
			cmd = (const DrawArraysIndirectCommand*)((uintptr)indirect + n * stride);
		}
		else
		{
			cmd = (const DrawArraysIndirectCommand*)indirect + n;
		}

		glDrawArraysInstancedBaseInstance(mode, cmd->first, cmd->count, cmd->instanceCount, cmd->baseInstance);
	}
	*/
	glMultiDrawArraysIndirect(primModes[(int)mode_], indirect_, mulitDrawCount_, stride_);
}

void Primitives::MultiDrawIndices(Primitives::Mode mode_, const void* const* indices_, int* count_, int mulitDrawCount_)
{
	assert((EBO && indices_ == 0) || !indices_);

	glMultiDrawElements(primModes[(int)mode_], count_, GL_UNSIGNED_INT, indices_, mulitDrawCount_);
}

void Primitives::MultiDrawIndicesBaseVertex(Primitives::Mode mode_, const void* const* indices_, int* count_, int* baseVertex_, int mulitDrawCount_)
{
	assert((EBO && indices_ == 0) || !indices_);

	glMultiDrawElementsBaseVertex(primModes[(int)mode_], count_, GL_UNSIGNED_INT, indices_, mulitDrawCount_, baseVertex_);
}

void Primitives::MultiDrawIndicesIndirect(Primitives::Mode mode_, const void* indirect_, int mulitDrawCount_, int stride_)
{
	/*
	typedef  struct {
		uint  count;
		uint  instanceCount;
		uint  first;
		uint  baseVertex;
		uint  baseInstance;
	} DrawElementsIndirectCommand;

	GLsizei n;
	for (n = 0; n < drawcount; n++) 
	{
		const DrawElementsIndirectCommand *cmd;
		if (stride != 0) 
		{
			cmd = (const DrawElementsIndirectCommand  *)((uintptr)indirect + n * stride);
		} 
		else 
		{
			cmd = (const DrawElementsIndirectCommand  *)indirect + n;
		}

		glDrawElementsInstancedBaseVertexBaseInstance(mode,
													cmd->count,
													type,
													cmd->first * size-of-type,
													cmd->instanceCount,
													cmd->baseVertex,
													cmd->baseInstance);
	}
	*/
	glMultiDrawElementsIndirect(primModes[(int)mode_], GL_UNSIGNED_INT, indirect_, mulitDrawCount_, stride_);
}

unsigned int Primitives::GetCount()
{
	if (EBO)
	{
		return indicesCount;
	}
	else
	{
		return verticesCount;
	}
}