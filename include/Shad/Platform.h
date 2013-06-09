#ifndef PLATFORM_H
#define PLATFORM_H

#include <Polymesh/PolyMesh.h>
#include <Shad/PlatformEdge.h>
#include <Polymesh/Character.h>
#include <GL\glew.h>
#include <GL\glut.h>
#include <string>
#include <PolyMesh/bitmap_image.h>
#include <Spline/stdint.h>

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
		
		void setShrinking(bool state, float deltaX, float deltaY, float deltaZ);
		
		bool isMoving();
		bool isShrinking();

		void setCollapsible(float startX, float startY, float startZ);
		bool isCollapsible();


		void move(uint64_t deltaPoint);
		
		void shrink(uint64_t deltaPoint);


		bool moveWChar(uint64_t deltaPoint, float charX, float charY, float charZ);

		OpenMesh::Vec3f getDirection();
		OpenMesh::Vec3f getShrinking();

		bool withInBounds(float charX, float charY, float charZ);

		void collapse(bool onGround, float charX, float charY, float charZ);
		float * getColor();
		void reset();

	private:
		void GenerateEdges();
		bool moving;
		bool shrinking;
		float deltaX, deltaY, deltaZ;
		float scaleX, scaleY, scaleZ;
		float initialX, initialY, initialZ;
		bool collapsible;
		float color[4];
};

#endif