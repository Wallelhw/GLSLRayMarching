#ifndef _Shape_h_
#define _Shape_h_

#include "Texture.h"
#include "RenderStates.h"
#include "ShaderProgram.h"
#include "Buffer.h"
#include "Primitives.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "AABB3.h"
#include "Frame3.h"
#include "Camera.h"

template<class UniformBlockData>
class Shape : public Frame3
{
public:
	Shape()
	{
	}

	virtual ~Shape()
	{
	}

	RenderStates& GetRenderStates()
	{
		return renderStates;
	}

	Texture2DFile& GetTexture()
	{
		return texture;
	}

	UniformBlockData& GetUniformBlockData()
	{
		return uniformBlockData;
	}

	ShaderProgram& GetShaderProgram()
	{
		return shaderProgram;
	}

	Primitives& GetPrimitives()
	{
		return primitives;
	}

	static bool AddXYRect(std::vector<Vector3>& vertices_, const Vector3& size_, const Vector3& pivot_ = Vector3::Zero)
	{
		vertices_.push_back(Vector3(-size_.X() - pivot_.X(), -size_.Y() - pivot_.Y(), +size_.Z() - pivot_.Z()));
		vertices_.push_back(Vector3(-size_.X() - pivot_.X(), +size_.Y() - pivot_.Y(), +size_.Z() - pivot_.Z()));
		vertices_.push_back(Vector3(+size_.X() - pivot_.X(), -size_.Y() - pivot_.Y(), +size_.Z() - pivot_.Z()));

		vertices_.push_back(Vector3(+size_.X() - pivot_.X(), -size_.Y() - pivot_.Y(), +size_.Z() - pivot_.Z()));
		vertices_.push_back(Vector3(-size_.X() - pivot_.X(), +size_.Y() - pivot_.Y(), +size_.Z() - pivot_.Z()));
		vertices_.push_back(Vector3(+size_.X() - pivot_.X(), +size_.Y() - pivot_.Y(), +size_.Z() - pivot_.Z()));

		return true;
	}

	static bool AddYZRect(std::vector<Vector3>& vertices_, const Vector3& size_, const Vector3& pivot_ = Vector3::Zero)
	{
		vertices_.push_back(Vector3(+size_.X() - pivot_.X(), -size_.Y() - pivot_.Y(), -size_.Z() - pivot_.Z()));
		vertices_.push_back(Vector3(+size_.X() - pivot_.X(), +size_.Y() - pivot_.Y(), -size_.Z() - pivot_.Z()));
		vertices_.push_back(Vector3(+size_.X() - pivot_.X(), -size_.Y() - pivot_.Y(), +size_.Z() - pivot_.Z()));

		vertices_.push_back(Vector3(+size_.X() - pivot_.X(), -size_.Y() - pivot_.Y(), +size_.Z() - pivot_.Z()));
		vertices_.push_back(Vector3(+size_.X() - pivot_.X(), +size_.Y() - pivot_.Y(), -size_.Z() - pivot_.Z()));
		vertices_.push_back(Vector3(+size_.X() - pivot_.X(), +size_.Y() - pivot_.Y(), +size_.Z() - pivot_.Z()));

		return true;
	}

	static bool AddZXRect(std::vector<Vector3>& vertices_, const Vector3& size_, const Vector3& pivot_ = Vector3::Zero)
	{
		vertices_.push_back(Vector3(-size_.X() - pivot_.X(), +size_.Y() - pivot_.Y(), -size_.Z() - pivot_.Z()));
		vertices_.push_back(Vector3(+size_.X() - pivot_.X(), +size_.Y() - pivot_.Y(), -size_.Z() - pivot_.Z()));
		vertices_.push_back(Vector3(-size_.X() - pivot_.X(), +size_.Y() - pivot_.Y(), +size_.Z() - pivot_.Z()));

		vertices_.push_back(Vector3(-size_.X() - pivot_.X(), +size_.Y() - pivot_.Y(), +size_.Z() - pivot_.Z()));
		vertices_.push_back(Vector3(+size_.X() - pivot_.X(), +size_.Y() - pivot_.Y(), -size_.Z() - pivot_.Z()));
		vertices_.push_back(Vector3(+size_.X() - pivot_.X(), +size_.Y() - pivot_.Y(), +size_.Z() - pivot_.Z()));

		return true;
	}

	static bool AddBox(std::vector<Vector3>& vertices_, const Vector3& size_)
	{
		return true;
	}

	static bool AddTriangle(std::vector<Vector3>& vertices_, const Vector3& v0_, const Vector3& v1_, const Vector3& v2_)
	{
		vertices_.push_back(v0_);
		vertices_.push_back(v1_);
		vertices_.push_back(v2_);

		return true;
	}
	
	#define USE_UNIFORM_BLOCK

	bool Create(std::vector<Vector3>& vertices)
	{
		////////////////////////////////////////////////////////////
		bool success = primitives
			.Begin()
			.FillVertices(0, 3, VertexAttribute::DataType::FLOAT, false, 0, 0, &vertices[0], vertices.size())
			.End();
		if (!success)
		{
			return false;
		}

		////////////////////////////////////////////////////////////
		renderStates.scissorTestState.enabled = true;
		renderStates.scissorTestState.pos = Vector2(0, 0);
		renderStates.viewportState.pos = Vector2(0, 0);

		renderStates.polygonModeState.face = PolygonModeState::Face::FRONT_AND_BACK;
		renderStates.polygonModeState.mode = PolygonModeState::Mode::LINE;

		renderStates.cullFaceState.enabled = true;
		renderStates.cullFaceState.mode = CullFaceState::Mode::BACK;

		renderStates.depthTestState.depthTestEnabled = true;
		renderStates.depthTestState.depthWriteEnabled = true;
		renderStates.depthTestState.func = DepthTestState::Func::LEQUAL;

		////////////////////////////////////////////////////////////
		if (!shaderProgram.Create("ShapeVS.glsl", "ShapePS.glsl"))
		{
			return false;
		}

		////////////////////////////////////////////////////////////
		if (!uniformBlockBuffer
			.Begin(Buffer::Type::UNIFORM_BUFFER, Buffer::Usage::STATIC_DRAW)
			.Fill(&uniformBlockData, sizeof(UniformBlockData))
			.End())
		{
			return false;
		}

		return true;
	}

	void Update(Camera& camera, const Vector2& screenSize)
	{
		//////////////////////////////////////////////////////
		// RS
		renderStates.scissorTestState.size = screenSize;
		renderStates.viewportState.size = screenSize;
		renderStates.Apply();

		///////////////////////////////////////////////////////
		// TexMap

		///////////////////////////////////////////////////////
		shaderProgram.Bind();
		shaderProgram.BindUniformBlock(uniformBlockBuffer, "UniformBlockData", 0);

#ifdef USE_UNIFORM_BLOCK
		UniformBlockData uniformBlockData;
		uniformBlockData.worldTransform = GetGlobalTransform().Transpose();
		uniformBlockData.viewTransform = camera.GetInverseGlobalTransform().Transpose();
		uniformBlockData.projTransform = camera.GetProjectionTransform().Transpose();
		uniformBlockBuffer.Update(0, &uniformBlockData, sizeof(UniformBlockData));
#else
		shaderProgram.SetUniformMatrix4x4fv("worldTransform", 1, GetGlobalTransform());
		shaderProgram.SetUniformMatrix4x4fv("viewTransform", 1, camera.GetInverseGlobalTransform());
		shaderProgram.SetUniformMatrix4x4fv("projTransform", 1, camera.GetProjectionTransform());
#endif
		primitives.Bind();
		primitives.DrawArray(Primitives::Mode::TRIANGLES, 0, primitives.GetCount());
	}

	void Destroy()
	{
		renderStates.Destroy();
		texture.Destroy();
		uniformBlockBuffer.Destroy();
		shaderStorageBlockBuffer.Destroy();
		shaderProgram.Destroy();
		primitives.Destroy();
	}
private:
	RenderStates renderStates;
	Texture2DFile texture;
	UniformBlockData uniformBlockData;
	Buffer uniformBlockBuffer;
	Buffer shaderStorageBlockBuffer;
	ShaderProgram shaderProgram;
	Primitives primitives;
};

#endif