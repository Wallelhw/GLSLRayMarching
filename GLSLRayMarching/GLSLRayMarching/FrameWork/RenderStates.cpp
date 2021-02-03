#include "Platform.h"
#include "RenderStates.h"
#include "Graphics.h"

////////////////////////////////////////////////////////////////////////
static int blendStateGLEquations[] =
{
	GL_FUNC_ADD,
	GL_FUNC_SUBTRACT,
	GL_FUNC_REVERSE_SUBTRACT,
	GL_MIN,
	GL_MAX
};

static int blendStateGLFuncs[] =
{
	GL_ZERO,
	GL_ONE,

	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,

	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,

	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,

	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA,

	GL_CONSTANT_COLOR,
	GL_ONE_MINUS_CONSTANT_COLOR,

	GL_CONSTANT_ALPHA,
	GL_ONE_MINUS_CONSTANT_ALPHA
};

BlendState::BlendState()
	: enabled(false)
	, colorEquation(Equation::ADD)
	, colorSrcFactor(Func::ONE)
	, colorDstFactor(Func::ZERO)
	, alphaEquation(Equation::ADD)
	, alphaSrcFactor(Func::ONE)
	, alphaDstFactor(Func::ZERO)
	, color(0.0f, 0.0f, 0.0f, 0.0f)
{
}

void BlendState::Apply() const
{
	if (enabled)
	{
		glEnable(GL_BLEND);
	}
	else
	{
		glDisable(GL_BLEND);
	}

	glBlendColor(color[0], color[1], color[2], color[3]);
	glBlendEquationSeparate
	(
		blendStateGLEquations[(int)colorEquation],
		blendStateGLEquations[(int)alphaEquation]
	);

	glBlendFuncSeparate
	(
		blendStateGLFuncs[(int)colorSrcFactor], blendStateGLFuncs[(int)colorDstFactor],
		blendStateGLFuncs[(int)alphaSrcFactor], blendStateGLFuncs[(int)alphaDstFactor]
	);
}

//////////////////////////////////////////////////////////////
ClearState::ClearState()
	: enableClearColor(true)
	, enableClearDepth(true)
	, enableClearStencil(true)
	, clearColor(0, 0, 0, 1)
	, clearDepth(1.0f)
	, clearStencil(0)
{
}

void ClearState::Apply() const
{
	unsigned int clearBit = 0;
	clearBit |= enableClearColor ? GL_COLOR_BUFFER_BIT : 0;
	clearBit |= enableClearDepth ? GL_DEPTH_BUFFER_BIT : 0;
	clearBit |= enableClearStencil ? GL_STENCIL_BUFFER_BIT : 0;

	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	glClearDepth(clearDepth);
	glClearStencil(clearStencil);

	if (clearBit)
		glClear(clearBit);
}

//////////////////////////////////////////////////////////////
static int cullFaceStateGLModes[] =
{
	GL_FRONT,
	GL_BACK,
	GL_FRONT_AND_BACK
};

CullFaceState::CullFaceState()
: enabled(true)
, mode(Mode::BACK)
{
}

void CullFaceState::Apply() const
{
	if (enabled)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

	glFrontFace(GL_CCW);
	glCullFace(cullFaceStateGLModes[(int)mode]);
}

//////////////////////////////////////////////////////////////
int depthTestStateGLFuncs[] =
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

DepthTestState::DepthTestState()
	: depthTestEnabled(false)
	, depthWriteEnabled(true)
	, func(DepthTestState::Func::LESS)
	, near(0.0f)
	, far(1.0f)
{
}

void DepthTestState::Apply() const
{
	if (depthTestEnabled)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	glDepthFunc(depthTestStateGLFuncs[(int)func]);

	glDepthMask(depthWriteEnabled);
	glDepthRangef(near, far);
}

//////////////////////////////////////////////////////////////
int polygonModeGLFaces[] =
{
	GL_FRONT,
	GL_BACK,
	GL_FRONT_AND_BACK
};

int polygonModeGLRenderModes[] =
{
	GL_POINT,
	GL_LINE,
	GL_FILL
};

PolygonModeState::PolygonModeState()
	: face(Face::FRONT_AND_BACK)
	, mode(Mode::FILL)
{
}

void PolygonModeState::Apply() const
{
	glPolygonMode(polygonModeGLFaces[(int)face], polygonModeGLRenderModes[(int)mode]);
}

//////////////////////////////////////////////////////////////
PolygonOffsetState::PolygonOffsetState()
	: factor(1.0)
	, offset(0.0)
{
}

void PolygonOffsetState::Apply() const
{
	glPolygonOffset(factor, offset);
}

//////////////////////////////////////////////////////////////
ScissorTestState::ScissorTestState()
	: enabled(false)
	, pos(0.0f, 0.0f)
	, size(800.0f, 400.0f)
{
}

void ScissorTestState::Apply() const
{
	if (enabled)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);

	glScissor(pos[0], pos[1], size[0], size[1]);
}

////////////////////////////////////////////////////////////////////////
static int stencilStateGLFuncs[] =
{
	GL_NEVER,
	GL_LESS,
	GL_LEQUAL,
	GL_GREATER,
	GL_GEQUAL,
	GL_EQUAL,
	GL_NOTEQUAL,
	GL_ALWAYS
};

static int stencilStateGLOperations[] =
{
	GL_KEEP,
	GL_ZERO,
	GL_REPLACE,
	GL_INCR,
	GL_INCR_WRAP,
	GL_DECR,
	GL_DECR_WRAP,
	GL_INVERT
};

StencilState::StencilState()
	: enabled(false)
	, frontFaceFunc(Func::ALWAYS)
	, frontFaceRef(0)
	, frontFaceMask(255)
	, frontFaceSFail(Operation::KEEP)
	, frontFaceSPassDFail(Operation::KEEP)
	, frontFaceSPassDPass(Operation::KEEP)

	, backFaceFunc(Func::ALWAYS)
	, backFaceRef(0)
	, backFaceMask(255)
	, backFaceSFail(Operation::KEEP)
	, backFaceSPassDFail(Operation::KEEP)
	, backFaceSPassDPass(Operation::KEEP)
{
}

void StencilState::Apply() const
{
	if (enabled)
	{
		glEnable(GL_BLEND);
	}
	else
	{
		glDisable(GL_BLEND);
	}

	glStencilFuncSeparate(GL_FRONT, stencilStateGLFuncs[(int)frontFaceFunc], frontFaceRef, frontFaceMask);
	glStencilOpSeparate(GL_FRONT,
		stencilStateGLOperations[(int)frontFaceSFail],
		stencilStateGLOperations[(int)frontFaceSPassDFail],
		stencilStateGLOperations[(int)frontFaceSPassDPass]);

	glStencilFuncSeparate(GL_BACK, stencilStateGLFuncs[(int)backFaceFunc], backFaceRef, backFaceMask);
	glStencilOpSeparate(GL_BACK,
		stencilStateGLOperations[(int)backFaceSFail],
		stencilStateGLOperations[(int)backFaceSPassDFail],
		stencilStateGLOperations[(int)backFaceSPassDPass]);
}

//////////////////////////////////////////////////////////////
ViewportState::ViewportState()
	: pos(0.0f, 0.0f)
	, size(800.0f, 400.0f)
{
}

void ViewportState::Apply() const
{
	glViewport(pos[0], pos[1], size[0], size[1]);
}
