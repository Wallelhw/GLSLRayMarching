#include "fbo.h"
#include "Graphics.h"
#include "Vector4.h"
#include <exception>
#include <stdexcept>

FBO::FBO(const IVector2 &size) 
: size(size), data(size.X() * size.Y(), Vector4::Zero)
{
    glGenFramebuffers(1, &handle);
    glBindFramebuffer(GL_FRAMEBUFFER, handle);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.X(), size.Y(), 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        throw std::runtime_error("Framebuffer not complete.");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FBO::~FBO()
{
    glDeleteTextures(1, &texture);
    glDeleteFramebuffers(1, &handle);
}

void FBO::bind()
{
    glGetIntegerv(GL_VIEWPORT, prev_viewport);
    glViewport(0, 0, size.X(), size.Y());

    glGetIntegerv(GL_SCISSOR_BOX, prev_scissor);
    glScissor(0, 0, size.X(), size.Y());

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, handle);
}

void FBO::unBind()
{
    glViewport(prev_viewport[0], prev_viewport[1], prev_viewport[2], prev_viewport[3]);
    glScissor(prev_scissor[0], prev_scissor[1], prev_scissor[2], prev_scissor[3]);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::bindTexture()
{
    glBindTexture(GL_TEXTURE_2D, texture);
}

float FBO::getMaxAlpha()
{
    glReadPixels(0, 0, size.X(), size.Y(), GL_RGBA, GL_FLOAT, data.data());

    float max_alpha = 0.0f;
    for (int y = 0; y < size.Y(); y++)
    {
        for (int x = 0; x < size.X(); x++)
        {
            float alpha = data[y * size.Y() + x].W();
            if (alpha > max_alpha)
            {
                max_alpha = alpha;
            }
        }
    }
    return max_alpha;
}