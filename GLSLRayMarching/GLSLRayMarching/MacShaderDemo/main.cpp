#include "FrameWork.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "Primitives.h"
#include "RenderStates.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;
#include <iostream>
#include <fstream>
#include <sstream>

#define SCR_WIDTH 800*2
#define SCR_HEIGHT 400*2

#include <Windows.h>
#include <WinUser.h>
#include <time.h>

class KeyboardTexture : public Texture2D
{
public:
	KeyboardTexture()
		: Texture2D()
		, buffer(256*3)
	{
	}

	virtual ~KeyboardTexture()
	{
	}

	bool Create()
	{
		if (!Texture2D::Create(256, 3, 1, false, &buffer[0]))
			return false;
		
		SetMinFilter(Texture::MinFilter::NEAREST);
		SetMagFilter(Texture::MagFilter::NEAREST);
		SetWarpS(Texture::Wrap::CLAMP);
		SetWarpR(Texture::Wrap::CLAMP);
		SetWarpT(Texture::Wrap::CLAMP);

		return true;
	}

	void UpdateKey(int key)
	{
		char* keydown = &buffer[256 * 0];
		char* keyclick = &buffer[256 * 1];
		char* keytoggle = &buffer[256 * 2];

		int oldKeydown = keydown[key];

		int keyState = GetAsyncKeyState(key);
		keydown[key] = (keyState & 0x8000) != 0;
		keyclick[key] = (keydown[key] != oldKeydown) && keydown[key];
		keytoggle[key] = (keyState & 0x0001) != 0;

		/*
		bool release = (keydown[key] != oldKeydown) && !keydown[key];
		if (key == 38)
		{
			//printf("%d %d %d\n", keydown[key], keyclick[key], keytoggle[key]);
			if (keyclick[key])
			{
				int a = 1;
			}
		}
		*/
	}

	virtual void Update()
	{
		for (int i = 'a'; i < 'z'; i++)
		{
			UpdateKey(i);
		}

		for (int i = 'A'; i < 'Z'; i++)
		{
			UpdateKey(i);
		}

		UpdateKey(VK_LEFT);
		UpdateKey(VK_UP);
		UpdateKey(VK_RIGHT);
		UpdateKey(VK_DOWN);
		UpdateKey(VK_PRIOR);
		UpdateKey(VK_NEXT);
		UpdateKey(VK_END);
		UpdateKey(VK_HOME);
		UpdateKey(VK_SHIFT);
		UpdateKey(VK_CONTROL);
		UpdateKey(VK_MENU);

		Texture2D::UpdateData(&buffer[0]);
	}
private:
private:
	std::vector<char> buffer;
};

class GUITexture : public Texture2D
{
public:
	GUITexture()
	: Texture2D()
	, buffer(256 * 3)
	{
	}

	virtual ~GUITexture()
	{
	}

	bool Create()
	{
		if (!Texture2D::Create(256, 3, 1, false, &buffer[0]))
			return false;

		SetMinFilter(Texture::MinFilter::NEAREST);
		SetMagFilter(Texture::MagFilter::NEAREST);
		SetWarpS(Texture::Wrap::CLAMP);
		SetWarpR(Texture::Wrap::CLAMP);
		SetWarpT(Texture::Wrap::CLAMP);

		return true;
	}

	virtual void Update()
	{
		/*
		static int test = 0;
		if (ImGui::Begin("SuperGameObject"))
		{
			for (auto& v : bValues)
			{
				ImGui::Checkbox(v.first, &v.second);
			}

			for (auto& v : iValues)
			{
				ImGui::SliderInt(v.first, &v.second.value, v.second.min, v.second.max);
			}

			for (auto& v : fValues)
			{
				ImGui::SliderFloat(v.first, &v.second.value, v.second.min, v.second.max);
			}

			for (auto& v : vec4Values)
			{
				float vvv[4] = { v.second.value[0], v.second.value[1], v.second.value[2], v.second.value[3] };

				ImGui::SliderFloat4(v.first, vvv, v.second.min, v.second.max);
			}
		}

		ImGui::End();
		*/
		Texture2D::UpdateData(&buffer[0]);
	}

	void AddValue(const char* name_, bool v)
	{
		bValues[name_] = v;
	}

	void SetValue(const char* name_, bool v_)
	{
		bValues[name_] = v_;
	}

	void GetValue(const char* name_, bool& value_)
	{
		value_ = bValues[name_];
	}

	void AddValue(const char* name_, int v_, int min_, int max_)
	{
		iValues[name_] = { v_, min_, max_ };
	}

	void SetValue(const char* name_, int v_)
	{
		iValues[name_].value = v_;
	}

	void GetValue(const char* name_, int& value_)
	{
		value_ = iValues[name_].value;
	}

	void AddValue(const char* name_, float v_, float min_, float max_)
	{
		fValues[name_] = { v_, min_, max_ };
	}

	void SetValue(const char* name_, float v_)
	{
		fValues[name_].value = v_;
	}

	void GetValue(const char* name_, float& value_)
	{
		value_ = fValues[name_].value;
	}

	void AddValue(const char* name_, Vector4 v_, float min_, float max_)
	{
		vec4Values[name_] = { v_, min_, max_ };
	}

	void SetValue(const char* name_, Vector4 v_)
	{
		vec4Values[name_].value = v_;
	}

	void GetValue(const char* name_, Vector4& value_)
	{
		value_ = vec4Values[name_].value;
	}
private:
private:
	struct IValue
	{
		int value;
		int min;
		int max;
	};

	struct FValue
	{
		float value;
		float min;
		float max;
	};

	struct Vec4Value
	{
		Vector4 value;
		float min;
		float max;
	};

	std::map<const char*, bool> bValues;
	std::map<const char*, IValue> iValues;
	std::map<const char*, FValue> fValues;
	std::map<const char*, Vec4Value> vec4Values;

	std::vector<char> buffer;
};


class WebcamTexture : public Texture2D
{
public:
	WebcamTexture()
		: Texture2D()
		, buffer(1280 * 720 * 4)
	{
	}

	virtual ~WebcamTexture()
	{
	}

	bool Create(bool vflip_)
	{
		return Texture2D::Create(1280, 720, 4, false, &buffer[0], vflip_);
	}

	virtual void Update()
	{
		Texture2D::UpdateData(&buffer[0]);
	}
private:
private:
	std::vector<char> buffer;
};

class MicrophoneTexture : public Texture2D
{
public:
	MicrophoneTexture()
		: Texture2D()
		, buffer(1280 * 720 * 4)
	{
	}

	virtual ~MicrophoneTexture()
	{
	}

	bool Create()
	{
		return Texture2D::Create(512, 2, 1, false, &buffer[0]);
	}

	virtual void Update()
	{
		Texture2D::UpdateData(&buffer[0]);
	}
private:
private:
	std::vector<char> buffer;
};

class SoundCloudTexture : public Texture2D
{
public:
	SoundCloudTexture()
		: Texture2D()
		, buffer(1280 * 720 * 4)
	{
	}

	virtual ~SoundCloudTexture()
	{
	}

	bool Create(const std::string& url, bool vflip_)
	{
		return Texture2D::Create(512, 2, 1, false, &buffer[0], vflip_);
	}

	virtual void Update()
	{
		Texture2D::UpdateData(&buffer[0]);
	}
private:
private:
	std::vector<char> buffer;
};

class VideoTexture : public Texture2D
{
public:
	VideoTexture()
		: Texture2D()
		, buffer(1280 * 720 * 4)
	{
	}

	virtual ~VideoTexture()
	{
	}

	bool Create(const std::string& url, bool vflip_)
	{
		return Texture2D::Create(1280, 720, 4, false, &buffer[0], vflip_);
	}

	virtual void Update()
	{
		Texture2D::UpdateData(&buffer[0]);
	}
private:
private:
	std::vector<char> buffer;
};

class FlipFrameBuffer : public FrameBuffer
{
public:
	FlipFrameBuffer()
		: FrameBuffer()
		, current(0)
	{
	}

	virtual ~FlipFrameBuffer()
	{
	}

	virtual void Flip()
	{
		current = 1 - current;
	}

	virtual Texture* GetCurrentTexture() = 0;
protected:
	int GetCurrent()
	{
		return current;
	}
private:
	int current;
};

class FlipTexture2DFrameBuffer : public FlipFrameBuffer
{
public:
	FlipTexture2DFrameBuffer()
		: FlipFrameBuffer()
	{
	}

	virtual ~FlipTexture2DFrameBuffer()
	{
	}

	virtual bool Create(unsigned int width, unsigned int height, unsigned int nrComponents, bool isHDR)
	{
		if (!FlipFrameBuffer::Create())
			return false;

		if (!texture[0].Create(width, height, nrComponents, isHDR, nullptr))
			return false;

		if (!texture[1].Create(width, height, nrComponents, isHDR, nullptr))
			return false;

		SetColorAttachment(FrameBuffer::Attachment::COLOR_ATTACHMENT0, &texture[0]);
		return true;
	}

	virtual void Destroy()
	{
		for (int i = 0; i < 2; i++)
			texture[i].Destroy();

		return FlipFrameBuffer::Destroy();
	}

	Texture* GetCurrentTexture()
	{
		return &texture[1 - GetCurrent()];
	}

	virtual void Flip()
	{
		FlipFrameBuffer::Flip();

		SetColorAttachment(FrameBuffer::Attachment::COLOR_ATTACHMENT0, &texture[GetCurrent()]);
	}
private:
	Texture2D texture[2];
};

class FlipTextureCubeMapFrameBuffer : public FlipFrameBuffer
{
public:
	FlipTextureCubeMapFrameBuffer()
		: FlipFrameBuffer()
	{
	}

	virtual ~FlipTextureCubeMapFrameBuffer()
	{
	}

	virtual bool Create(unsigned int size, unsigned int nrComponents, bool isHDR)
	{
		if (!FlipFrameBuffer::Create())
			return false;

		if (!texture[0].Create(size, nrComponents, isHDR, nullptr))
			return false;

		if (!texture[1].Create(size, nrComponents, isHDR, nullptr))
			return false;

		SetColorAttachment(FrameBuffer::Attachment::COLOR_ATTACHMENT0, &texture[0]);
		return true;
	}

	virtual void Destroy()
	{
		for (int i = 0; i < 2; i++)
			texture[i].Destroy();

		return FlipFrameBuffer::Destroy();
	}

	Texture* GetCurrentTexture()
	{
		return &texture[1 - GetCurrent()];
	}

	virtual void Flip()
	{
		FlipFrameBuffer::Flip();
		
		//Invalidate();
		SetColorAttachment(FrameBuffer::Attachment::COLOR_ATTACHMENT0, &texture[GetCurrent()]);
	}
private:
	TextureCubeMap texture[2];
};


#define SOUND 0
#define BUFFER_A 1
#define BUFFER_B 2
#define BUFFER_C 3
#define BUFFER_D 4
#define CUBEMAP_A 5
#define IMAGE 6

#define CHANNEL_COUNT 4

class Pass
{
public:
	enum Filter
	{
		Nearest,
		Linear,
		Mipmap
	};

	enum Wrap
	{
		Clamp,
		Repeat,
	};

	class Channel
	{
	public:
		Channel()
			: texture(nullptr)
			, frameBuffer(nullptr)
			, filter(Mipmap)
			, wrap(Repeat)
			, vFlip(true)
		{
		}

		void Clear()
		{
			texture = nullptr;
			frameBuffer = nullptr;
			filter = Mipmap;
			wrap = Repeat;
			vFlip = true;
		}

		Texture* texture;
		FlipFrameBuffer* frameBuffer;
		Filter filter;
		Wrap wrap;
		bool vFlip;
	};

	Pass()
		: enabled(true)
		, primitives()
		, shaderProgram()
		, iChannels(CHANNEL_COUNT)
		, frameBuffer(nullptr)
	{
	}

	virtual ~Pass()
	{
	}

	bool Create()
	{
		float vertices[] =
		{
			1.0f,  1.0f, 0.0f,  // top right
			1.0f, -1.0f, 0.0f,  // bottom right
			-1.0f, -1.0f, 0.0f,  // bottom left

			-1.0f, -1.0f, 0.0f,  // bottom left
			1.0f,  1.0f, 0.0f,  // top right
			-1.0f,  1.0f, 0.0f   // top left 
		};

		bool success =
			primitives
			.Begin()
			.FillVertices(0, 3, VertexAttribute::DataType::FLOAT, false, 0, 0, vertices, sizeof(vertices) / sizeof(vertices[0]) / 3)
			.End();
		if(!success)
		{
			return false;
		}

		for (int i = 0; i < iChannels.size(); i++)
			iChannels[i].Clear();

		frameBuffer = nullptr;

		return true;
	}

	bool Update(unsigned int width, unsigned height, double time, double deltaTime, Vector4 mouse, Vector2 mouseDelta, int frameCounter)
	{
		//int facecount = 1;
		Vector3 resolution = Vector3(SCR_WIDTH, SCR_HEIGHT, 1.0);
		if (frameBuffer)
		{
			//if (frameBuffer->GetColorAttachment(GL_COLOR_ATTACHMENT0)->GetType() == GL_TEXTURE_CUBE_MAP)
				//facecount = 6;
			frameBuffer->Bind();

			frameBuffer->GetColorAttachment(FrameBuffer::Attachment::COLOR_ATTACHMENT0)->GetResolution(resolution);

			renderStates.viewportState.pos = Vector2(0, 0);
			renderStates.viewportState.size = Vector2(resolution[0], resolution[1]);

			renderStates.scissorTestState.enabled = true;
			renderStates.scissorTestState.pos = Vector2(0, 0);
			renderStates.scissorTestState.size = Vector2(resolution[0], resolution[1]);
		}
		else
		{
			renderStates.viewportState.pos = Vector2(0, 0);
			renderStates.viewportState.size = Vector2(width, height);

			renderStates.scissorTestState.enabled = true;
			renderStates.scissorTestState.pos = Vector2(0, 0);
			renderStates.scissorTestState.size = Vector2(width, height);
		}

		shaderProgram.Bind();
		shaderProgram.SetUniform3f("iResolution", resolution[0], resolution[1], resolution[2]);
		shaderProgram.SetUniform1f("iTime", (float)time);
		shaderProgram.SetUniform1f("iTimeDelta", (float)deltaTime);
		shaderProgram.SetUniform1f("iFrameRate", 60.0f);
		shaderProgram.SetUniform1i("iFrame", frameCounter);
		shaderProgram.SetUniform4f("iMouse", mouse.X(), mouse.Y(), mouse.Z(), mouse.W());

		printf("%f %f %f %f\n", mouse.X(), mouse.Y(), mouse.Z(), mouse.W());
		SYSTEMTIME lt = { 0 };
		GetLocalTime(&lt);
		shaderProgram.SetUniform4f("iDate", (float)lt.wYear-1, (float)lt.wMonth-1, (float)lt.wDay, lt.wHour*60.0f *60.0f + lt.wMinute*60.0f + lt.wSecond);
		shaderProgram.SetUniform1f("iSampleRate", 48000.0);

		std::vector<Vector3> channelResolutions(CHANNEL_COUNT);
		std::vector<float> channelTimes(CHANNEL_COUNT);
		for (int i = 0; i < iChannels.size(); i++)
		{
			Texture* texture = nullptr;
			if (iChannels[i].texture)
				texture = iChannels[i].texture;
			else if (iChannels[i].frameBuffer)
				texture = iChannels[i].frameBuffer->GetCurrentTexture();

			if (texture)
			{
				texture->GetResolution(channelResolutions[i]);
				channelTimes[i] = 0.0;

				if (iChannels[i].wrap == Pass::Wrap::Repeat)
				{
					texture->SetWarpS(Texture::Wrap::REPEAT);
					texture->SetWarpR(Texture::Wrap::REPEAT);
					texture->SetWarpT(Texture::Wrap::REPEAT);
				}
				else if (iChannels[i].wrap == Pass::Wrap::Clamp)
				{
					texture->SetWarpS(Texture::Wrap::CLAMP);
					texture->SetWarpR(Texture::Wrap::CLAMP);
					texture->SetWarpT(Texture::Wrap::CLAMP);
				}

				if (iChannels[i].filter == Pass::Filter::Nearest)
				{
					texture->SetMinFilter(Texture::MinFilter::NEAREST);
					texture->SetMagFilter(Texture::MagFilter::NEAREST);
				}
				else if (iChannels[i].filter == Pass::Filter::Linear)
				{
					texture->SetMinFilter(Texture::MinFilter::LINEAR);
					texture->SetMagFilter(Texture::MagFilter::LINEAR);
				}
				else if (iChannels[i].filter == Pass::Filter::Mipmap)
				{
					texture->SetMinFilter(Texture::MinFilter::LINEAR_MIPMAP_LINEAR);
					texture->SetMagFilter(Texture::MagFilter::LINEAR);
				}

				texture->Bind(i);
			}
			else
			{
				channelResolutions[i] = Vector3(0.0, 0.0, 0.0);
				channelTimes[i] = 0.0;
			}
		}

		shaderProgram.SetUniform3fv("iChannelResolution", CHANNEL_COUNT, &channelResolutions[0][0]);
		shaderProgram.SetUniform1fv("iChannelTime", CHANNEL_COUNT, &channelTimes[0]);

		for (int i = 0; i < CHANNEL_COUNT; i++)
		{
			std::string name = "iChannel";
			name += ('0' + i);

			shaderProgram.SetUniform1i(name.c_str(), i);
		}

		primitives.Bind();
		primitives.DrawArray(Primitives::Mode::TRIANGLES, 0, primitives.GetCount());

		if (frameBuffer)
		{
			frameBuffer->UnBind();
		}

		return true;
	}

	void Flip()
	{
		if (frameBuffer)
		{
			frameBuffer->Flip();
		}
	}

	void Destroy()
	{
	}

	void SetEnabled(bool enabled_)
	{
		this->enabled = enabled_;
	}

	bool GetEnabled() const
	{
		return enabled;
	}

	void SetFilter(int i, Filter filter_)
	{
		this->iChannels[i].filter = filter_;
	}

	void SetWrap(int i, Wrap wrap_)
	{
		this->iChannels[i].wrap = wrap_;
	}

	void SetVFlip(int i, bool vFlip_)
	{
		this->iChannels[i].vFlip = vFlip_;
	}

	void SetChannelTexture(int i, Texture* texture_)
	{
		this->iChannels[i].texture = texture_;
		this->iChannels[i].frameBuffer = nullptr;
	}

	void SetChannelFrameBuffer(int i, FlipFrameBuffer* frameBuffer_)
	{
		this->iChannels[i].texture = nullptr;
		this->iChannels[i].frameBuffer = frameBuffer_;
	}

	const Pass::Channel& GetChannel(int i) const
	{
		return this->iChannels[i];
	}

	void SetFrameBuffer(FlipFrameBuffer* frameBuffer_)
	{
		frameBuffer = frameBuffer_;
	}

	FlipFrameBuffer* GetFrameBuffer() const
	{
		return frameBuffer;
	}

	bool LoadShader(const char* path_, std::string& fShaderCode)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			fShaderFile.open(path_);
			std::stringstream fShaderStream;

			// read file's buffer contents into streams
			fShaderStream << fShaderFile.rdbuf();

			// close file handlers
			fShaderFile.close();

			// convert stream into string
			fShaderCode = fShaderStream.str();
		}
		catch (std::ifstream::failure&)
		{
			return false;
		}

		return true;
	}

	bool SetShader(const char* folder_, const char* fShaderURL_, const char* commonShaderURL_)
	{
		std::string vShaderCode =
			"#version 330 core\n"
			"layout(location = 0) in vec3 aPos;\n"
			"out vec2 fragCoord;\n"
			"uniform vec3 iResolution;\n"
			"uniform float iTime;\n"
			"uniform float iTimeDelta;\n"
			"uniform int iFrame;\n"
			"uniform float iChannelTime[4];\n"
			"uniform vec4 iMouse;\n"
			"uniform vec4 iDate;\n"
			"uniform float iSampleRate;\n"
			"uniform vec3 iChannelResolution[4];\n"
			"void main()\n"
			"{\n"
			"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"fragCoord = (vec2(aPos.x, aPos.y) + 1.0) / 2.0 * iResolution.xy;\n"
			"}\n";

		std::string fShaderHeader =
			"#version 330 core\n"
			"precision highp float;\n"
			"precision highp int;\n"
			"in vec2 fragCoord;\n"
			"out vec4 FragColor;\n"
			"uniform vec3 iResolution;\n"
			"uniform float iTime;\n"
			"uniform float iTimeDelta;\n"
			"uniform int iFrame;\n"
			"uniform float iChannelTime[4];\n"
			"uniform vec4 iMouse;\n"
			"uniform vec4 iDate;\n"
			"uniform float iSampleRate;\n"
			"uniform vec3 iChannelResolution[4];\n";

		std::string fShaderChannels = "";
		for (int i = 0; i < iChannels.size(); i++)
		{
			std::string idx = "0";
			idx[0] += i;

			if (iChannels[i].texture)
			{
				if (iChannels[i].texture->GetType() == Texture::Type::TEXTURE_2D)
					fShaderChannels += "uniform sampler2D iChannel" + idx + ";\n";
				else if (iChannels[i].texture->GetType() == Texture::Type::TEXTURE_CUBE_MAP)
					fShaderChannels += "uniform samplerCube iChannel" + idx + ";\n";
			}
			else if (iChannels[i].frameBuffer)
			{
				if (iChannels[i].frameBuffer->GetCurrentTexture()->GetType() == Texture::Type::TEXTURE_2D)
					fShaderChannels += "uniform sampler2D iChannel" + idx + ";\n";
				else if (iChannels[i].frameBuffer->GetCurrentTexture()->GetType() == Texture::Type::TEXTURE_CUBE_MAP)
					fShaderChannels += "uniform samplerCube iChannel" + idx + ";\n";
			}
		}

		std::string fShaderMain =
			"void main()\n"
			"{\n"
			"vec4 fragColor; \n"
			"mainImage(fragColor, fragCoord);\n"
			"FragColor = fragColor;\n"
			"}\n";

		std::string fShaderCode;
		if (fShaderURL_)
		{
			std::string url;
			url = folder_;
			url += "/";
			url += fShaderURL_;
			if (!LoadShader(url.c_str(), fShaderCode))
			{
				Platform::Debug("failed to load shader\n", url.c_str());
				return false;
			}
		}

		std::string commonShaderCode;
		if (commonShaderURL_)
		{
			std::string url;
			url = folder_;
			url += "/";
			url += commonShaderURL_;
			if (!LoadShader(url.c_str(), commonShaderCode))
			{
				Platform::Debug("failed to load shader\n", url.c_str());
				return false;
			}
		}

		std::string fShader = fShaderHeader + "\n" +
			fShaderChannels + "\n" +
			commonShaderCode + "\n" +
			fShaderCode + "\n" +
			fShaderMain;
		{
			FILE* fptr = fopen("test.log", "wt");
			if (fptr)
			{
				fprintf(fptr, "%s\n", fShader.c_str());
				fclose(fptr);
			}
		}
		return shaderProgram.CreateFromBuffer(vShaderCode.c_str(), fShader.c_str());
	}
protected:
private:

public:
protected:
private:
	bool enabled;

	RenderStates renderStates;
	ShaderProgram shaderProgram;
	std::vector<Channel> iChannels;
	FlipFrameBuffer* frameBuffer;
	Primitives primitives;
};

class MacShaderDemo
{
public:
	MacShaderDemo()
		: textures()
		, black()
		, soundFrameBuffer()
		, bufferAFrameBuffer()
		, bufferBFrameBuffer()
		, bufferCFrameBuffer()
		, bufferDFrameBuffer()
		, cubeMapAFrameBuffer()
		, cubeMapBFrameBuffer()
		, cubeMapCFrameBuffer()
		, cubeMapDFrameBuffer()
		, passes()
	{
	}

	virtual ~MacShaderDemo()
	{
	}

	std::string ToLower(const char* str)
	{
		std::string result = str;

		for (int i = 0; i < result.size(); i++)
		{
			result[i] = tolower(result[i]);
		}

		return result;
	}

	Pass::Filter GetFilter(const char* str)
	{
		std::string s = ToLower(str);

		if (s == "nearest")
		{
			return Pass::Filter::Nearest;
		}
		else if (s == "linear")
		{
			return Pass::Filter::Linear;
		}
		else// if (s == "mipmap")
		{
			return Pass::Filter::Mipmap;
		}
	}

	Pass::Wrap GetWrap(const char* str)
	{
		std::string s = ToLower(str);

		if (s == "clamp")
		{
			return Pass::Wrap::Clamp;
		}
		else// if (s == "repeat")
		{
			return Pass::Wrap::Repeat;
		}
	}

	FlipFrameBuffer* GetFrameBuffer(const char* str)
	{
		std::string s = ToLower(str);

		if (s == "sound")
		{
			return &soundFrameBuffer;
		}
		else if (s == "buffera")
		{
			return &bufferAFrameBuffer;
		}
		else if (s == "bufferb")
		{
			return &bufferBFrameBuffer;
		}
		else if (s == "bufferc")
		{
			return &bufferCFrameBuffer;
		}
		else if (s == "bufferd")
		{
			return &bufferDFrameBuffer;
		}
		else if (s == "cubemapa")
		{
			return &cubeMapAFrameBuffer;
		}
		else if (s == "cubemapb")
		{
			return &cubeMapBFrameBuffer;
		}
		else if (s == "cubemapc")
		{
			return &cubeMapCFrameBuffer;
		}
		else if (s == "cubemapd")
		{
			return &cubeMapDFrameBuffer;
		}
		else
			return nullptr;
	}

	Texture* AddKeyboardTexture()
	{
		KeyboardTexture* texture = new KeyboardTexture();
		if (!texture)
			return nullptr;
		if (!texture->Create())
		{
			delete texture;
			return nullptr;
		}

		textures.push_back(texture);
		return texture;
	}

	GUITexture* AddGUITexture()
	{
		GUITexture* texture = new GUITexture();
		if (!texture)
			return nullptr;
		if (!texture->Create())
		{
			delete texture;
			return nullptr;
		}

		textures.push_back(texture);
		return texture;
	}
	
	Texture* AddWebcamTexture(bool vflip_)
	{
		WebcamTexture* texture = new WebcamTexture();
		if (!texture)
			return nullptr;
		if (!texture->Create(vflip_))
		{
			delete texture;
			return nullptr;
		}

		textures.push_back(texture);
		return texture;
	}

	Texture* AddMicrophoneTexture()
	{
		MicrophoneTexture* texture = new MicrophoneTexture();
		if (!texture)
			return nullptr;
		if (!texture->Create())
		{
			delete texture;
			return nullptr;
		}

		textures.push_back(texture);
		return texture;
	}

	Texture* AddSoundCloudTexture(const char* url_, bool vflip_)
	{
		SoundCloudTexture* texture = new SoundCloudTexture();
		if (!texture)
			return nullptr;
		if (!texture->Create(url_, vflip_))
		{
			delete texture;
			return nullptr;
		}

		textures.push_back(texture);
		return texture;
	}

	Texture* AddTexture2D(const char* path, bool vflip_)
	{
		Texture2DFile* texture = new Texture2DFile();
		if (!texture)
			return nullptr;
		if (!texture->Create(path, vflip_))
		{
			delete texture;
			return nullptr;
		}

		textures.push_back(texture);
		return texture;
	}

	Texture* AddTextureCubemap(const char* path, bool vflip_)
	{
		TextureCubeMapFile* texture = new TextureCubeMapFile();
		if (!texture)
			return nullptr;
		if (!texture->Create(path, vflip_))
		{
			delete texture;
			return nullptr;
		}

		textures.push_back(texture);
		return texture;
	}

	Texture* AddVideoTexture(const char* path, bool vflip_)
	{
		VideoTexture* texture = new VideoTexture();
		if (!texture)
			return nullptr;
		if (!texture->Create(path, vflip_))
		{
			delete texture;
			return nullptr;
		}

		textures.push_back(texture);
		return texture;
	}
	void DebugLog(const char* msg)
	{
		Platform::Debug("Error: %s\n", msg);
	}

	bool CreateScene(const char* folder_, const char* scenefile_)
	{
		std::vector<char> colors(32 * 32 * 4);
		memset(&colors[0], 0, (32 * 32 * 4));

		if (!black.Create(32, 32, 4, false, &colors[0], false))
			return false;
		if (!soundFrameBuffer.Create(512, 2, 1, true))
			return false;
		if (!bufferAFrameBuffer.Create(SCR_WIDTH, SCR_HEIGHT, 4, true))
			return false;
		if (!bufferBFrameBuffer.Create(SCR_WIDTH, SCR_HEIGHT, 4, true))
			return false;
		if (!bufferCFrameBuffer.Create(SCR_WIDTH, SCR_HEIGHT, 4, true))
			return false;
		if (!bufferDFrameBuffer.Create(SCR_WIDTH, SCR_HEIGHT, 4, true))
			return false;
		if (!cubeMapAFrameBuffer.Create(1024, 4, true))
			return false;
		if (!cubeMapBFrameBuffer.Create(1024, 4, true))
			return false;
		if (!cubeMapCFrameBuffer.Create(1024, 4, true))
			return false;
		if (!cubeMapDFrameBuffer.Create(1024, 4, true))
			return false;

		std::string folder = folder_;

		// 1. retrieve the vertex/fragment source code from filePath
		std::string url;
		std::string shaderToyCode;
		std::ifstream shaderToyFile;
		// ensure ifstream objects can throw exceptions:
		shaderToyFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			url = folder;
			url += "/";
			url += scenefile_;

			// open files
			shaderToyFile.open(url.c_str());
			std::stringstream shaderToyStream;

			// read file's buffer contents into streams
			shaderToyStream << shaderToyFile.rdbuf();

			// close file handlers
			shaderToyFile.close();

			// convert stream into string
			shaderToyCode = shaderToyStream.str();
		}
		catch (std::ifstream::failure&)
		{
			printf("Failed to Open %s\n", url.c_str());
			return false;
		}

		Document shaderToyDoc;
		shaderToyDoc.Parse(shaderToyCode.c_str());

		const char* commonShaderURL = nullptr;
		if (shaderToyDoc.HasMember("common"))
		{
			Value& commonsJson = shaderToyDoc["common"];

			if (commonsJson.HasMember("shader"))
			{
				commonShaderURL = commonsJson["shader"].GetString();
			}
		}
		if (shaderToyDoc.HasMember("passes"))
		{
			Value& passesJson = shaderToyDoc["passes"];
			if (passesJson.IsArray())
			{
				passes.resize(passesJson.Size());
				for (int i = 0; i < passes.size(); i++)
				{
					if (!passes[i].Create())
					{
						printf("Failed to Create Pass %d\n", i);
						return false;
					}

					Value& passJson = passesJson[i];
					if (passJson.IsObject())
					{
						if (passJson.HasMember("rendertarget"))
						{
							std::string rendertargetname = passJson["rendertarget"].GetString();

							if (rendertargetname != "image")
							{
								FlipFrameBuffer* rendertarget = GetFrameBuffer(rendertargetname.c_str());
								if (!rendertarget)
								{
									printf("rendertarget=%s not supported\n", rendertargetname.c_str());
									return false;
								}

								passes[i].SetFrameBuffer(rendertarget);
							}
							else
							{
								passes[i].SetFrameBuffer(nullptr);
							}
						}
						else
						{
							printf("Pass must have Render Target\n");
							return false;
						}

						for (int j = 0; j < 4; j++)
						{
							passes[i].SetChannelTexture(j, &black);

							std::string name = "ichannel";
							name += ('0' + j);

							if (passJson.HasMember(name.c_str()))
							{
								Value& channelJson = passJson[name.c_str()];
								if (channelJson.IsObject())
								{
									if (channelJson.HasMember("filter"))
									{
										Pass::Filter channelFilter = GetFilter(channelJson["filter"].GetString());
										passes[i].SetFilter(j, channelFilter);
									}
									if (channelJson.HasMember("wrap"))
									{
										Pass::Wrap channelWrap = GetWrap(channelJson["wrap"].GetString());
										passes[i].SetWrap(j, channelWrap);
									}
									if (channelJson.HasMember("vflip"))
									{
										bool vFlip = channelJson["vflip"].GetBool();
										passes[i].SetVFlip(j, vFlip);
									}

									if (channelJson.HasMember("keyboard"))
									{
										Texture* texture = AddKeyboardTexture();
										if (!texture)
											return false;

										passes[i].SetChannelTexture(j, texture);
									}
									if (channelJson.HasMember("gui"))
									{
										Value& guisJson = channelJson["gui"];

										if (guisJson.IsArray())
										{
											GUITexture* guiTexture = AddGUITexture();
											if (!guiTexture)
												return false;
											passes[i].SetChannelTexture(j, guiTexture);

											for (unsigned k = 0; k < guisJson.Size(); k++)
											{
												if (guisJson[k].IsObject())
												{
													if (guisJson[k].HasMember("checkbox"))
													{
														if (guisJson[k].HasMember("name") && guisJson[k].HasMember("value"))
														{
															guiTexture->AddValue(guisJson[k]["name"].GetString(), guisJson[k]["value"].GetBool());
														}
													}
													else if (guisJson[k].HasMember("intslider"))
													{
														if (guisJson[k].HasMember("name") && guisJson[k].HasMember("value") && guisJson[k].HasMember("min") && guisJson[k].HasMember("max"))
														{
															guiTexture->AddValue(guisJson[k]["name"].GetString(), guisJson[k]["value"].GetInt(), guisJson[k]["min"].GetInt(), guisJson[k]["max"].GetInt());
														}
													}
													else if (guisJson[k].HasMember("floatslider"))
													{
														if (guisJson[k].HasMember("name") && guisJson[k].HasMember("value") && guisJson[k].HasMember("min") && guisJson[k].HasMember("max"))
														{
															guiTexture->AddValue(guisJson[k]["name"].GetString(), guisJson[k]["value"].GetFloat(), guisJson[k]["min"].GetFloat(), guisJson[k]["max"].GetFloat());
														}
													}
													else if (guisJson[k].HasMember("vec4slider"))
													{
														if (guisJson[k].HasMember("name") && guisJson[k].HasMember("value") && guisJson[k].HasMember("min") && guisJson[k].HasMember("max"))
														{
															if (guisJson[k]["value"].IsArray() && guisJson[k]["value"].Size()==4)
															{
																Vector4 v;
																v[0] = guisJson[k]["value"][0].GetFloat();
																v[1] = guisJson[k]["value"][1].GetFloat();
																v[2] = guisJson[k]["value"][2].GetFloat();
																v[3] = guisJson[k]["value"][3].GetFloat();
																guiTexture->AddValue(guisJson[k]["name"].GetString(), v, guisJson[k]["min"].GetFloat(), guisJson[k]["max"].GetFloat());
															}
														}
													}
												}
											}
										}
										else
										{
											printf("gui is not an object\n");
										}
									}
									else if (channelJson.HasMember("webcam"))
									{
										Texture* texture = AddWebcamTexture(passes[i].GetChannel(j).vFlip);
										if (!texture)
											return false;

										passes[i].SetChannelTexture(j, texture);
									}
									else if (channelJson.HasMember("microphone"))
									{
										Texture* texture = AddMicrophoneTexture();
										if (!texture)
											return false;

										passes[i].SetChannelTexture(j, texture);
									}
									else if (channelJson.HasMember("soundcloud"))
									{
										std::string url = channelJson["soundcloud"].GetString();

										Texture* texture = AddSoundCloudTexture(url.c_str(), passes[i].GetChannel(j).vFlip);
										if (!texture)
										{
											printf("channel %d: failed to load soundcloud %s\n", j, url.c_str());
											return false;
										}

										passes[i].SetChannelTexture(j, texture);
									}
									else if (channelJson.HasMember("texture2d"))
									{
										std::string url = folder;
										url += "/";
										url += channelJson["texture2d"].GetString();

										Texture* texture = AddTexture2D(url.c_str(), passes[i].GetChannel(j).vFlip);
										if (!texture)
										{
											printf("channel %d: failed to load texture2d %s\n", j, url.c_str());
											return false;
										}

										passes[i].SetChannelTexture(j, texture);
									}
									else if (channelJson.HasMember("texturecubemap"))
									{
										std::string url = folder;
										url += "/";
										url += channelJson["texturecubemap"].GetString();

										Texture* texture = AddTextureCubemap(url.c_str(), passes[i].GetChannel(j).vFlip);
										if (!texture)
										{
											printf("channel %d: failed to load texturecubemap %s\n", j, url.c_str());
											return false;
										}

										passes[i].SetChannelTexture(j, texture);
									}
									else if (channelJson.HasMember("texturevideo"))
									{
										std::string url = folder;
										url += "/";
										url += channelJson["texturevideo"].GetString();

										Texture* texture = AddVideoTexture(url.c_str(), passes[i].GetChannel(j).vFlip);
										if (!texture)
										{
											printf("channel %d: failed to load texture video %s\n", j, url.c_str());
											return false;
										}

										passes[i].SetChannelTexture(j, texture);
									}
									else if (channelJson.HasMember("buffer"))
									{
										std::string buffername = channelJson["buffer"].GetString();

										FlipFrameBuffer* fb = GetFrameBuffer(buffername.c_str());
										if (!fb)
										{
											printf("channel %d: buffer=%s is not supported\n", j, buffername.c_str());
											return false;
										}

										passes[i].SetChannelFrameBuffer(j, fb);
									}
									else
									{
										printf("channel%d: must have texture or frame buffer specified or texture type is not supported\n", j);
										return false;
									}
								}
							}
						}
						if (passJson.HasMember("shader"))
						{
							const char* shaderURL = passJson["shader"].GetString();
							if (!passes[i].SetShader(folder.c_str(), shaderURL, commonShaderURL))
							{
								return false;
							}
						}
						else
						{
							printf("channel%d: must have shader specified\n", i);
						}
					}
				}
			}
		}

		return true;
	}


	bool Create(const char* folder_)
	{
		if (!CreateScene(folder_, "scene.json"))
			return false;

		return true;
	}

	bool Update(unsigned int width, unsigned int height, double time, double deltaTime, Vector4 mouse, Vector2 mouseDelta, int frameCounter)
	{
		for (auto& texture : textures)
		{
			texture->Update();
		}

		for (auto& pass : passes)
		{
			if (!pass.Update(width, height, time, deltaTime, mouse, mouseDelta, frameCounter))
				return false;

			pass.Flip();
		}

		return true;
	}

	void Destroy()
	{
		for (auto texture : textures)
		{
			if (texture)
			{
				texture->Destroy();

				delete texture;
				texture = nullptr;
			}
		}
		textures.clear();

		black.Destroy();

		soundFrameBuffer.Destroy();
		bufferAFrameBuffer.Destroy();
		bufferBFrameBuffer.Destroy();
		bufferCFrameBuffer.Destroy();
		bufferDFrameBuffer.Destroy();
		cubeMapAFrameBuffer.Destroy();
		cubeMapBFrameBuffer.Destroy();
		cubeMapCFrameBuffer.Destroy();
		cubeMapDFrameBuffer.Destroy();
	}
protected:
private:
	std::vector<Texture*> textures;
	Texture2D black;

	FlipTexture2DFrameBuffer soundFrameBuffer;
	FlipTexture2DFrameBuffer bufferAFrameBuffer;
	FlipTexture2DFrameBuffer bufferBFrameBuffer;
	FlipTexture2DFrameBuffer bufferCFrameBuffer;
	FlipTexture2DFrameBuffer bufferDFrameBuffer;
	FlipTextureCubeMapFrameBuffer cubeMapAFrameBuffer;
	FlipTextureCubeMapFrameBuffer cubeMapBFrameBuffer;
	FlipTextureCubeMapFrameBuffer cubeMapCFrameBuffer;
	FlipTextureCubeMapFrameBuffer cubeMapDFrameBuffer;

	std::vector<Pass> passes;
};

class MacShaderDemoApp : public FrameWork
{
public:
	MacShaderDemoApp()
		: FrameWork("MacShaderDemo")
		, macShaderDemo()
	{
	}

	virtual ~MacShaderDemoApp()
	{
	}

	virtual bool OnCreate() override
	{
		//return macShaderDemo.Create("Demos/Noise/Perlin");//
		//return macShaderDemo.Create("Demos/Clouds/Cheap Cloud Flythrough");//
		//return macShaderDemo.Create("Demos/Clouds/Cloud");//
		//return macShaderDemo.Create("Demos/Clouds/CloudFight");//
		//return macShaderDemo.Create("Demos/default");
		//return macShaderDemo.Create("Demos/Greek Temple");
		//return macShaderDemo.Create("Demos/JustForFuns/Hexagonal Grid Traversal - 3D");		
		//return macShaderDemo.Create("Demos/JustForFuns/MO");
		//return macShaderDemo.Create("Demos/PathTracings/Bidirectional path tracing");
		//return macShaderDemo.Create("Demos/PathTracings/Demofox Path Tracing 1");
		//return macShaderDemo.Create("Demos/PathTracings/Demofox Path Tracing 2");
		//return macShaderDemo.Create("Demos/PathTracings/Path Tracer MIS");
		//return macShaderDemo.Create("Demos/PathTracings/PBR Material Gold");
		//return macShaderDemo.Create("Demos/PathTracings/Room DI");
		//return macShaderDemo.Create("Demos/Post process - SSAO");
		
		//return macShaderDemo.Create("Demos/Scattering/Atmospheric scattering explained");
		//return macShaderDemo.Create("Demos/Scattering/Atmospheric Scattering Fog");
		//return macShaderDemo.Create("Demos/Scattering/Fast Atmospheric Scattering");
		//return macShaderDemo.Create("Demos/Scattering/Fast Atmospheric Scattering");
		//return macShaderDemo.Create("Demos/Scattering/RayleighMieDayNight");
		//return macShaderDemo.Create("Demos/Scattering/RealySimpleAtmosphericScatter");
		//return macShaderDemo.Create("Demos/Terrains/Cloudy Terrain");
		//return macShaderDemo.Create("Demos/Terrains/Desert Sand");
		//return macShaderDemo.Create("Demos/Terrains/Elevated");
		//return macShaderDemo.Create("Demos/Terrains/Lake in highland");
		//return macShaderDemo.Create("Demos/Terrains/Mountains");
		//return macShaderDemo.Create("Demos/Terrains/Rainforest");
		//return macShaderDemo.Create("Demos/Terrains/Sirenian Dawn");

		//return macShaderDemo.Create("Demos/Waters/RiverGo");
		//return macShaderDemo.Create("Demos/Waters/Oceanic");
		//return macShaderDemo.Create("Demos/Waters/Ocean");
		//return macShaderDemo.Create("Demos/Waters/Very fast procedural ocean");
		//return macShaderDemo.Create("Demos/Waters/Water World");
		//return macShaderDemo.Create("Demos/Wave Propagation Effect");
		//return macShaderDemo.Create("Demos/Beneath the Sea God Ray");
		return macShaderDemo.Create("Demos/Scattering/VolumetricIntegration");
		return macShaderDemo.Create("Demos/Waters/Spout");
	}

	virtual bool OnUpdate() override
	{
		return macShaderDemo.Update(GetWidth(), GetHeight(), GetTime(), GetDeltaTime(), GetMouse(), GetMouseDelta(), GetFrameCounter());
	}

	void OnDestroy() override
	{
		return macShaderDemo.Destroy();
	}
private:
	MacShaderDemo macShaderDemo;
};

int main(int argc, char** argv)
{
	MacShaderDemoApp macShaderDemoApp;

	if (!macShaderDemoApp.Create(SCR_WIDTH, SCR_HEIGHT))
		return -1;

	macShaderDemoApp.Start();

	macShaderDemoApp.Destroy();

	return 0;
}