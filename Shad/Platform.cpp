#include <Shad/Platform.h>

Platform::Platform(string model) {
	platform = (new PolyMesh())->LoadObj(model)->GenerateRigidBody();
	//Plane->AttachShader(shader);
	platform->RigidBody->setRollingFriction(0.8f);
	platform->RigidBody->setAnisotropicFriction(platform->RigidBody->getCollisionShape()->getAnisotropicRollingFrictionDirection(),btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);
}

void Platform::Scale(float scalex, float scaley, float scalez) {
	platform->Scale(OpenMesh::Vec3f(scalex, scaley, scalez));
}

void Platform::Translate(float tx, float ty, float tz) {
	platform->Translate(OpenMesh::Vec3f(tx, ty, tz));
}

void Platform::Rotate(float angle, float x, float y, float z) {
	platform->Rotate(angle, x, y, z);
}

void Platform::Texture(bitmap_image & image, string texture) {
	image = bitmap_image(texture);
	image.rgb_to_bgr();
	platform->ApplyTexture(image.data(), image.width(), image.height());
}