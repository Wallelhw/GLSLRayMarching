#ifndef _RenderStates_h_
#define _RenderStates_h_

#include "Platform.h"
#include "ColorRGBA.h"
#include "Vector2.h"
#include "Graphics.h"

/*
int renerStateGLEnableFlags[] =
{
	// GL_BLEND,
	// GL_CULL_FACE,
	GL_DEBUG_OUTPUT,
	GL_DEBUG_OUTPUT_SYNCHRONOUS,
	// GL_DEPTH_TEST,
	GL_DITHER,
	// GL_POLYGON_OFFSET_FILL,
	GL_PRIMITIVE_RESTART_FIXED_INDEX,
	GL_RASTERIZER_DISCARD,
	GL_SAMPLE_ALPHA_TO_COVERAGE,
	GL_SAMPLE_COVERAGE,
	GL_SAMPLE_MASK,
	// GL_SCISSOR_TEST,
	// GL_STENCIL_TEST
};

/*
Stencil
2.0
glStencilFunc
glStencilMask
glStencilOp
glStencilFuncSeparate
glStencilMaskSeparate
glStencilOpSeparate
Cull Face
2.0
glCullFace
glFrontFace
Blend State
2.0
glBlendColor
glBlendEquation
glBlendFunc
glBlendEquationSeparate
glBlendFuncSeparate
3.2
glBlendEquationi
glBlendFunci
glBlendEquationSeparatei
glBlendFuncSeparatei
glBlendBarrier
Clear State
2.0
glClear
glClearColor
glClearDepthf
glClearStencil
Color Mask
2.0
glColorMask
3.2
glColorMaski
Depth State
2.0
glDepthFunc
glDepthMask
glDepthRangef
Misc
2.0
glSampleCoverage
glLineWidth
glPolygonOffset
glScissor
glViewport
glMinSampleShading
3.1
glSampleMaski
*/

////////////////////////////////////////////////////////////////////////
class BlendState
{
public:
	enum class Equation
	{
		ADD = 0,
		SUBTRACT,
		REVERSE_SUBTRACT,
		MIN,
		MAX
	};

	enum class Func
	{
		ZERO = 0,
		ONE,

		SRC_COLOR,
		ONE_MINUS_SRC_COLOR,

		DST_COLOR,
		ONE_MINUS_DST_COLOR,

		SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA,

		DST_ALPHA,
		ONE_MINUS_DST_ALPHA,

		CONSTANT_COLOR,
		ONE_MINUS_CONSTANT_COLOR,

		CONSTANT_ALPHA,
		ONE_MINUS_CONSTANT_ALPHA
	};

	BlendState();
	void Apply() const;

	bool enabled;
	Equation colorEquation;
	Func colorSrcFactor;
	Func colorDstFactor;
	Equation alphaEquation;
	Func alphaSrcFactor;
	Func alphaDstFactor;

	ColorRGBA color;
};

////////////////////////////////////////////////////////////////////////
class ClearState
{
public:
	ClearState();
	void Apply() const;

	bool enableClearColor;
	bool enableClearDepth;
	bool enableClearStencil;

	ColorRGBA clearColor;
	float clearDepth;
	unsigned int clearStencil;
};

////////////////////////////////////////////////////////////////////////
class CullFaceState
{
public:
	enum class Mode
	{
		FRONT = 0,
		BACK,
		FRONT_AND_BACK
	};

	CullFaceState();
	void Apply() const;

	bool enabled;
	Mode mode;
};

////////////////////////////////////////////////////////////////////////
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

	DepthTestState();
	void Apply() const;

	bool depthTestEnabled;
	bool depthWriteEnabled;
	Func func;
	float near;
	float far;
};

////////////////////////////////////////////////////////////////////////
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

	PolygonModeState();
	void Apply() const;

	Face face;
	Mode mode;
};

////////////////////////////////////////////////////////////////////////
class PolygonOffsetState
{
public:
	PolygonOffsetState();
	void Apply() const;

	float factor;
	float offset;
};

////////////////////////////////////////////////////////////////////////
class ScissorTestState
{
public:
	ScissorTestState();
	void Apply() const;

	bool enabled;
	Vector2 pos;
	Vector2 size;
};

////////////////////////////////////////////////////////////////////////
class StencilState
{
public:
	enum class Func
	{
		NEVER = 0,
		LESS,
		LEQUAL,
		GREATER,
		GEQUAL,
		EQUAL,
		NOTEQUAL,
		ALWAYS
	};

	enum class Operation
	{
		KEEP = 0, 
		ZERO, 
		REPLACE, 
		INCR, 
		INCR_WRAP, 
		DECR, 
		DECR_WRAP, 
		INVERT
	};

	StencilState();
	void Apply() const;

	bool enabled;
	Func frontFaceFunc;
	unsigned int frontFaceRef;
	unsigned int frontFaceMask;
	Operation frontFaceSFail;
	Operation frontFaceSPassDFail;
	Operation frontFaceSPassDPass;

	Func backFaceFunc;
	unsigned int backFaceRef;
	unsigned int backFaceMask;
	Operation backFaceSFail;
	Operation backFaceSPassDFail;
	Operation backFaceSPassDPass;
};

//////////////////////////////////////////////////////////////
class ViewportState
{
public:
	ViewportState();
	void Apply() const;

	Vector2 pos;
	Vector2 size;
};

//////////////////////////////////////////////////////////////
class RenderStates
{
public:
	RenderStates()
	{
	}

	~RenderStates()
	{
	}

	void Apply() const
	{
		blendState.Apply();
		clearState.Apply();
		cullFaceState.Apply();
		depthTestState.Apply();
		polygonModeState.Apply();
		polygonOffsetState.Apply();
		scissorTestState.Apply();
		stencilState.Apply();
		viewportState.Apply();
	}

	BlendState blendState;
	ClearState clearState;
	CullFaceState cullFaceState;
	DepthTestState depthTestState;
	PolygonModeState polygonModeState;
	PolygonOffsetState polygonOffsetState;
	ScissorTestState scissorTestState;
	StencilState stencilState;
	ViewportState viewportState;
};

#endif