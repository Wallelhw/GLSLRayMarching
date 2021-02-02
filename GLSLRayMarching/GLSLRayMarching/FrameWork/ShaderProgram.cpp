#include "Platform.h"
#include "Graphics.h"
#include "ShaderProgram.h"
#include "Buffer.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ShaderProgramImpl
{
public:
	ShaderProgramImpl()
		: handle(0)
	{
	}

	unsigned int handle;
};

/////////////////////////////////////////////////////////////////////////
ShaderProgram::ShaderProgram()
	: impl(nullptr)
{
	impl = new ShaderProgramImpl();
}

ShaderProgram::~ShaderProgram()
{
	Assert(impl);

	Destroy();

	if (impl)
	{
		delete impl;
		impl = nullptr;
	}
}

bool ShaderProgram::Create(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	Assert(impl);

	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		if (geometryPath)
			gShaderFile.open(geometryPath);
		std::stringstream vShaderStream, fShaderStream, gShaderStream;

		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		if (geometryPath)
			gShaderStream << gShaderFile.rdbuf();

		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		if (geometryPath)
			gShaderFile.close();

		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		if (geometryPath)
			geometryCode = gShaderStream.str();
	}
	catch (std::ifstream::failure&)
	{
		return false;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();
	if (!geometryPath)
		gShaderCode = nullptr;

	return CreateFromSource(vShaderCode, fShaderCode, gShaderCode);
}

void ShaderProgram::Destroy()
{
	Assert(impl);

	if (impl->handle)
	{
		glDeleteProgram(impl->handle);
		impl->handle = 0;

		Platform::MemSet(impl, 0, sizeof(*impl));
	}
}

bool ShaderProgram::CreateFromSource(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode)
{
	Assert(impl);

	Destroy();

	// 2. compile shaders
	unsigned int vertex, fragment, geometry;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	if (!CheckCompileErrors(vertex, "VERTEX"))
	{
		printf("failed to compile vertex shader\n");
		return false;
	}

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	if (!CheckCompileErrors(fragment, "FRAGMENT"))
	{
		printf("failed to compile fragment shader\n");
		return false;
	}

	if (gShaderCode)
	{
		// fragment Shader
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		if (!CheckCompileErrors(geometry, "GEOMETRY"))
		{
			printf("failed to compile geometry shader\n");
			return false;
		}
	}

	// shader Program
	impl->handle = glCreateProgram();
	if (impl->handle == 0)
		return false;

	glAttachShader(impl->handle, vertex);
	glAttachShader(impl->handle, fragment);
	if (gShaderCode)
	{
		glAttachShader(impl->handle, geometry);
	}

	glLinkProgram(impl->handle);
	if (!CheckCompileErrors(impl->handle, "PROGRAM"))
	{
		printf("failed to link shader programs\n");
		return false;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (gShaderCode)
	{
		glDeleteShader(geometry);
	}

	return true;
}

void ShaderProgram::GetProgramBinary(ShaderProgramBinary& shaderProgramBinary)
{
	Assert(impl);
	Assert(impl->handle);

	int inBufLength = 0;
	int outBufLength = 0;
	glGetProgramiv(impl->handle, GL_PROGRAM_BINARY_LENGTH, &inBufLength);

	shaderProgramBinary.buffer.resize(inBufLength);
	glGetProgramBinary(impl->handle, inBufLength, &outBufLength, &shaderProgramBinary.format, &shaderProgramBinary.buffer[0]);
}

bool ShaderProgram::CreateFromBinary(const ShaderProgramBinary& shaderProgramBinary)
{
	Assert(impl);

	Destroy();

	// shader Program
	impl->handle = glCreateProgram();
	if(impl->handle==0)
		return false;

	glProgramBinary(impl->handle, shaderProgramBinary.format, &shaderProgramBinary.buffer[0], shaderProgramBinary.buffer.size());
	if (!CheckCompileErrors(impl->handle, "PROGRAM"))
	{
		printf("failed to load binary program\n");
		return false;
	}

	return true;
}

unsigned int ShaderProgram::GetHandle() const
{
	Assert(impl);

	return impl->handle;
}

void ShaderProgram::Bind() const
{
	Assert(impl);
	Assert(impl->handle);

	glUseProgram(impl->handle);
}

void ShaderProgram::Unbind() const
{
	Assert(impl);

	glUseProgram(0);
}

void ShaderProgram::SetUniform1i(const char* name_, int v0_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform1i(idx, v0_);
}

void ShaderProgram::SetUniform2i(const char* name_, int v0_, int v1_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform2i(idx, v0_, v1_);
}

void ShaderProgram::SetUniform3i(const char* name_, int v0_, int v1_, int v2_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform3i(idx, v0_, v1_, v2_);
}

void ShaderProgram::SetUniform4i(const char* name_, int v0_, int v1_, int v2_, int v3_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform4i(idx, v0_, v1_, v2_, v3_);
}

void ShaderProgram::SetUniform1iv(const char* name_, int count_, const int* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform1iv(idx, count_, v_);
}

void ShaderProgram::SetUniform2iv(const char* name_, int count_, const int* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform2iv(idx, count_, v_);
}

void ShaderProgram::SetUniform3iv(const char* name_, int count_, const int* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform3iv(idx, count_, v_);
}

void ShaderProgram::SetUniform4iv(const char* name_, int count_, const int* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform4iv(idx, count_, v_);
}

void ShaderProgram::SetUniform1ui(const char* name_, unsigned int v0_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform1i(idx, v0_);
}

void ShaderProgram::SetUniform2ui(const char* name_, unsigned int v0_, unsigned int v1_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform2i(idx, v0_, v1_);
}

void ShaderProgram::SetUniform3ui(const char* name_, unsigned int v0_, unsigned int v1_, unsigned int v2_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform3i(idx, v0_, v1_, v2_);
}

void ShaderProgram::SetUniform4ui(const char* name_, unsigned int v0_, unsigned int v1_, unsigned int v2_, unsigned int v3_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform4i(idx, v0_, v1_, v2_, v3_);
}

void ShaderProgram::SetUniform1uiv(const char* name_, int count_, const unsigned int* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform1uiv(idx, count_, v_);
}

void ShaderProgram::SetUniform2uiv(const char* name_, int count_, const unsigned  int* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform2uiv(idx, count_, v_);
}

void ShaderProgram::SetUniform3uiv(const char* name_, int count_, const unsigned int* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform3uiv(idx, count_, v_);
}

void ShaderProgram::SetUniform4uiv(const char* name_, int count_, const unsigned int* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform4uiv(idx, count_, v_);
}

void ShaderProgram::SetUniform1f(const char* name_, float v0_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform1f(idx, v0_);
}

void ShaderProgram::SetUniform2f(const char* name_, float v0_, float v1_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform2f(idx, v0_, v1_);
}

void ShaderProgram::SetUniform3f(const char* name_, float v0_, float v1_, float v2_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform3f(idx, v0_, v1_, v2_);
}

void ShaderProgram::SetUniform4f(const char* name_, float v0_, float v1_, float v2_, float v3_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform4f(idx, v0_, v1_, v2_, v3_);
}

void ShaderProgram::SetUniform1fv(const char* name_, int count_, const float* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform1fv(idx, count_, v_);
}

void ShaderProgram::SetUniform2fv(const char* name_, int count_, const float* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform2fv(idx, count_, v_);
}

void ShaderProgram::SetUniform3fv(const char* name_, int count_, const float* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform3fv(idx, count_, v_);
}

void ShaderProgram::SetUniform4fv(const char* name_, int count_, const float* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniform4fv(idx, count_, v_);
}

void ShaderProgram::SetUniformMatrix2x2fv(const char* name_, int count_, const float* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniformMatrix2fv(idx, count_, true, v_);
}

void ShaderProgram::SetUniformMatrix2x3fv(const char* name_, int count_, const float* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniformMatrix2x3fv(idx, count_, true, v_);
}

void ShaderProgram::SetUniformMatrix2x4fv(const char* name_, int count_, const float* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniformMatrix2x4fv(idx, count_, true, v_);
}

void ShaderProgram::SetUniformMatrix3x2fv(const char* name_, int count_, const float* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniformMatrix3x2fv(idx, count_, true, v_);
}

void ShaderProgram::SetUniformMatrix3x3fv(const char* name_, int count_, const float* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniformMatrix3fv(idx, count_, true, v_);
}

void ShaderProgram::SetUniformMatrix3x4fv(const char* name_, int count_, const float* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniformMatrix3x4fv(idx, count_, true, v_);
}

void ShaderProgram::SetUniformMatrix4x2fv(const char* name_, int count_, const float* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniformMatrix4x2fv(idx, count_, true, v_);
}

void ShaderProgram::SetUniformMatrix4x3fv(const char* name_, int count_, const float* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniformMatrix4x3fv(idx, count_, true, v_);
}

void ShaderProgram::SetUniformMatrix4x4fv(const char* name_, int count_, const float* v_)
{
	Assert(impl);

	int idx = glGetUniformLocation(impl->handle, name_);

	glUniformMatrix4fv(idx, count_, true, v_);
}

void ShaderProgram::BindShaderStorageBuffer(Buffer& buffer_, const char* name_, unsigned int bindingPoint_)
{
	Assert(impl);

	buffer_.BindShaderStorage(*this, name_, bindingPoint_);
}

void ShaderProgram::BindShaderStorageBuffer(Buffer& buffer_, unsigned int bindingPoint_)
{
	Assert(impl);

	buffer_.BindShaderStorage(*this, bindingPoint_);
}

void ShaderProgram::BindUniformBlock(Buffer& buffer_, const char* name_, unsigned int bindingPoint_)
{
	Assert(impl);

	buffer_.BindUniformBlock(*this, name_, bindingPoint_);
}

void ShaderProgram::BindUniformBlock(Buffer& buffer_, unsigned int bindingPoint_)
{
	Assert(impl);

	buffer_.BindUniformBlock(*this, bindingPoint_);
}

bool ShaderProgram::CheckCompileErrors(unsigned int shader, std::string type) const
{
	Assert(impl);

	GLint success = 0;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			Platform::Debug("ERROR::SHADER_COMPILATION_ERROR of type: %s%s%s%s\n", type.c_str(), "\n", infoLog, "\n");

			return false;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			Platform::Debug("ERROR::PROGRAM_LINKING_ERROR of type: %s%s%s%s\n", type.c_str(), "\n", infoLog, "\n");

			return false;
		}
	}

	return true;
}

void ShaderProgram::ReleaseShaderCompiler()
{
	glReleaseShaderCompiler();
}