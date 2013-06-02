#include <string>

#include "Level.h"

#include <GL\glew.h>
#include <GL\glut.h>
#include <PolyMesh/bitmap_image.h>
#include <PolyMesh/PolyMesh.h>
#include <PolyMesh/Cloth.h>

using namespace std;


Level::Level(int level) {
	_level = level;
	LEVEL = "level";
	EXT = ".ext";
	filename = LEVEL +  to_string(level) + EXT;
}

	
void Level::generateBlocks(string shader, bitmap_image& space_image) {
	switch(_level) {
		case 1:
			PolyMesh *Plane = (new PolyMesh())->LoadObj("assets\\obj\\plane.obj")->GenerateRigidBody()->Scale(OpenMesh::Vec3f(10,10,10))->Translate(OpenMesh::Vec3f(0,-1,0));
			//Plane->AttachShader(shader);
			Plane->RigidBody->setRollingFriction(0.8f);
			Plane->RigidBody->setAnisotropicFriction(Plane->RigidBody->getCollisionShape()->getAnisotropicRollingFrictionDirection(),btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);

			space_image = bitmap_image("assets\\bmp\\checkerboard.bmp");
			space_image.rgb_to_bgr();
			Plane->ApplyTexture(space_image.data(), space_image.width(), space_image.height());
	
			PolyMesh *Plane2 = (new PolyMesh())->LoadObj("assets\\obj\\plane.obj")->GenerateRigidBody()->Scale(OpenMesh::Vec3f(10,10,10))->Translate(OpenMesh::Vec3f(10,-1,10));
			//	Plane2->AttachShader(shader);
			Plane2->RigidBody->setRollingFriction(0.8f);
			Plane2->RigidBody->setAnisotropicFriction(Plane2->RigidBody->getCollisionShape()->getAnisotropicRollingFrictionDirection(),btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);

			space_image = bitmap_image("assets\\bmp\\checkerboard.bmp");
			space_image.rgb_to_bgr();
			Plane2->ApplyTexture(space_image.data(), space_image.width(), space_image.height());
			break;
	};

}

