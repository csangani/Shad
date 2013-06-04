#ifndef PLATFORM_H
#define PLATFORM_H

#include <Polymesh/PolyMesh.h>
#include <GL\glew.h>
#include <GL\glut.h>
#include <string>
#include <PolyMesh/bitmap_image.h>

class Platform {
	public:
		Platform(std::string model);
		~Platform();
		void Scale(float scalex, float scaley, float scalez);
		void Translate(float tx, float ty, float tz);
		void Rotate(float angle, float x, float y, float z);
		void Platform::Texture(bitmap_image & image, std::string texture);
	private:
		PolyMesh *platformMesh;
		PolyMesh *edgesMesh;
};

#endif