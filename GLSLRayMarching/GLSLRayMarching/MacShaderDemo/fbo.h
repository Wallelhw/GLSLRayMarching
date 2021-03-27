#pragma once

#include "Platform.h"
#include <Vector2.h>
#include <Vector4.h>

class FBO
{
public:
    FBO(const IVector2& size);
    ~FBO();

    void bind();

    void unBind();

    float getMaxAlpha();

    void bindTexture();

    unsigned int handle, texture;
    const IVector2 size;

    int prev_viewport[4] = { 0 };
    int prev_scissor[4] = { 0 };

    // Allocated data that the framebuffer can be written to if needed
    std::vector<Vector4> data;
};