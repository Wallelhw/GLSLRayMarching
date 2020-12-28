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

	VertexAttribute(unsigned int index_, int elementCount_, DataType type_, bool normalized_, unsigned int stride_ = 0)
		: index(index_)
		, elementCount(elementCount_)
		, type(type_)
		, normalized(normalized_)
		, stride(stride_)
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

		return *this;	
	}

	VertexArrayObject& Attribute(unsigned int index_, int elementCount_, VertexAttribute::DataType type_, bool normalized_, unsigned int stride_ = 0)
	{
		vertexAttributes.push_back(VertexAttribute(index_, elementCount_, type_, normalized_, stride_));

		return *this;
	}

	VertexArrayObject& FillVertices(float* vertices_, int verticesCount_)
	{
		glGenVertexArrays(1, &VAO);
		if (VAO == 0)
		{
			Destroy();
			return *this;
		}

		glGenBuffers(1, &VBO);
		if (VBO == 0)
		{
			Destroy();
			return *this;
		}

		glBindVertexArray(VAO);

		for (int i=0; i< vertexAttributes.size(); i++)
		{
			VertexAttribute& d = vertexAttributes[i];

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, verticesCount_ * sizeof(float), vertices_, GL_STATIC_DRAW);

			//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glVertexAttribPointer(d.index, d.elementCount, d.type, d.normalized, d.elementCount * d.elementSize, (void*)0);
			glEnableVertexAttribArray(i);
		}

		glBindVertexArray(0);

		initialized = true;
		verticesCount = verticesCount_;
		indicesCount = 0;

		return *this;
	}

	VertexArrayObject& FillVerticesAndIndices(float* vertices_, int verticesCount_, unsigned int* indices_, int indicesCount_)
	{
		glGenVertexArrays(1, &VAO);
		if (VAO == 0)
		{
			Destroy();
			return *this;
		}

		glGenBuffers(1, &VBO);
		if (VBO == 0)
		{
			Destroy();
			return *this;
		}

		glGenBuffers(1, &EBO);
		if (EBO == 0)
		{
			Destroy();
			return *this;
		}

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, verticesCount_ * sizeof(float), vertices_, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount_ * sizeof(int), indices_, GL_STATIC_DRAW);

		for (int i = 0; i < vertexAttributes.size(); i++)
		{
			VertexAttribute& d = vertexAttributes[i];

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(float), vertices_, GL_STATIC_DRAW);

			//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glVertexAttribPointer(d.index, d.elementCount, d.type, d.normalized, d.elementCount * d.elementSize, (void*)0);
			glEnableVertexAttribArray(i);
		}

		glBindVertexArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		initialized = true;
		verticesCount = verticesCount_;
		indicesCount = indicesCount_;

		return *this;
	}

	bool End()
	{
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

	void Draw(unsigned int type_, unsigned int count_ = 0)
	{
		if (EBO)
		{
			if (count_ == 0)
				count_ = indicesCount;

			glDrawElements(type_, count_, GL_UNSIGNED_INT, 0);
		}
		else
		{
			if (count_ == 0)
				count_ = verticesCount;
			glDrawArrays(type_, 0, count_);
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