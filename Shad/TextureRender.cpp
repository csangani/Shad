#include <Shad/TextureRender.h>
#include <iostream>

TextureRender::TextureRender(unsigned int width, unsigned int height, GLenum renderType) {
    width_ = width;
    height_ = height;

    // Initialize the texture, including filtering options
    glGenTextures(1, &textureID_);
    glBindTexture(GL_TEXTURE_2D, textureID_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D,  0, renderType, width_, height_, 0, renderType, GL_UNSIGNED_BYTE, 0);

    // Generate a framebuffer
    glGenFramebuffers(1, &frameBufferID_);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID_);

	glGenRenderbuffers(1, &depthBufferID_);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID_);

    // Attach the texture to the frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID_, 0);

	GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, drawBuffers);

    // Check the status of the FBO
    glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Invalid framebuffer configuration." << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

TextureRender::~TextureRender() {
    glDeleteFramebuffers(1, &frameBufferID_);
    glDeleteRenderbuffers(1, &depthBufferID_);
    glDeleteTextures(1, &textureID_);
}

GLuint TextureRender::textureID() const {
    return textureID_;
}

void TextureRender::bind() {
    glPushAttrib(GL_VIEWPORT_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID_);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID_);
    glDrawBuffer(GL_NONE);
    glViewport(0, 0, width_, height_);
}

void TextureRender::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glDrawBuffer(GL_BACK);
    glPopAttrib();
}
