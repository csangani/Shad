#include <Shad/Blender.h>
#include <iostream>

#define BLEND_SHADER "assets\\shaders\\blend"

Blender::Blender(BlendMode mode, GLuint width, GLuint height) 
{
	mode_ = mode;
	blendTarget = new TextureRender(width, height, GL_RGBA);
	blendShader = new Shader(BLEND_SHADER);
	if (!blendShader->loaded()) {
		std::cerr << "Failed to load shader: " << blendShader->path() << std::endl;
		std::cerr << blendShader->errors() << std::endl;
	}
}

GLuint Blender::blendTextures(GLuint texID1, GLuint texID2)
{
	blendTarget->bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* use blend shader */
	glUseProgram(blendShader->programID());

	/* set orthographic projectionation */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1,1,-1,1);

	/* draw textured quad */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);

	/* setting active textures */
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texID2);
	glActiveTexture(GL_TEXTURE0);

	GLint texture1 = glGetUniformLocation(blendShader->programID(), "texture1");
	glUniform1i(texture1, 0);

	GLint texture2 = glGetUniformLocation(blendShader->programID(), "texture2");
	glUniform1i(texture2, 1);

	GLint blendMode = glGetUniformLocation(blendShader->programID(), "BlendMode");
	glUniform1i(blendMode, mode_);

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

	glViewport(0, 0, blendTarget->width(), blendTarget->height());

	blendTarget->unbind();

	glUseProgram(0);
	glActiveTexture(GL_TEXTURE0);

	return blendTarget->textureID();
}

GLuint Blender::width()
{
	return blendTarget->width();
}

GLuint Blender::height()
{
	return blendTarget->height();
}