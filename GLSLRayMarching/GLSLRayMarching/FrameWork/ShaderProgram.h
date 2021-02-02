#ifndef _ShaderProgram_h_
#define _ShaderProgram_h_

#include "Platform.h"
#include <vector>

class ShaderProgramImpl;
class Buffer;

class ShaderProgramBinary
{
public:
	ShaderProgramBinary()
	{
	}

	unsigned int format;
	std::vector<unsigned char> buffer;
};

class ShaderProgram
{
public:
	ShaderProgram();
	virtual ~ShaderProgram();

	bool Create(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	void Destroy();
	bool CreateFromSource(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode = nullptr);
	void GetProgramBinary(ShaderProgramBinary& binary);
	bool CreateFromBinary(const ShaderProgramBinary& binary);
	unsigned int GetHandle() const;

	void Bind() const;
	void Unbind() const;
	
	void SetUniform1i(const char* name_, int v0_);
	void SetUniform2i(const char* name_, int v0_, int v1_);
	void SetUniform3i(const char* name_, int v0_, int v1_, int v2_);
	void SetUniform4i(const char* name_, int v0_, int v1_, int v2_, int v3_);
	void SetUniform1iv(const char* name_, int count_, const int* v_);
	void SetUniform2iv(const char* name_, int count_, const int* v_);
	void SetUniform3iv(const char* name_, int count_, const int* v_);
	void SetUniform4iv(const char* name_, int count_, const int* v_);

	void SetUniform1ui(const char* name_, unsigned int v0_);
	void SetUniform2ui(const char* name_, unsigned int v0_, unsigned int v1_);
	void SetUniform3ui(const char* name_, unsigned int v0_, unsigned int v1_, unsigned int v2_);
	void SetUniform4ui(const char* name_, unsigned int v0_, unsigned int v1_, unsigned int v2_, unsigned int v3_);
	void SetUniform1uiv(const char* name_, int count_, const unsigned int* v_);
	void SetUniform2uiv(const char* name_, int count_, const unsigned int* v_);
	void SetUniform3uiv(const char* name_, int count_, const unsigned int* v_);
	void SetUniform4uiv(const char* name_, int count_, const unsigned int* v_);

	void SetUniform1f(const char* name_, float v0_);
	void SetUniform2f(const char* name_, float v0_, float v1_);
	void SetUniform3f(const char* name_, float v0_, float v1_, float v2_);
	void SetUniform4f(const char* name_, float v0_, float v1_, float v2_, float v3_);
	void SetUniform1fv(const char* name_, int count_, const float* v_);
	void SetUniform2fv(const char* name_, int count_, const float* v_);
	void SetUniform3fv(const char* name_, int count_, const float* v_);
	void SetUniform4fv(const char* name_, int count_, const float* v_);

	void SetUniformMatrix2x2fv(const char* name_, int count_, const float* v_);
	void SetUniformMatrix2x3fv(const char* name_, int count_, const float* v_);
	void SetUniformMatrix2x4fv(const char* name_, int count_, const float* v_);
	void SetUniformMatrix3x2fv(const char* name_, int count_, const float* v_);
	void SetUniformMatrix3x3fv(const char* name_, int count_, const float* v_);
	void SetUniformMatrix3x4fv(const char* name_, int count_, const float* v_);
	void SetUniformMatrix4x2fv(const char* name_, int count_, const float* v_);
	void SetUniformMatrix4x3fv(const char* name_, int count_, const float* v_);
	void SetUniformMatrix4x4fv(const char* name_, int count_, const float* v_);

	void BindShaderStorageBuffer(Buffer& buffer_, const char* name_, unsigned int bindingPoint_);
	void BindUniformBlock(Buffer& buffer_, const char* name_, unsigned int bindingPoint_);
	void BindShaderStorageBuffer(Buffer& buffer_, unsigned int bindingPoint_);
	void BindUniformBlock(Buffer& buffer_, unsigned int bindingPoint_);

	static void ReleaseShaderCompiler();
private:
	bool CheckCompileErrors(unsigned int shader, std::string type) const;

	ShaderProgramImpl* impl;
};

#endif