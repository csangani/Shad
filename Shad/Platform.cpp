#include <Shad/Platform.h>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

struct bounds{
	float low;
	float high;
	bool set;
};

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


	moving = false;

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


bool Platform::isMoving() {
	return moving;
}

void Platform::setMoving(bool state, float _deltaX, float _deltaY, float _deltaZ) {
	moving = state;
	deltaX = _deltaX;
	deltaY = _deltaY;
	deltaZ = _deltaZ;
}

void Platform::moveX(int deltaPoint) {
	if (deltaPoint < 5) {
		platformMesh->Translate(OpenMesh::Vec3f(-0.2, 0, 0));
	}
	else {
		platformMesh->Translate(OpenMesh::Vec3f(0.2, 0, 0));
	}
}


void Platform::moveY(int deltaPoint) {
	if (deltaPoint < 5) {
		platformMesh->Translate(OpenMesh::Vec3f(0, -0.2, 0));
	}
	else {
		platformMesh->Translate(OpenMesh::Vec3f(0, 0.2, 0));
	}
}


void Platform::moveWChar(int deltaPoint) {
	//platformMesh->
	struct bounds xBounds;
	xBounds.set = false;
	struct bounds yBounds;
	yBounds.set = false;
	struct bounds zBounds;
	zBounds.set = false;
	for (uint i = 0; i < edges.size(); i++)
	{
		OpenMesh::Vec3f start = edges[i]->getStartPoint();
		OpenMesh::Vec3f end = edges[i]->getEndPoint();

		if (xBounds.set == false) {
			xBounds.set = true;
			xBounds.low = start[0];
			xBounds.high = start[0];

			if (xBounds.low > end[0]) {
				xBounds.low = end[0];
			}
			if (xBounds.high < end[0]) {
				xBounds.high = end[0];
			}
		}
		else {
			if (xBounds.low < start[0]) {
				xBounds.low = start[0];
			}
			if (xBounds.low < end[0]) {
				xBounds.low = end[0];
			}
			if (xBounds.high > start[0]) {
				xBounds.high = start[0];
			}
			if (xBounds.high > end[0]) {
				xBounds.high = end[0];
			}
		}

		if (yBounds.set == false) {
			yBounds.set = true;
			yBounds.low = start[1];
			yBounds.high = start[1];

			if (yBounds.low > end[1]) {
				yBounds.low = end[1];
			}
			if (yBounds.high < end[1]) {
				yBounds.high = end[1];
			}
		}
		else {
			if (yBounds.low < start[1]) {
				yBounds.low = start[1];
			}
			if (yBounds.low < end[1]) {
				yBounds.low = end[1];
			}
			if (yBounds.high > start[1]) {
				yBounds.high = start[1];
			}
			if (yBounds.high > end[1]) {
				yBounds.high = end[1];
			}
		}

		if (zBounds.set == false) {
			zBounds.set = true;
			zBounds.low = start[2];
			zBounds.high = start[2];

			if (zBounds.low > end[2]) {
				zBounds.low = end[2];
			}
			if (zBounds.high < end[2]) {
				zBounds.high = end[2];
			}
		}
		else {
			if (zBounds.low < start[2]) {
				zBounds.low = start[2];
			}
			if (zBounds.low < end[2]) {
				zBounds.low = end[2];
			}
			if (zBounds.high > start[2]) {
				zBounds.high = start[2];
			}
			if (zBounds.high > end[2]) {
				zBounds.high = end[2];
			}
		}
	}
}
