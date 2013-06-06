#include <Shad/Platform.h>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

Platform::Platform(std::string model) {
	platformMesh = (new PolyMesh())->LoadObj(model)->GenerateRigidBody();
	
	//translucent red platform?
	float *platformColor = new float[4];
	platformColor[0] = 1.0;
	platformColor[1] = 1.0;
	platformColor[2] = 1.0;
	platformColor[3] = 0.5;
	platformMesh->Color = platformColor;

	platformMesh->RigidBody->setRollingFriction(0.8f);
	platformMesh->RigidBody->setAnisotropicFriction(platformMesh->RigidBody->getCollisionShape()->getAnisotropicRollingFrictionDirection(),btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);

	GenerateEdges();
}

Platform::~Platform() {
	Physics::DynamicsWorld->removeRigidBody(platformMesh->RigidBody);
	delete platformMesh->RigidBody;
	delete platformMesh;
}

void Platform::GenerateEdges()
{
	for (PolyMesh::EdgeIter e_it = platformMesh->edges_begin(); e_it != platformMesh->edges_end(); ++e_it)
	{
		// Obtain half edges
		PolyMesh::HalfedgeHandle heHandle = platformMesh->halfedge_handle(e_it.handle(), 0);

		// Obtain vertices bounding the edge
		PolyMesh::VertexHandle v1 = platformMesh->to_vertex_handle(heHandle);
		PolyMesh::VertexHandle v2 = platformMesh->from_vertex_handle(heHandle);

		OpenMesh::Vec3f startPoint = platformMesh->point(v1);
		OpenMesh::Vec3f endPoint = platformMesh->point(v2);
		
		float edgeLength = (endPoint - startPoint).length();
	
		// Is it a diagonal?
		if (edgeLength < 1.4) {
			PlatformEdge *edge = new PlatformEdge(startPoint, endPoint);
			edges.push_back(edge);
		}
	}
}

Platform *Platform::Scale(float scalex, float scaley, float scalez) {
	platformMesh->Scale(OpenMesh::Vec3f(scalex, scaley, scalez));
	for (unsigned int i = 0; i < edges.size(); i++) {
		edges[i]->Scale(scalex,scaley,scalez);
	}
	return this;
}

Platform *Platform::Translate(float tx, float ty, float tz) {
	platformMesh->Translate(OpenMesh::Vec3f(tx, ty, tz));
	for (unsigned int i = 0; i < edges.size(); i++) {
		edges[i]->Translate(tx,ty,tz);
	}
	return this;
}

Platform *Platform::Rotate(float angle, float x, float y, float z) {
	platformMesh->Rotate(angle, x, y, z);
	for (unsigned int i = 0; i < edges.size(); i++) {
		edges[i]->Rotate(angle,x,y,z);
	}
	return this;
}

void Platform::Texture(bitmap_image & image, std::string texture) {
	image = bitmap_image(texture);
	image.rgb_to_bgr();
	platformMesh->ApplyTexture(image.data(), image.width(), image.height());
}

