#include <string>

#include <PolyMesh/bitmap_image.h>

#include <GL/glew.h>
#include <GL/glut.h>

enum SkyBoxFace {
	SkyBoxLeft = 0,
	SkyBoxRight,
	SkyBoxTop,
	SkyBoxBottom,
	SkyBoxFront,
	SkyBoxBack
};

class SkyBox {
public:
	SkyBox(float cx, float cy, float cz, float size);

	GLuint addTexture(std::string path, SkyBoxFace face);
	void draw();

	GLuint textures_[6];
private:
	GLfloat cx_, cy_, cz_, size_;
};