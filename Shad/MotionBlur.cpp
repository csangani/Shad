#include <Shad/MotionBlur.h>
#include <iostream>

#define MOTION_BLUR_SHADER "assets\\shaders\\motionblur"

MotionBlur::MotionBlur(GLuint width, GLuint height, GLuint numFrames)
{
	// initialize frames_, limit number of frames
	numFrames_ = (numFrames > MAX_FRAMES) ? MAX_FRAMES : numFrames;
	frames_.clear();
	frames_.reserve(numFrames_ + 1);

	// set up render-to-texture target and shader
	motionBlurTarget_ = new TextureRender(width, height, GL_RGBA);
	motionBlurShader_ = new Shader(MOTION_BLUR_SHADER);
	if (!motionBlurShader_->loaded()) {
		std::cerr << "Failed to load shader: " << motionBlurShader_->path() << std::endl;
		std::cerr << motionBlurShader_->errors() << std::endl;
	}
}

void MotionBlur::addFrame(GLuint texID)
{
	frames_.push_back(texID);
	if (frames_.size() > numFrames_)
		frames_.erase(frames_.begin());
}

GLuint MotionBlur::blurFrames()
{
	motionBlurTarget_->bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* use blend shader */
	glUseProgram(motionBlurShader_->programID());

	/* set orthographic projectionation */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1,1,-1,1);

	/* draw textured quad */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);

	/* bind textures to shader samplers */
	for (unsigned int i = 0; i < frames_.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, frames_[i]);
		std::string varName = "frame";
		varName += i;
		GLint frame = glGetUniformLocation(motionBlurShader_->programID(), varName.c_str());
		glUniform1i(frame, i);
	}

	GLint numFramesLoc = glGetUniformLocation(motionBlurShader_->programID(), "numFrames");
	glUniform1i(numFramesLoc, frames_.size());

	/* draw full screen quad with textures */
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(-1.f, -1.f, 0.f);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, -1.f, 0.f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1.f, 1.f, 0.f);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(-1.f, 1.f, 0.f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glViewport(0, 0, motionBlurTarget_->width(), motionBlurTarget_->height());

	motionBlurTarget_->unbind();

	glUseProgram(0);
	glActiveTexture(GL_TEXTURE0);

	return motionBlurTarget_->textureID();
}

GLuint MotionBlur::width()
{
	return motionBlurTarget_->width();
}

GLuint MotionBlur::height()
{
	return motionBlurTarget_->height();
}

void MotionBlur::clearFrames()
{
	frames_.clear();
}

void MotionBlur::printFrames()
{
	std::cout << "frames_ contents:" << std::endl;
	for (unsigned int i = 0; i < frames_.size(); i++)
		std::cout << "\tindex " << i << " - " << frames_[i] << std::endl;
}