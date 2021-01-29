#ifndef _RenderStates_h_
#define _RenderStates_h_

#include "Platform.h"
#include "ColorRGBA.h"
#include "Vector2.h"
#include "Graphics.h"

class ClearState
{
public:
	ClearState()
		: enableClearColor(true)
		, enableClearDepth(true)
		, enableClearStencil(true)
		, clearColor(0, 0, 0, 1)
		, clearDepth(1.0f)
		, clearStencil(0)
	{
	}

	void Apply()
	{
		unsigned int clearBit = 0;
		clearBit |= enableClearColor ? GL_COLOR_BUFFER_BIT : 0;
		clearBit |= enableClearDepth ? GL_DEPTH_BUFFER_BIT : 0;
		clearBit |= enableClearStencil ? GL_STENCIL_BUFFER_BIT : 0;

		glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
		glClearDepth(clearDepth);
		glClearStencil(clearStencil);
		glClear(clearBit);
	}

	bool enableClearColor;
	bool enableClearDepth;
	bool enableClearStencil;

	ColorRGBA clearColor;
	float clearDepth;
	unsigned int clearStencil;
};

int depthTestState_Func_GL[] =
{
	GL_NEVER,
	GL_LESS,
	GL_EQUAL,
	GL_LEQUAL,
	GL_GREATER,
	GL_NOTEQUAL,
	GL_GEQUAL,
	GL_ALWAYS
};

class DepthTestState
{
public:
	enum class Func
	{
		NEVER = 0,
		LESS,
		EQUAL,
		LEQUAL,
		GREATER,
		NOTEQUAL,
		GEQUAL,
		ALWAYS
	};

	DepthTestState()
	: enabled(false)
	, func(DepthTestState::Func::LEQUAL)
	{
	}

	void Apply()
	{
		if(enabled)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
		
		glDepthFunc(depthTestState_Func_GL[(int)func]);
	}

	bool enabled;
	Func func;
};

class ScissorTestState
{
public:
	ScissorTestState()
	: enabled(false)
	, pos(0.0f, 0.0f)
	, size(800.0f, 400.0f)
	{
	}

	void Apply()
	{
		if (enabled)
			glEnable(GL_SCISSOR_TEST);
		else
			glDisable(GL_SCISSOR_TEST);

		glScissor(pos[0], pos[1], size[0], size[1]);
	}

	bool enabled;
	Vector2 pos;
	Vector2 size;
};

class ViewportState
{
public:
	ViewportState()
		: pos(0.0f, 0.0f)
		, size(800.0f, 400.0f)
	{
	}

	void Apply()
	{
		glViewport(pos[0], pos[1], size[0], size[1]);
	}

	Vector2 pos;
	Vector2 size;
};

int polygonMode_Face_GL[] =
{
	GL_FRONT,
	GL_BACK,
	GL_FRONT_AND_BACK
};

int polygonMode_Mode_GL[] =
{
	GL_POINT,
	GL_LINE,
	GL_FILL
};

class PolygonModeState
{
public:
	enum class Face
	{
		FRONT = 0,
		BACK,
		FRONT_AND_BACK
	};

	enum class Mode	
	{
		POINT = 0,
		LINE,
		FILL
	};

	PolygonModeState()
	: face(Face::FRONT)
	, mode(Mode::FILL)
	{
	}

	void Apply()
	{
		glPolygonMode(polygonMode_Face_GL[(int)face], polygonMode_Mode_GL[(int)mode]);
	}

	Face face;
	Mode mode;
};

class RenderStates
{
public:
	RenderStates()
	{
	}

	~RenderStates()
	{
	}

	void SetClearState()
	{
	}

	void Apply()
	{
		scissorTestState.Apply();
		viewportState.Apply();

		clearState.Apply();
		depthTestState.Apply();
		polygonModeState.Apply();
	}

	ScissorTestState scissorTestState;
	ViewportState viewportState;
	
	ClearState clearState;
	DepthTestState depthTestState;
	PolygonModeState polygonModeState;
};

#endif