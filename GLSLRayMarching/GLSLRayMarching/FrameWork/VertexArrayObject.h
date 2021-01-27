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

	void Draw(unsigned int mode_, unsigned int first_, unsigned int count_)
	{
		if (EBO)
		{
			if (count_ == 0)
				count_ = indicesCount;

			glDrawElements(mode_, count_, GL_UNSIGNED_INT, 0);
		}
		else
		{
			if (count_ == 0)
				count_ = verticesCount;

			glDrawArrays(mode_, first_, count_);
		}
	}

	void DrawInstanced(unsigned int mode_, unsigned int first_, unsigned int count_, unsigned int instancedCount_)
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
		if (EBO)
		{
			if (count_ == 0)
				count_ = indicesCount;

			glDrawElementsInstanced(mode_, count_, GL_UNSIGNED_INT, 0, instancedCount_);
		}
		else
		{
			if (count_ == 0)
				count_ = verticesCount;
			
			glDrawArraysInstanced(mode_, first_, count_, instancedCount_);
		}
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