#include <Shad/MotionBlur.h>
#include <iostream>

#define MOTION_BLUR_SHADER "assets\\shaders\\motionblur"

MotionBlur::MotionBlur(GLuint width, GLuint height)
{
	motionBlurTarget = new TextureRender(width, height, GL_RGB);
	motionBlurShader = new Shader(MOTION_BLUR_SHADER);
	if (!motionBlurShader->loaded()) {
		std::cerr << "Failed to load shader: " << motionBlurShader->path() << std::endl;
		std::cerr << motionBlurShader->errors() << std::endl;
	}
}

GLuint MotionBlur::blurFrames(GLuint numFrames, GLuint *texIDs)
{
	// Limit of 5 frames max for performance
	if (numFrames > MAX_FRAMES)
		return -1;
	motionBlurTarget->bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* use blend shader */
	glUseProgram(motionBlurShader->programID());

	/* set orthographic projectionation */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1,1,-1,1);

	/* draw textured quad */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);

	/* bind textures to shader samplers */
	for (unsigned int i = 0; i < MAX_FRAMES; i++) {
		if (numFrames > i) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, texIDs[i]);
			std::string varName = "frame";
			varName += i;
			GLint frame = glGetUniformLocation(motionBlurShader->programID(), varName.c_str());
			glUniform1i(frame, i);
		}
	}
	/*if (numFrames > 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texIDs[0]);
		GLint texture1 = glGetUniformLocation(motionBlurShader->programID(), "frame1");
		glUniform1i(texture1, 0);
	}
	if (numFrames > 1) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texIDs[1]);
		GLint texture2 = glGetUniformLocation(motionBlurShader->programID(), "frame2");
		glUniform1i(texture2, 1);
	}
	if (numFrames > 2) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texIDs[2]);
		GLint texture3 = glGetUniformLocation(motionBlurShader->programID(), "frame3");
		glUniform1i(texture3, 2);
	}
	if (numFrames > 3) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, texIDs[3]);
		GLint texture4 = glGetUniformLocation(motionBlurShader->programID(), "frame4");
		glUniform1i(texture4, 3);
	}
	if (numFrames > 4) {
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, texIDs[4]);
		GLint texture5 = glGetUniformLocation(motionBlurShader->programID(), "frame5");
		glUniform1i(texture5, 4);
	}*/
	GLint numFramesLoc = glGetUniformLocation(motionBlurShader->programID(), "numFrames");
	glUniform1i(numFramesLoc, numFrames);

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

	glViewport(0, 0, motionBlurTarget->width(), motionBlurTarget->height());

	motionBlurTarget->unbind();

	glUseProgram(0);
	glActiveTexture(GL_TEXTURE0);

	return motionBlurTarget->textureID();
}