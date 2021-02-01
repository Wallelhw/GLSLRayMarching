#ifndef _ShaderProgram_h_
#define _ShaderProgram_h_

#include "Platform.h"
class ShaderProgramImpl;
class Buffer;

class ShaderProgram
{
public:
	ShaderProgram();
	virtual ~ShaderProgram();

	bool CreateFromBuffer(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode = nullptr);
	bool Create(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	void Destroy();
	void Bind() const;
	void Unbind() const;
	
	void SetUniform1i(const char* name_, int v0_);
	void SetUniform2i(const char* name_, int v0_, int v1_);
	void SetUniform3i(const char* name_, int v0_, int v1_, int v2_);
	void SetUniform4i(const char* name_, int v0_, int v1_, int v2_, int v3_);
	void SetUniform1f(const char* name_, float v0_);
	void SetUniform2f(const char* name_, float v0_, float v1_);
	void SetUniform3f(const char* name_, float v0_, float v1_, float v2_);
	void SetUniform4f(const char* name_, float v0_, float v1_, float v2_, float v3_);
	void SetUniform1iv(const char* name_, int count_, const int* v_);
	void SetUniform2iv(const char* name_, int count_, const int* v_);
	void SetUniform3iv(const char* name_, int count_, const int* v_);
	void SetUniform4iv(const char* name_, int count_, const int* v_);
	void SetUniform1fv(const char* name_, int count_, const float* v_);
	void SetUniform2fv(const char* name_, int count_, const float* v_);
	void SetUniform3fv(const char* name_, int count_, const float* v_);
	void SetUniform4fv(const char* name_, int count_, const float* v_);
	void SetUniformMatrix2fv(const char* name_, int count_, const float* v_);
	void SetUniformMatrix3fv(const char* name_, int count_, const float* v_);
	void SetUniformMatrix4fv(const char* name_, int count_, const float* v_);

	void BindShaderStorageBuffer(Buffer& buffer_, const char* name_, unsigned int bindingPoint_);

	unsigned int GetHandle() const;
private:
	bool CheckCompileErrors(unsigned int shader, std::string type) const;

	ShaderProgramImpl* impl;
};

#endif