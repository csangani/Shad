#ifndef __BLUR_H__
#define __BLUR_H__

#include <Shad/TextureRender.h>
#include <Shad\Shader.h>
#include <GL/glew.h>
#include <GL/glut.h>

class Blur {
public:
	Blur(GLuint width, GLuint height);
	GLuint blurTexture(GLuint textureID);

	GLuint width();
	GLuint height();

private:
	TextureRender *horizontalTextureTarget;
	TextureRender *verticalTextureTarget;

	Shader *blurShader;

	void directionalBlur(GLuint textureID, int orientation);
};

#endif