#include <string>

#include <Shad/Level.h>

#include <GL\glew.h>
#include <GL\glut.h>



using namespace std;

Level::Level(int level) {
	_level = level;
	LEVEL = "level";
	EXT = ".ext";
	platforms = std::vector<Platform *>();
}

	
void Level::generateBlocks(string shader, bitmap_image& space_image) {
	string cube = "assets\\obj\\cube.obj";
	Platform *platform;
	switch(_level) {
		case 1:
			platforms.push_back((new Platform(cube))->Scale(1, 1, 10)->Translate(0,-10,0));

			platforms.push_back((new Platform(cube))->Scale(1, 5, 1)->Translate(0,-13,-6));

			platforms.push_back((new Platform(cube))->Scale(1, 1, 5)->Translate(2,-10,0));

			platforms.push_back((new Platform(cube))->Scale(1, 1, 5)->Translate(-2,-10,0));

			platforms.push_back((new Platform(cube))->Scale(1, 5, 1)->Translate(0,-15,-8));

			platforms.push_back((new Platform(cube))->Rotate(45, 0, 1, 1)->Scale(2, 2, 2)->Translate(0,-15,-11));

			platforms.push_back((new Platform(cube))->Rotate(35, 1, 0, 0)->Scale(10, 2, 2)->Translate(0,-15,-14));

			platforms.push_back((new Platform(cube))->Rotate(35, 1, 0, 0)->Scale(8, 2, 2)->Translate(0,-15,-16));

			platforms.push_back((new Platform(cube))->Rotate(35, 1, 0, 0)->Scale(6, 2, 2)->Translate(0,-15,-18));

			platforms.push_back((new Platform(cube))->Rotate(35, 1, 0, 0)->Scale(4, 2, 2)->Translate(0,-15,-20));

			platforms.push_back((new Platform(cube))->Scale(2, 2, 2)->Translate(0,-15,-22));

			target = OpenMesh::Vec3f(0, -14, -22);
			break;
		case 2:
			platform = new Platform(cube);
			platform->Scale(1, 1, 1);
			platform->Translate(0, -5, 0);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(1, 5, 1);
			platform->Translate(0, -5, -3);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(5, 1, 5);
			platform->Translate(0, -5, -8);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(5, 10, 1);
			platform->Translate(0, -11, -14);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(1, 10, 5);
			platform->Translate(3, -11, -16);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(1, 4, 5);
			platform->Translate(-2, -8, -16);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(10, 1, 5);
			platform->Translate(-6, -12, -16);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(5, 10, 1);
			platform->Translate(0, -11, -18);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(1, 5, 1);
			platform->Translate(-13, -10, -14);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(1, 5, 1);
			platform->Translate(-13, -10, -19);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(5, 1, 6);
			platform->Translate(-12, -8, -16);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(5, 1, 6);
			platform->Translate(-14, -10, -16);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(2, 2, 2);
			platform->Translate(-20, -10, -16);
			platforms.push_back(platform);

			break;
	};
}

void Level::createPlatform(float scaleX, float scaleY, float scaleZ, float translateX, float translateY, float translateZ, float rotationAngle, float rotateX, float rotateY, float rotateZ)
{
	string cube = "assets\\obj\\cube.obj";
	Platform *platform = new Platform(cube);
	if (rotationAngle > 0) {
		platform->Rotate(rotationAngle, rotateX, rotateY, rotateZ);
	}
	platform->Scale(scaleX, scaleY, scaleZ);
	platform->Translate(translateX, translateY, translateZ);
	platforms.push_back(platform);
}

void Level::drawPlatformEdges()
{
	for (unsigned int i = 0; i < platforms.size(); i++) {
		Platform *platform = platforms[i];
		for (unsigned int j = 0; j < platform->edges.size(); j++) {
			PlatformEdge *edge = platform->edges[j];
			edge->Draw();
		}
	}
}

void Level::destroyPlatforms() {
	for(int i = 0; i < platforms.size(); i++) {
		Platform * p =  platforms[i];
		delete p;
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

void Level::setTarget(float x, float y, float z) {
	target = OpenMesh::Vec3f(x, y, z);
}

OpenMesh::Vec3f Level::getTarget() {
	return target;
}
