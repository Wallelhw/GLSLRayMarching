#include "FrameWork.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "VertexArrayObject.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;
#include <iostream>

// https://shadertoyunofficial.wordpress.com/2016/07/20/special-shadertoy-features/

#define SCR_WIDTH 800
#define SCR_HEIGHT 400

class KeyboardTexture : public Texture2D
{
public:
	KeyboardTexture()
		: Texture2D()
		, buffer(256)
	{
	}

	virtual ~KeyboardTexture()
	{
	}

	bool Create()
	{
		return Texture2D::Create(256, 1, 1, false, &buffer[0]);
	}

	virtual void Update()
	{
		Texture2D::Update(&buffer[0]);
	}
private:
private:
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

	bool Create()
	{
		return Texture2D::Create(1280, 720, 4, false, &buffer[0]);
	}

	virtual void Update()
	{
		Texture2D::Update(&buffer[0]);
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
		Texture2D::Update(&buffer[0]);
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

	bool Create(const std::string& url)
	{
		return Texture2D::Create(512, 2, 1, false, &buffer[0]);
	}

	virtual void Update()
	{
		Texture2D::Update(&buffer[0]);
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

	bool Create(const std::string& url)
	{
		return Texture2D::Create(1280, 720, 4, false, &buffer[0]);
	}

	virtual void Update()
	{
		Texture2D::Update(&buffer[0]);
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

		SetColorAttachment(GL_COLOR_ATTACHMENT0, &texture[0]);
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

		SetColorAttachment(GL_COLOR_ATTACHMENT0, &texture[GetCurrent()]);
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

		SetColorAttachment(GL_COLOR_ATTACHMENT0, &texture[0]);
		return true;
	}

	virtual void Destroy()
	{
		for(int i=0;i<2; i++)
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

		SetColorAttachment(GL_COLOR_ATTACHMENT0, &texture[GetCurrent()]);
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
		, vertexArrayObject()
		, shaderProgram()
		, iChannels(4)
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
			-1.0f,  1.0f, 0.0f   // top left 
		};
		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		if (!vertexArrayObject.Create(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0])))
		{
			return false;
		}

		for (int i = 0; i < iChannels.size(); i++)
			iChannels[i].Clear();

		frameBuffer = nullptr;

		return true;
	}

	bool Update(double time, double deltaTime, vec4 mouse, vec2 mouseDelta, int frameCounter)
	{
		vec3 resolution = vec3(SCR_WIDTH, SCR_HEIGHT, 1.0);
		if (frameBuffer)
		{
			frameBuffer->GetColorAttachment(GL_COLOR_ATTACHMENT0)->GetResolution(resolution);
			frameBuffer->Bind();
		}
		else
		{
			FrameBuffer::UnBind();
		}
		
		glViewport(0, 0, resolution[0], resolution[1]);
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 0, resolution[0], resolution[1]);
		//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		//glClearDepth(1.0f);
		//glClearStencil(0);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		shaderProgram.Bind();
		shaderProgram.SetUniform3f("iResolution", resolution[0], resolution[1], resolution[2]);
		shaderProgram.SetUniform1f("iTime", time);
		shaderProgram.SetUniform1f("iTimeDelta", deltaTime);
		shaderProgram.SetUniform1f("iFrameRate", 60.0f);
		shaderProgram.SetUniform1i("iFrame", frameCounter);
		shaderProgram.SetUniform4f("iMouse", mouse.X(), mouse.Y(), mouse.Z(), mouse.W());
		shaderProgram.SetUniform4f("iDate", 0.0, 0.0, 0.0, 0.0);
		shaderProgram.SetUniform1f("iSampleRate", 48000.0);

		vec3 channelResolutions[4];
		float channelTimes[4];
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
					texture->SetWarpS(GL_REPEAT);
					texture->SetWarpR(GL_REPEAT);
					texture->SetWarpT(GL_REPEAT);
				}
				else if (iChannels[i].wrap == Pass::Wrap::Clamp)
				{
					texture->SetWarpS(GL_CLAMP_TO_EDGE);
					texture->SetWarpR(GL_CLAMP_TO_EDGE);
					texture->SetWarpT(GL_CLAMP_TO_EDGE);
				}

				if (iChannels[i].filter == Pass::Filter::Nearest)
				{
					texture->SetMinFilter(GL_NEAREST);
					texture->SetMagFilter(GL_NEAREST);
				}
				else if (iChannels[i].filter == Pass::Filter::Linear)
				{
					texture->SetMinFilter(GL_LINEAR);
					texture->SetMagFilter(GL_LINEAR);
				}
				else if (iChannels[i].filter == Pass::Filter::Mipmap)
				{
					texture->SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
					texture->SetMagFilter(GL_LINEAR);
				}

				texture->Bind(i);
			}
			else
			{
				channelResolutions[i] = vec3(0.0, 0.0, 0.0);
				channelTimes[i] = 0.0;
			}
		}

		shaderProgram.SetUniform3fv("iChannelResolution", 4, &channelResolutions[0][0]);
		shaderProgram.SetUniform1fv("iChannelTime", 4, &channelTimes[0]);
		shaderProgram.SetUniform1i("iChannel0", 0);
		shaderProgram.SetUniform1i("iChannel1", 1);
		shaderProgram.SetUniform1i("iChannel2", 2);
		shaderProgram.SetUniform1i("iChannel3", 3);

		vertexArrayObject.Bind();
		vertexArrayObject.Draw(GL_TRIANGLES, 6);

		if (frameBuffer)
		{
			frameBuffer->UnBind();
			frameBuffer->Flip();
		}

		return true;
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

	void SetChannelTexture(int i, FlipFrameBuffer* frameBuffer_)
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
				if (iChannels[i].texture->GetType() == GL_TEXTURE_2D)
					fShaderChannels += "uniform sampler2D iChannel" + idx + ";\n";
				else if (iChannels[i].texture->GetType() == GL_TEXTURE_CUBE_MAP)
					fShaderChannels += "uniform samplerCube iChannel" + idx + ";\n";
			}
			else if (iChannels[i].frameBuffer)
			{
				if (iChannels[i].frameBuffer->GetCurrentTexture()->GetType() == GL_TEXTURE_2D)
					fShaderChannels += "uniform sampler2D iChannel" + idx + ";\n";
				else if (iChannels[i].frameBuffer->GetCurrentTexture()->GetType() == GL_TEXTURE_CUBE_MAP)
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

	VertexArrayObject vertexArrayObject;
	ShaderProgram shaderProgram;
	std::vector<Channel> iChannels;
	FlipFrameBuffer* frameBuffer;
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

	Texture* GetTexture(const char* folder_, const char* file_, const char* type_)
	{
		std::string folder;
		folder = folder_;
		folder += "/";

		std::string file = ToLower(file_);
		if (file == "keyboard")
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
		else if (file == "webcam")
		{
			WebcamTexture* texture = new WebcamTexture();
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
		else if (file == "microphone")
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
		else if (file == "soundcloud")
		{
			SoundCloudTexture* texture = new SoundCloudTexture();
			if (!texture)
				return nullptr;
			if (!texture->Create(file))
			{
				delete texture;
				return nullptr;
			}

			textures.push_back(texture);
			return texture;
		}
		else
		{
			std::string type = ToLower(type_);
			if (type == "cube")
			{
				TextureCubeMapFile* texture = new TextureCubeMapFile();
				if (!texture)
					return nullptr;
				if (!texture->Create(folder + file))
				{
					delete texture;
					return nullptr;
				}

				textures.push_back(texture);
				return texture;
			}
			else if (type == "video")
			{
				VideoTexture* texture = new VideoTexture();
				if (!texture)
					return nullptr;
				if (!texture->Create(file))
				{
					delete texture;
					return nullptr;
				}

				textures.push_back(texture);
				return texture;
			}
			else //if (type == "2d")
			{
				Texture2DFile* texture = new Texture2DFile();
				if (!texture)
					return nullptr;
				if (!texture->Create(folder + file))
				{
					delete texture;
					return nullptr;
				}

				textures.push_back(texture);
				return texture;
			}
		}
	}

	bool CreateScene(const char* folder_, const char* scenefile_)
	{
		std::string folder = folder_;

		// 1. retrieve the vertex/fragment source code from filePath
		std::string shaderToyCode;
		std::ifstream shaderToyFile;
		// ensure ifstream objects can throw exceptions:
		shaderToyFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			std::string url;
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
						return false;

					Value& passJson = passesJson[i];
					if (passJson.IsObject())
					{
						if (passJson.HasMember("rendertarget"))
						{
							passes[i].SetFrameBuffer(GetFrameBuffer(passJson["rendertarget"].GetString()));
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
									if (channelJson.HasMember("url"))
									{
										std::string url = channelJson["url"].GetString();
										std::string type = "2d";
										if(channelJson.HasMember("type"))
											type = channelJson["type"].GetString();
										
										FlipFrameBuffer* frameBuffer = GetFrameBuffer(url.c_str());
										Texture* texture = GetTexture(folder.c_str(), url.c_str(), type.c_str());
										if (texture)
											passes[i].SetChannelTexture(j, texture);
										else if (frameBuffer)
											passes[i].SetChannelTexture(j, frameBuffer);
										else
											return false;
									}
								}
							}
						}
						if (passJson.HasMember("shader"))
						{
							const char* shaderURL = passJson["shader"].GetString();
							if (!passes[i].SetShader(folder.c_str(), shaderURL, commonShaderURL))
								return false;
						}
					}
				}
			}
		}

		return true;
	}


	bool Create(const char* folder_)
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

		if (!CreateScene(folder_, "scene.json"))
			return false;

		return true;
	}

	bool Update(double time, double deltaTime, vec4 mouse, vec2 mouseDelta, int frameCounter)
	{
		for (auto& pass : passes)
		{
			if (!pass.Update(time, deltaTime, mouse, mouseDelta, frameCounter))
				return false;
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
		//return macShaderDemo.Create("default");
		//return macShaderDemo.Create("Fast Atmospheric Scattering");
		//return macShaderDemo.Create("Atmospheric scattering explained");
		//return macShaderDemo.Create("Atmospheric Scattering Fog");
		return macShaderDemo.Create("Elevated");
		//return macShaderDemo.Create("Path tracing cornellbox with MIS");
		//return macShaderDemo.Create("Bidirectional path tracing");
		//return macShaderDemo.Create("Demofox Path Tracing 1");
		//return macShaderDemo.Create("Demofox Path Tracing 2");
		//return macShaderDemo.Create("Greek Temple");
		//return macShaderDemo.Create("Hexagonal Grid Traversal - 3D");
		//return macShaderDemo.Create("Post process - SSAO");
		//return macShaderDemo.Create("Lake in highland");
		//return macShaderDemo.Create("MO");
		//return macShaderDemo.Create("PBR Material Gold");
		return macShaderDemo.Create("Path Tracer MIS");
		//return macShaderDemo.Create("testmouse");
	}

	virtual bool OnUpdate() override
	{
		return macShaderDemo.Update(GetTime(), GetDeltaTime(), GetMouse(), GetMouseDelta(), GetFrameCounter());
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