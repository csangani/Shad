#ifndef PLATFORM_H
#define PLATFORM_H

#include <Polymesh/PolyMesh.h>
#include <Shad/PlatformEdge.h>
#include <GL\glew.h>
#include <GL\glut.h>
#include <string>
#include <PolyMesh/bitmap_image.h>

class Platform {
	public:
		Platform(std::string model);
		~Platform();
		Platform *Scale(float scalex, float scaley, float scalez);
		Platform *Translate(float tx, float ty, float tz);
		Platform *Rotate(float angle, float x, float y, float z);
		void Platform::Texture(bitmap_image & image, std::string texture);
		std::vector<PlatformEdge *> edges;
		PolyMesh *platformMesh;

		void setMoving(bool state, float deltaX, float deltaY, float deltaZ);
		bool isMoving();

		void move(int deltaPoint);

		void moveWChar(int deltaPoint);

	private:
		void GenerateEdges();
		bool moving;
		float deltaX, deltaY, deltaZ;
};

#endif