#ifndef __ANTIALIAS_H_
#define __ANTIALIAS_H__

#include <Shad/TextureRender.h>
#include <Shad/Shader.h>
#include <GL/glew.h>
#include <GL/glut.h>

class Antialias {
public:
	Antialias(GLuint width, GLuint height);
	GLuint antialiasTexture(GLuint texID, GLuint texWidth, GLuint texHeight);

	GLuint width();
	GLuint height();

private:
	TextureRender *aaTarget;
	Shader *aaShader;
};

#endif