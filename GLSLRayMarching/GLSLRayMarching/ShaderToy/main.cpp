#include "FrameWork.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "VertexArrayObject.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;
#include <iostream>

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

	bool Update()
	{
		return Texture2D::Update(256, 1, &buffer[0]);
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

	bool Update()
	{
		return Texture2D::Update(1280, 720, &buffer[0]);
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

	bool Update()
	{
		return Texture2D::Update(512, 2, &buffer[0]);
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

	bool Update()
	{
		return Texture2D::Update(512, 2, &buffer[0]);
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

	bool Update()
	{
		return Texture2D::Update(1280, 720, &buffer[0]);
	}
private:
private:
	std::vector<char> buffer;
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
			, filter(Mipmap)
			, wrap(Repeat)
			, vFlip(true)
		{
		}

		void Clear()
		{
			texture = nullptr;
			filter = Mipmap;
			wrap = Repeat;
			vFlip = true;
		}

		Texture* texture;
		Filter filter;
		Wrap wrap;
		bool vFlip;
	};

	Pass()
		: enabled(true)
		, vertexArrayObject()
		, shaderProgram()
		, renderTarget(nullptr)
		, iChannels(4)
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

		renderTarget = nullptr;

		return true;
	}

	bool Update(double time, double deltaTime, vec2 mouse, vec2 mouseDelta, bool mouseButtonClick, int frameCounter)
	{
		vec3 resolution = vec3(SCR_WIDTH, SCR_HEIGHT, 0.0);
		if (renderTarget)
		{
			renderTarget->GetResolution(resolution);
			renderTarget->BindFrameBuffer();
		}
		
		glViewport(0, 0, resolution[0], resolution[1]);
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 0, resolution[0], resolution[1]);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearDepth(1.0f);
		glClearStencil(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		shaderProgram.Bind();
		shaderProgram.SetUniform3f("iResolution", resolution[0], resolution[1], resolution[2]);
		shaderProgram.SetUniform1f("iTime", time);
		shaderProgram.SetUniform1f("iTimeDelta", deltaTime);
		shaderProgram.SetUniform1i("iFrame", frameCounter);
		shaderProgram.SetUniform4f("iMouse", mouse.X(), mouse.Y(), 0.0, 0.0);
		shaderProgram.SetUniform4f("iDate", 0.0, 0.0, 0.0, 0.0);
		shaderProgram.SetUniform1f("iSampleRate", 48000.0);

		vec3 channelResolutions[4];
		float channelTimes[4];
		for (int i = 0; i < iChannels.size(); i++)
		{
			if (iChannels[i].texture)
			{
				iChannels[i].texture->GetResolution(channelResolutions[i]);
				channelTimes[i] = 0.0;

				if (iChannels[i].wrap == Pass::Wrap::Repeat)
				{
					iChannels[i].texture->SetWarpS(GL_REPEAT);
					iChannels[i].texture->SetWarpR(GL_REPEAT);
					iChannels[i].texture->SetWarpT(GL_REPEAT);
				}
				else if (iChannels[i].wrap == Pass::Wrap::Clamp)
				{
					iChannels[i].texture->SetWarpS(GL_CLAMP_TO_EDGE);
					iChannels[i].texture->SetWarpR(GL_CLAMP_TO_EDGE);
					iChannels[i].texture->SetWarpT(GL_CLAMP_TO_EDGE);
				}

				if (iChannels[i].filter == Pass::Filter::Nearest)
				{
					iChannels[i].texture->SetMinFilter(GL_NEAREST);
					iChannels[i].texture->SetMagFilter(GL_NEAREST);
				}
				else if (iChannels[i].filter == Pass::Filter::Linear)
				{
					iChannels[i].texture->SetMinFilter(GL_LINEAR);
					iChannels[i].texture->SetMagFilter(GL_LINEAR);
				}
				else if (iChannels[i].filter == Pass::Filter::Mipmap)
				{
					iChannels[i].texture->SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
					iChannels[i].texture->SetMagFilter(GL_LINEAR);
				}

				iChannels[i].texture->Bind(i);
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

		if (renderTarget)
			renderTarget->UnBindFrameBuffer();

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

	void SetTexture(int i, Texture* texture_)
	{
		this->iChannels[i].texture = texture_;
	}

	const Pass::Channel& GetChannel(int i) const
	{
		return this->iChannels[i];
	}

	void SetRenderTarget(RenderTarget* renderTarget_)
	{
		renderTarget = renderTarget_;
	}

	RenderTarget* GetRenderTarget() const
	{
		return renderTarget;
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
			"fragCoord = (vec2(aPos.x, aPos.y) + 1.0) / 2.0 * (iResolution.xy - 1.0) + 0.5;\n"
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
				else if(iChannels[i].texture->GetType() == GL_TEXTURE_CUBE_MAP)
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

		std::string fShader = fShaderHeader + fShaderChannels + commonShaderCode + fShaderCode + fShaderMain;
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
	RenderTarget* renderTarget;
};

class ShaderToyDemo
{
public:
	ShaderToyDemo()
	{
	}

	virtual ~ShaderToyDemo()
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

	RenderTarget* GetRenderTarget(const char* str)
	{
		std::string s = ToLower(str);

		if (s == "sound")
		{
			return &sound;
		}
		else if (s == "buffera")
		{
			return &bufferA;
		}
		else if (s == "bufferb")
		{
			return &bufferB;
		}
		else if (s == "bufferc")
		{
			return &bufferC;
		}
		else if (s == "bufferd")
		{
			return &bufferD;
		}
		else if (s == "cubemapa")
		{
			return &cubeMapA;
		}
		else
			return nullptr;
	}

	Texture* GetTexture(const char* type_, const char* folder_, const char* file_)
	{
		std::string folder;
		folder = folder_;
		folder += "/";

		std::string file = ToLower(file_);

		if (file == "buffera")
		{
			return &bufferA;
		}
		else if (file == "bufferb")
		{
			return &bufferB;
		}
		else if (file == "bufferc")
		{
			return &bufferC;
		}
		else if (file == "bufferd")
		{
			return &bufferD;
		}
		else if (file == "cubemapa")
		{
			return &cubeMapA;
		}
		else if (file == "keyboard")
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
				TextureCubeMap* texture = new TextureCubeMap();
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
				Texture2D* texture = new Texture2D();
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
							passes[i].SetRenderTarget(GetRenderTarget(passJson["rendertarget"].GetString()));
						}

						for (int j = 0; j < 4; j++)
						{
							passes[i].SetTexture(j, &black);

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
									if (channelJson.HasMember("url") && channelJson.HasMember("type"))
									{
										std::string url = channelJson["url"].GetString();
										std::string type = channelJson["type"].GetString();
										passes[i].SetTexture(j, GetTexture(type.c_str(), folder.c_str(), url.c_str()));
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
		if (!black.Create(32, 32, 4, false, nullptr))
			return false;
		if (!sound.Create(512, 2, 1, false))
			return false;
		if (!bufferA.Create(1024, 1024, 4, false))
			return false;
		if (!bufferB.Create(1024, 1024, 4, false))
			return false;
		if (!bufferC.Create(1024, 1024, 4, false))
			return false;
		if (!bufferD.Create(1024, 1024, 4, false))
			return false;
		//if (!cubeMapA.Create(1024, 4, false))
			//return false;


		if (!CreateScene(folder_, "scene.json"))
			return false;

		return true;
	}

	bool Update(double time, double deltaTime, vec2 mouse, vec2 mouseDelta, bool mouseButtonClick, int frame)
	{
		for (auto& pass : passes)
		{
			if (!pass.Update(time, deltaTime, mouse, mouseDelta, mouseButtonClick, frame))
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

		black.Destroy();
		sound.Destroy();
		bufferA.Destroy();
		bufferB.Destroy();
		bufferC.Destroy();
		bufferD.Destroy();
		cubeMapA.Destroy();
	}
protected:
private:
	std::vector<Texture*> textures;
	Texture2D black;
	RenderTarget2D sound;
	RenderTarget2D bufferA;
	RenderTarget2D bufferB;
	RenderTarget2D bufferC;
	RenderTarget2D bufferD;
	RenderTargetCubeMap cubeMapA;

	std::vector<Pass> passes;
};

class ShaderToy : public FrameWork
{
public:
	ShaderToy()
		: FrameWork("ShaderToy")
		, shaderToyDemo()
	{
	}

	virtual ~ShaderToy()
	{
	}

	virtual bool OnCreate() override
	{
		//return shaderToyDemo.Create("testing");
		//return shaderToyDemo.Create("default");
		//return shaderToyDemo.Create("Atmospheric scattering explained");
		return shaderToyDemo.Create("Atmospheric Scattering Fog");
		//return shaderToyDemo.Create("Elevated");
		//return shaderToyDemo.Create("Fast Atmospheric Scattering");
		//return shaderToyDemo.Create("Path tracing cornellbox with MIS");
		//return shaderToyDemo.Create("Bidirectional path tracing");
		//return shaderToyDemo.Create("Demofox Path Tracing 1");
		//return shaderToyDemo.Create("Demofox Path Tracing 2");
		//return shaderToyDemo.Create("Greek Temple");
		//return shaderToyDemo.Create("Hexagonal Grid Traversal - 3D");
		//return shaderToyDemo.Create("Post process - SSAO");
	}

	virtual bool OnUpdate() override
	{
		return shaderToyDemo.Update(GetTime(), GetDeltaTime(), GetMouse(), GetMouseDelta(), GetMouseButtonClicked(), GetFrameCounter());
	}

	void OnDestroy() override
	{
		return shaderToyDemo.Destroy();
	}
private:
	ShaderToyDemo shaderToyDemo;
};

int main()
{
	ShaderToy chapter;

	if (!chapter.Create(SCR_WIDTH, SCR_HEIGHT))
		return -1;

	chapter.Start();

	chapter.Destroy();

	return 0;
}