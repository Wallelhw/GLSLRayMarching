#ifndef _Primitives_h_
#define _Primitives_h_

#include "Platform.h"
#include <map>

class VertexAttribute
{
public:
	enum class DataType
	{
		BYTE = 0,
		UNSIGNED_BYTE,
		SHORT,
		UNSIGNED_SHORT,
		INT,
		UNSIGNED_INT,

		HALF_FLOAT,
		FLOAT,
		DOUBLE,
		FIXED,
		INT_2_10_10_10_REV,
		UNSIGNED_INT_2_10_10_10_REV,
		UNSIGNED_INT_10F_11F_11F_REV
	};

	VertexAttribute();
	VertexAttribute(unsigned int index_, int elementCount_, VertexAttribute::DataType type_, bool normalized_, unsigned int divisor_ = 0, unsigned int stride_ = 0);

	unsigned int index;
	int elementCount;
	int elementSize;
	DataType dataType;
	bool normalized;
	unsigned int stride;
	unsigned int divisor;
};

class Primitives
{
public:
	enum class Mode
	{
		POINTS = 0,
		LINE_STRIP,
		LINE_LOOP,
		LINES,
		LINE_STRIP_ADJACENCY,
		LINES_ADJACENCY,
		TRIANGLE_STRIP,
		TRIANGLE_FAN,
		TRIANGLES,
		TRIANGLE_STRIP_ADJACENCY,
		TRIANGLES_ADJACENCY,
		PATCHES
	};

	Primitives();
	virtual ~Primitives();

	Primitives& Begin();
	Primitives& FillVertices(unsigned int index_, int elementCount_, VertexAttribute::DataType type_, bool normalized_, unsigned int stride_, unsigned int divisor_, const void* vertices_, int verticesCount_);
	Primitives& FillIndices(const unsigned int* indices_, int indicesCount_);
	bool End();
	
	void Terminate();
	void Bind();
	void Unbind();

	////////////////////////////////////////////////////////////////////////////////////
	void DrawArray(Primitives::Mode mode_, int first_, int count_);
	void DrawArrayInstanced(Primitives::Mode mode_, int first_, int count_, int instancedCount_);
	void DrawArrayInstancedBaseInstance(Primitives::Mode mode_, int first_, int count_, int instancedCount_, int baseInstance_);

	////////////////////////////////////////////////////////////////////////////////////
	void DrawIndices(Primitives::Mode mode_, void* indices_, int count_);
	void DrawIndicesBaseVertex(Primitives::Mode mode_, void* indices_, int count_, int baseVertex_);
	void DrawIndicesInstanced(Primitives::Mode mode_, const void* indices_, int count_, int instancedCount_);
	void DrawIndicesInstancedBaseVertex(Primitives::Mode mode_, const void* indices_, int count_, int instancedCount_, int baseVertex_);
	void DrawIndicesInstancedBaseInstance(Primitives::Mode mode_, const void* indices_, int count_, int instancedCount_, int baseInstance_);
	void DrawIndicesInstancedBaseVertexBaseInstance(Primitives::Mode mode_, const void* indices_, int count_, int instancedCount_, int baseVertex_, int baseInstance_);
	void DrawIndicesIndirect(Primitives::Mode mode_, void* indirect_);
	
	//////////////////////////////////////////////////////////////////////////
	void MultiDrawArray(Primitives::Mode mode_, int* first_, int* count_, unsigned int mulitDrawCount_);
	void MultiDrawArrayIndirect(Primitives::Mode mode_, const void* indirect_, int mulitDrawCount_, int stride_ = 0);
	void MultiDrawIndices(Primitives::Mode mode_, const void* const* indices_, int* count_, int mulitDrawCount_);
	void MultiDrawIndicesBaseVertex(Primitives::Mode mode_, const void* const* indices_, int* count_, int* baseVertex_, int mulitDrawCount_);
	void MultiDrawIndicesIndirect(Primitives::Mode mode_, const void* indirect_, int mulitDrawCount_, int stride_);
	unsigned int GetCount();
private:
private:
	std::map<int, VertexAttribute> vertexAttributes;
	std::map<int, unsigned int> vbos;
	unsigned int vao;
	unsigned int ebo;
	unsigned int verticesCount;
	unsigned int indicesCount;
};

#endif