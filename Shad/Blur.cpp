#include <Shad\Blur.h>
#include <iostream>

#define BLUR_SHADER "assets\\shaders\\blur"
#define BLUR_AMOUNT 10
#define BLUR_SCALE 1.f
#define BLUR_STRENGTH 0.2f

Blur::Blur(GLuint width, GLuint height)
{
	horizontalTextureTarget = new TextureRender(width, height, GL_RGBA);
	verticalTextureTarget = new TextureRender(width, height, GL_RGBA);
	blurShader = new Shader(BLUR_SHADER);
	if (!blurShader->loaded()) {
		std::cerr << "Failed to load shader: " << blurShader->path() << std::endl;
		std::cerr << blurShader->errors() << std::endl;
	}
}

GLuint Blur::blurTexture(GLuint textureID)
{
	directionalBlur(textureID, 0);
	directionalBlur(horizontalTextureTarget->textureID(), 1);
	return verticalTextureTarget->textureID();
}

void Blur::directionalBlur(GLuint textureID, int orientation)
{
	TextureRender *blurTarget = (orientation == 0) ? horizontalTextureTarget : verticalTextureTarget;
	blurTarget->bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* use blur shader */
	glUseProgram(blurShader->programID());

	/* set orthographic projectionation */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1,1,-1,1);

	/* draw textured quad */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//// bind texture to shader sampler
	GLint texture = glGetUniformLocation(blurShader->programID(), "texture");
	glUniform1i(texture, 0);

	GLint texelSize = glGetUniformLocation(blurShader->programID(), "TexelSize");
	glUniform2f(texelSize, 1.f/(float)blurTarget->width(), 1.f/(float)blurTarget->height());

	GLint blurAmount = glGetUniformLocation(blurShader->programID(), "BlurAmount");
	glUniform1i(blurAmount, BLUR_AMOUNT);
	GLint blurScale = glGetUniformLocation(blurShader->programID(), "BlurScale");
	glUniform1f(blurScale, BLUR_SCALE);
	GLint blurStrength = glGetUniformLocation(blurShader->programID(), "BlurStrength");
	glUniform1f(blurStrength, BLUR_STRENGTH);
	GLint orientationLocation = glGetUniformLocation(blurShader->programID(), "Orientation");
	glUniform1i(orientationLocation, orientation);

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

	glViewport(0, 0, blurTarget->width(), blurTarget->height());

	blurTarget->unbind();

	glUseProgram(0);
}

GLuint Blur::width()
{
	return verticalTextureTarget->width();
}

GLuint Blur::height()
{
	return verticalTextureTarget->height();
}