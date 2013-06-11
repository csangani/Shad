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
#include <Shad/Vertex.h>

class Platform {
	public:
		Platform(std::string model);
		~Platform();
		Platform *Scale(float scalex, float scaley, float scalez);
		Platform *Translate(float tx, float ty, float tz);
		Platform *Rotate(float angle, float x, float y, float z);
		void Platform::Texture(bitmap_image & image, std::string texture);
		std::vector<PlatformEdge *> edges;
		std::vector<Vertex *> vertices;
		PolyMesh *platformMesh;

		void setMoving(int beats, float deltaX, float deltaY, float deltaZ);
		
		void setShrinking(int _beat, float scale);

		bool isMoving();
		bool isShrinking();


		void setElevator(float xMove, float yMove, float zMove, float stopX, float stopY, float stopZ);

		void setCollapsible();
		bool isCollapsible();
		int getBeat();

		void move(uint64_t deltaPoint);
		void UpdateCounter();

		void shrink(uint64_t deltaPoint);


		bool moveWChar(uint64_t deltaPoint, float charX, float charY, float charZ);

		OpenMesh::Vec3f getDirection();
		OpenMesh::Vec3f getShrinking();
		bool checkIfGood(float limit);

		bool withInBounds(float charX, float charY, float charZ);

		void collapse(bool onGround, float charX, float charY, float charZ);
		bool carry(bool onGround, float charX, float charY, float charZ);

		float * getColor();
		void reset();

		void subdivide();
		int getCounter();

		void deform(bool onGround, float charX, float charY, float charZ);

		bool stopped();
	private:
		void GenerateVertices();
		bool resetBool;

		void GenerateEdges();
		void GenerateAllEdges();
		bool moving;
		bool shrinking;
		float deltaX, deltaY, deltaZ;
		float _scaleX, _scaleY, _scaleZ;
		float _shrinkX, _shrinkY, _shrinkZ, _shrinkScale;
		float initialX, initialY, initialZ;
		float _angle, xaxis, yaxis, zaxis;
		float runningTotalX, runningTotalY, runningTotalZ;
		float carryCheckX, carryCheckY, carryCheckZ;
		float limitX, limitY, limitZ;
		bool collapsible;
		bool collapsing;
		bool deformed;
		float color[4];
		std::string mod;
		int beats;
		int scaleBeat;
		bool elevator;
		bool finish;
		int counter;

};

#endif