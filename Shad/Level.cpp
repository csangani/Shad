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
	Platform *Box;
	Platform *Box2;
	Platform *Box3;
	Platform *Box4;
	Platform *Box5;
	Platform *Box6;
	Platform *Box7;
	Platform *Box8;
	Platform *Box9;
	Platform *Box10;
	Platform *Box11;
	switch(_level) {
		case 1:
			string cube = "assets\\obj\\cube.obj";
			string texture = "assets\\bmp\\checkerboard.bmp";
			Box = new Platform(cube);
			Box->Scale(1, 1, 10);
			Box->Translate(0,-10,0);
			Box->Texture(space_image, texture);

			Box2 = new Platform(cube);
			Box2->Scale(1, 5, 1);
			Box2->Translate(0,-13,-6);
			Box2->Texture(space_image, texture);

			Box3 = new Platform(cube);
			Box3->Scale(1, 1, 5);
			Box3->Translate(2,-10,0);
			Box3->Texture(space_image, texture);

			Box4 = new Platform(cube);
			Box4->Scale(1, 1, 5);
			Box4->Translate(-2,-10,0);
			Box4->Texture(space_image, texture);

			Box5 = new Platform(cube);
			Box5->Scale(1, 5, 1);
			Box5->Translate(0,-15,-8);
			Box5->Texture(space_image, texture);

			Box6 = new Platform(cube);
			Box6->Scale(2, 2, 2);
			Box6->Translate(0,-15,-11);
			Box6->Rotate(45, 0, 1, 1);
			Box6->Texture(space_image, texture);
	
			Box7 = new Platform(cube);
			Box7->Scale(10, 2, 2);
			Box7->Translate(0,-15,-14);
			Box7->Rotate(35, 1, 0, 0);
			Box7->Texture(space_image, texture);

			Box8 = new Platform(cube);
			Box8->Scale(8, 2, 2);
			Box8->Translate(0,-15,-16);
			Box8->Rotate(35, 1, 0, 0);
			Box8->Texture(space_image, texture);

			Box9 = new Platform(cube);
			Box9->Scale(6, 2, 2);
			Box9->Translate(0,-15,-18);
			Box9->Rotate(35, 1, 0, 0);
			Box9->Texture(space_image, texture);

			Box10 = new Platform(cube);
			Box10->Scale(4, 2, 2);
			Box10->Translate(0,-15,-20);
			Box10->Rotate(35, 1, 0, 0);
			Box10->Texture(space_image, texture);
			
			Box11 = new Platform(cube);
			Box11->Scale(2, 2, 2);
			Box11->Translate(0,-15,-22);
			Box11->Texture(space_image, texture);
			

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
	
