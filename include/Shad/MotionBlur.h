#ifndef __MOTION_BLUR_H_
#define __MOTION_BLUR_H_

#include <vector>

#include <Shad/TextureRender.h>
#include <Shad/Shader.h>
#include <GL/glew.h>
#include <GL/glut.h>

#define MAX_FRAMES 5

class MotionBlur {
public:
	MotionBlur(GLuint width, GLuint height, GLuint numFrames);

	void addFrame(GLuint texID);
	GLuint blurFrames();

	GLuint width();
	GLuint height();

	void clearFrames();

	void printFrames();

private:
	TextureRender *motionBlurTarget_;
	Shader *motionBlurShader_;

	GLuint numFrames_;
	std::vector<GLuint> frames_;
};

#endif