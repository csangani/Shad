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
	Platform *Box;
	Platform *Box2;
	Platform *Box3;
	switch(_level) {
		case 1:
			string cube = "assets\\obj\\cube.obj";
			string texture = "assets\\bmp\\checkerboard.bmp";
			Box = new Platform(cube);
			Box->Scale(1, 1, 10);
			Box->Translate(0,-10,0);
			Box->Texture(space_image, texture);

			Box2 = new Platform(cube);
			Box2->Scale(1, 10, 2);
			Box2->Translate(0,-10,-2);
			Box2->Texture(space_image, texture);
	/*		
	


			Box3 = (new PolyMesh())->LoadObj("assets\\obj\\cube.obj")->GenerateRigidBody()->Scale(OpenMesh::Vec3f(2,2,2))->Translate(OpenMesh::Vec3f(0,-11,-7));
			//Plane->AttachShader(shader);
			Box3->RigidBody->setRollingFriction(0.8f);
			Box3->RigidBody->setAnisotropicFriction(Box2->RigidBody->getCollisionShape()->getAnisotropicRollingFrictionDirection(),btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);

			space_image = bitmap_image("assets\\bmp\\checkerboard.bmp");
			space_image.rgb_to_bgr();
			Box3->ApplyTexture(space_image.data(), space_image.width(), space_image.height());
			*/
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
	
