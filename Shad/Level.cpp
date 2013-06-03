#include <string>

#include "Level.h"

#include <GL\glew.h>
#include <GL\glut.h>


using namespace std;

Level::Level(int level) {
	_level = level;
	LEVEL = "level";
	EXT = ".ext";
	filename = LEVEL +  to_string(level) + EXT;
}

	
void Level::generateBlocks(string shader, bitmap_image& space_image) {
	PolyMesh *Plane;
	switch(_level) {
		case 1:
			Plane = (new PolyMesh())->LoadObj("assets\\obj\\plane.obj")->GenerateRigidBody()->Scale(OpenMesh::Vec3f(10,10,10))->Translate(OpenMesh::Vec3f(0,-1,0));
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


	void Level::generateMesh (string shader, string object, float Diffuse[], float Ambient[], float Specular[], float Shininess[], bitmap_image &image, string texture)
		{
			PolyMesh *Mesh = (new PolyMesh())->LoadObj(object)->GenerateRigidBody()->SetMass(100.0f)->Translate(OpenMesh::Vec3f(0,-0.5f,0));
	        Mesh->AttachShader(shader);
	        Mesh->RigidBody->setRollingFriction(0.3f);
	        Mesh->RigidBody->setActivationState(DISABLE_DEACTIVATION);
	        Mesh->RigidBody->setAnisotropicFriction(Mesh->RigidBody->getCollisionShape()->getAnisotropicRollingFrictionDirection(),btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);
	
			Mesh->MaterialDiffuse = Diffuse;
			Mesh->MaterialAmbient = Ambient;
			Mesh->MaterialShininess = Specular;
			Mesh->MaterialShininess = Shininess;

			// Apply Texture to Mesh
			image = bitmap_image(texture);
			image.rgb_to_bgr();
			Mesh->ApplyTexture(image.data(), image.width(), image.height());
	        
			Mesh->EnableLighting();
	
	}
	
