#ifndef __BLENDER_H__
#define __BLENDER_H__

#include <Shad/TextureRender.h>
#include <Shad\Shader.h>
#include <GL/glew.h>
#include <GL/glut.h>

enum BlendMode {
	ADDITIVE,
	SCREEN,
	SOFT_LIGHTING
};

class Blender {
public:
	Blender(BlendMode mode, GLuint width, GLuint height);
	GLuint blendTextures(GLuint texID1, GLuint texID2);

	GLuint width();
	GLuint height();

private:
	BlendMode mode_;
	TextureRender *blendTarget;
	Shader *blendShader;
};

#endif