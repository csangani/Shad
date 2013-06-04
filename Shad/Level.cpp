#include <string>

#include <Shad/Level.h>
#include <Shad/Platform.h>

#include <GL\glew.h>
#include <GL\glut.h>



using namespace std;

Level::Level(int level) {
	_level = level;
	LEVEL = "level";
	EXT = ".ext";
	//filename = LEVEL +  to_string(level) + EXT;
}

	
void Level::generateBlocks(string shader, bitmap_image& space_image) {
	switch(_level) {
		case 1:
			//string texture = "assets\\bmp\\checkerboard.bmp";
			//Box2->Texture(space_image, texture);
			
			createPlatform(1,1,10,0,-10,0);
			createPlatform(1,5,1,0,-13,-6);
			createPlatform(1,1,5,2,-10,0);
			createPlatform(1,1,5,-2,-10,0);
			createPlatform(1,5,1,0,-15,-8);
			createPlatform(2,2,2,0,-15,-11,45,0,1,1);
			createPlatform(10,2,2,0,-15,-14,35,1,0,0);
			createPlatform(8,2,2,0,-15,-16,35,1,0,0);
			createPlatform(6,2,2,0,-15,-18,35,1,0,0);
			createPlatform(4,2,2,0,-15,-20,35,1,0,0);
			createPlatform(2,2,2,0,-15,-22);
			break;
	};
}

void Level::createPlatform(float scaleX, float scaleY, float scaleZ, float translateX, float translateY, float translateZ, float rotationAngle, float rotateX, float rotateY, float rotateZ)
{
	string cube = "assets\\obj\\cube.obj";
	Platform *platform = new Platform(cube);
	platform->Scale(scaleX, scaleY, scaleZ);
	platform->Translate(translateX, translateY, translateZ);
	if (rotationAngle > 0) {
		platform->Rotate(rotationAngle, rotateX, rotateY, rotateZ);
	}
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
	
