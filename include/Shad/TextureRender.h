#ifndef TEXTURE_RENDER_H
#define TEXTURE_RENDER_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <string>

class TextureRender {
public:
	static void renderToScreen(GLuint texID, GLuint width, GLuint height, bool flipHorizontal, bool flipVertical);
	static void copyTexture(GLuint fromTexID, GLuint toTexID, GLuint width, GLuint height);

    /**
     * Creates a new render buffer for render-to-texture.  When this
     * buffer is bound using the bind() method, all OpenGL rendering is
     * drawn to the texture.  The texture's ID can be obtained by calling
     * the texture() function, then passed to a shader.
     */
    TextureRender(unsigned int width, unsigned int height, GLenum renderType);

    ~TextureRender();

    /**
     * Binds the frame buffer for rendering.
     */
    void bind();

    /**
     * Restores the original OpenGL framebuffer for normal rendering.
     */
    void unbind();

	void writeToFile(std::string filename);

    /**
     * Returns the texture that was used for render-to-texture.
     */
    GLuint textureID() const;

	GLuint width() const;

	GLuint height() const;

private:
    GLuint textureID_;
    GLuint frameBufferID_;
    GLuint depthBufferID_;
    GLuint width_;
    GLuint height_;

};

#endif

