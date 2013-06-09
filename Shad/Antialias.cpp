#include <Shad/Antialias.h>
#include <iostream>

#define ANTIALIAS_SHADER "assets\\shaders\\antialias"

Antialias::Antialias(GLuint width, GLuint height) 
{
	aaTarget = new TextureRender(width, height, GL_RGBA);
	aaShader = new Shader(ANTIALIAS_SHADER);
	if (!aaShader->loaded()) {
		std::cerr << "Failed to load shader: " << aaShader->path() << std::endl;
		std::cerr << aaShader->errors() << std::endl;
	}
}

GLuint Antialias::antialiasTexture(GLuint texID, GLuint texWidth, GLuint texHeight)
{
	aaTarget->bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* use AA shader */
	glUseProgram(aaShader->programID());

	/* set orthographic projectionation */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1,1,-1,1);

	/* draw textured quad */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);

	/* bind texture data to shader */
	GLint frame = glGetUniformLocation(aaShader->programID(), "frame");
	glUniform1i(frame, 0);

	GLint rt_w = glGetUniformLocation(aaShader->programID(), "rt_w");
	glUniform1f(rt_w, (float)texWidth);

	GLint rt_h = glGetUniformLocation(aaShader->programID(), "rt_h");
	glUniform1f(rt_h, (float)texHeight);

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

	glViewport(0, 0, aaTarget->width(), aaTarget->height());

	aaTarget->unbind();

	glUseProgram(0);

	return aaTarget->textureID();
}

GLuint Antialias::width()
{
	return aaTarget->width();
}

GLuint Antialias::height()
{
	return aaTarget->height();
}