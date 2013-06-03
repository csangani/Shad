#include <Polymesh/PolyMesh.h>
#include <GL\glew.h>
#include <GL\glut.h>
#include <string>
#include <PolyMesh/bitmap_image.h>

using namespace std;

class Platform {
	public:
		Platform(string model);
		~Platform();
		void Scale(float scalex, float scaley, float scalez);
		void Translate(float tx, float ty, float tz);
		void Rotate(float angle, float x, float y, float z);
		void Platform::Texture(bitmap_image & image, string texture);
	private:
		PolyMesh * platform;

};