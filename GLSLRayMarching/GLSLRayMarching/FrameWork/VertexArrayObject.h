#ifndef _VertexArrayObject_h_
#define _VertexArrayObject_h_

#include <glad\glad.h>
#include <GLFW\glfw3.h>

class VertexAttribute
{
public:
	enum DataType
	{
		BYTE = GL_BYTE,
		UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
		SHORT = GL_SHORT,
		UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
		INT = GL_INT,
		UNSIGNED_INT = GL_UNSIGNED_INT,

		HALF_FLOAT = GL_HALF_FLOAT,
		FLOAT = GL_FLOAT,
		DOUBLE = GL_DOUBLE,
		FIXED = GL_FIXED,
		INT_2_10_10_10_REV = GL_INT_2_10_10_10_REV,
		UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV,
		UNSIGNED_INT_10F_11F_11F_REV = GL_UNSIGNED_INT_10F_11F_11F_REV
	};

	VertexAttribute()
	{
	}

	VertexAttribute(unsigned int index_, int elementCount_, DataType type_, bool normalized_, unsigned int divisor_ = 0, unsigned int stride_ = 0)
		: index(index_)
		, elementCount(elementCount_)
		, type(type_)
		, normalized(normalized_)
		, stride(stride_)
		, divisor(divisor_)
	{
		elementSize = GetSize(type_);
		if (stride)
		{
			stride = elementCount * elementSize;
		}
	}

	unsigned int GetSize(DataType type)
	{
		switch (type)
		{
		default:
		case BYTE:
			return 1;
		case UNSIGNED_BYTE:
			return 1;
		case SHORT:
			return 2;
		case UNSIGNED_SHORT:
			return 2;
		case INT:
			return 4;
		case UNSIGNED_INT:
			return 4;

		case HALF_FLOAT:
			return 2;
		case FLOAT:
			return 4;
		case DOUBLE:
			return 8;
		case FIXED:
			return 4;
		case INT_2_10_10_10_REV:
			return 4;
		case UNSIGNED_INT_2_10_10_10_REV:
			return 4;
		case UNSIGNED_INT_10F_11F_11F_REV:
			return 4;
		}
	}

	unsigned int index;
	int elementCount;
	int elementSize;
	DataType type;
	bool normalized;
	unsigned int stride;
	unsigned int divisor;
};

class VertexArrayObject
{
public:
	VertexArrayObject()
	: initialized(false)
	, vertexAttributes()
	, VAO(0)
	, VBO(0)
	, EBO(0)
	, verticesCount(0)
	, indicesCount(0)
	{
	}

	virtual ~VertexArrayObject()
	{
	}

	VertexArrayObject& Begin()
	{
		Destroy();

		glGenVertexArrays(1, &VAO);
		if (VAO == 0)
		{
			Destroy();
			return *this;
		}

		glBindVertexArray(VAO);

		return *this;	
	}

	VertexArrayObject& FillVertices(unsigned int index_, int elementCount_, VertexAttribute::DataType type_, bool normalized_, unsigned int stride_, unsigned int divisor_, float* vertices_, int verticesCount_)
	{
		glGenBuffers(1, &VBO);
		if (VBO == 0)
		{
			Destroy();
			return *this;
		}

		vertexAttributes.push_back(VertexAttribute(index_, elementCount_, type_, normalized_, divisor_, stride_));
		VertexAttribute& v = vertexAttributes.back();
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, verticesCount_ * elementCount_* GetElementSize(type_), vertices_, GL_STATIC_DRAW);
		glEnableVertexAttribArray(v.index);
		glVertexAttribPointer(v.index, v.elementCount, v.type, v.normalized, v.elementCount * v.elementSize, (void*)0);
		glVertexAttribDivisor(v.index, v.divisor);

		initialized = true;
		verticesCount = verticesCount_;

		return *this;
	}

	VertexArrayObject& FillIndices(unsigned int* indices_, int indicesCount_)
	{
		glGenBuffers(1, &EBO);
		if (EBO == 0)
		{
			Destroy();
			return *this;
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount_ * sizeof(int), indices_, GL_STATIC_DRAW);

		initialized = true;
		indicesCount = indicesCount_;

		return *this;
	}

	bool End()
	{
		glBindVertexArray(0);

		return initialized;
	}

	void Destroy()
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

	void Bind()
	{
		if (VAO)
		{
			glBindVertexArray(VAO);
		}
	}

	void Unbind()
	{
		glBindVertexArray(0);
	}

	void DrawArray(unsigned int mode_, int first_, int count_)
	{
		glDrawArrays(mode_, first_, count_);
	}

	void DrawArrayInstanced(unsigned int mode_, int first_, int count_, unsigned int instancedCount_)
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
		glDrawArraysInstanced(mode_, first_, count_, instancedCount_);
	}

	void DrawArrayInstancedBaseInstance(unsigned int mode_, int first_, int count_, unsigned int instancedCount_, unsigned int baseInstance_)
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
		glDrawArraysInstancedBaseInstance(mode_, first_, count_, instancedCount_, baseInstance_);
	}

	////////////////////////////////////////////////////////////////////////////////////
	void DrawIndices(unsigned int mode_, void* indices_, int count_)
	{
		assert((EBO && indices_==0) || !indices_);

		glDrawElements(mode_, count_, GL_UNSIGNED_INT, indices_);
	}

	void DrawIndicesBaseVertex(unsigned int mode_, void* indices_, int count_, int baseVertex_)
	{
		assert((EBO && indices_ == 0) || !indices_);

		glDrawElementsBaseVertex(mode_, count_, GL_UNSIGNED_INT, indices_, baseVertex_);
	}

	void DrawIndicesInstanced(unsigned int mode_, const void* indices_, int count_, int instancedCount_)
	{
		assert((EBO && indices_ == 0) || !indices_);
		
		glDrawElementsInstanced(mode_, count_, GL_UNSIGNED_INT, indices_, instancedCount_);
	}

	void DrawIndicesInstancedBaseVertex(unsigned int mode_, const void* indices_, int count_, int instancedCount_, int baseVertex_)
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

		glDrawElementsInstancedBaseVertex(mode_, count_, GL_UNSIGNED_INT, indices_, instancedCount_, baseVertex_);
	}

	void DrawIndicesInstancedBaseInstance(unsigned int mode_, const void* indices_, int count_, int instancedCount_, int baseInstance_)
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

		glDrawElementsInstancedBaseInstance(mode_, count_, GL_UNSIGNED_INT, indices_, instancedCount_, baseInstance_);
	}

	void DrawIndicesInstancedBaseVertexBaseInstance(unsigned int mode_, const void* indices_, int count_, int instancedCount_, int baseVertex_, int baseInstance_)
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

		glDrawElementsInstancedBaseVertexBaseInstance(mode_, count_, GL_UNSIGNED_INT, indices_, instancedCount_, baseVertex_, baseInstance_);
	}

	void DrawIndicesIndirect(unsigned int mode_, void* indirect_)
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
		glDrawElementsIndirect(mode_, GL_UNSIGNED_INT, indirect_);
	}

	//////////////////////////////////////////////////////////////////////////
	void MultiDrawArray(unsigned int mode_, int* first_, int* count_, unsigned int mulitDrawCount_)
	{
		glMultiDrawArrays(mode_, first_, count_, mulitDrawCount_);
	}

	void MultiDrawArrayIndirect(unsigned int mode_, const void* indirect_, int mulitDrawCount_, int stride_ = 0)
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
		glMultiDrawArraysIndirect(mode_, indirect_, mulitDrawCount_, stride_);
	}

	void MultiDrawIndices(unsigned int mode_, const void* const* indices_, int* count_, int mulitDrawCount_)
	{
		assert((EBO && indices_ == 0) || !indices_);

		glMultiDrawElements(mode_, count_, GL_UNSIGNED_INT, indices_, mulitDrawCount_);
	}

	void MultiDrawIndicesBaseVertex(unsigned int mode_, const void* const* indices_, int* count_, int* baseVertex_, int mulitDrawCount_)
	{
		assert((EBO && indices_ == 0) || !indices_);

		glMultiDrawElementsBaseVertex(mode_, count_, GL_UNSIGNED_INT, indices_, mulitDrawCount_, baseVertex_);
	}

	void MultiDrawIndicesIndirect(unsigned int mode_, const void* indirect_, int mulitDrawCount_, int stride_)
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
		glMultiDrawElementsIndirect(mode_, GL_UNSIGNED_INT, indirect_, mulitDrawCount_, stride_);
	}

	unsigned int GetCount()
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
private:
	unsigned int GetElementSize(unsigned int type_)
	{
		switch (type_)
		{
		case VertexAttribute::BYTE:
			return 1;
		case VertexAttribute::UNSIGNED_BYTE:
			return 1;
		case VertexAttribute::SHORT:
			return 2;
		case VertexAttribute::UNSIGNED_SHORT:
			return 2;
		case VertexAttribute::INT:
			return 4;
		case VertexAttribute::UNSIGNED_INT:
			return 4;

		case VertexAttribute::HALF_FLOAT:
			return 2;
		case VertexAttribute::FLOAT:
			return 4;
		case VertexAttribute::DOUBLE:
			return 8;
		case VertexAttribute::FIXED:
			return 4;
		case VertexAttribute::INT_2_10_10_10_REV:
			return 4;
		case VertexAttribute::UNSIGNED_INT_2_10_10_10_REV:
			return 4;
		case VertexAttribute::UNSIGNED_INT_10F_11F_11F_REV:
			return 4;
		}
	}


private:
	bool initialized;
	std::vector<VertexAttribute> vertexAttributes;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int verticesCount;
	unsigned int indicesCount;
};

#endif