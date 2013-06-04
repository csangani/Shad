#ifndef __MOTION_BLUR_H_
#define __MOTION_BLUR_H_

#include <Shad/TextureRender.h>
#include <Shad\Shader.h>
#include <GL/glew.h>
#include <GL/glut.h>

#define MAX_FRAMES 5

class MotionBlur {
public:
	MotionBlur(GLuint width, GLuint height);
	GLuint blurFrames(GLuint numFrames, GLuint *texIDs);

	void addFrame(GLuint texID);

private:
	TextureRender *motionBlurTarget;
	Shader *motionBlurShader;

	// store vector of frames? (texrendertargets?)
	// push back frames until we have 5, then pop like a queue
};

#endif