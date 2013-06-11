#include <Shad/Platform.h>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

struct bounds{
	float low;
	float high;
	bool set;
};

Platform::Platform(std::string model) {
	platformMesh = (new PolyMesh())->LoadObj(model)->GenerateRigidBody();
	mod = model;
	//translucent red platform?
	float *platformColor = new float[4];
	platformColor[0] = 0.0;
	platformColor[1] = 0.0;
	platformColor[2] = 0.0;
	platformColor[3] = 0.5;
	platformMesh->Color = platformColor;

	platformMesh->RigidBody->setRollingFriction(0.8f);
	platformMesh->RigidBody->setAnisotropicFriction(platformMesh->RigidBody->getCollisionShape()->getAnisotropicRollingFrictionDirection(),btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);

	GenerateEdges();
	collapsible = false;
	moving = false;
	resetBool = true;
	collapsing = false;
	deformed = false;

	color[0] = 1.0f;
	color[1] = 1.0f;
	color[2] = 1.0f;
	color[3] = 0.6f;

	runningTotalX = 0;
	runningTotalY = 0;
	runningTotalZ = 0;

}

Platform::~Platform() {
	Physics::DynamicsWorld->removeRigidBody(platformMesh->RigidBody);
	delete platformMesh->RigidBody;
	delete platformMesh;
	
}

void Platform::GenerateVertices()
{
	if (collapsible) {
		for (PolyMesh::VertexIter v_it = platformMesh->vertices_begin(); v_it != platformMesh->vertices_end(); ++v_it)
		{
			PolyMesh::VertexHandle v_handle =v_it.handle();
		
			OpenMesh::Vec3f coordinate = platformMesh->point(v_handle);

			Vertex * v = new Vertex(coordinate, v_handle);

			vertices.push_back(v);

		}
	}
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

float * Platform::getColor() {
	return color;
}

Platform *Platform::Scale(float scalex, float scaley, float scalez) {
	platformMesh->Scale(OpenMesh::Vec3f(scalex, scaley, scalez));
	for (unsigned int i = 0; i < edges.size(); i++) {
		edges[i]->Scale(scalex,scaley,scalez);
	}
	if (collapsible) {
		_scaleX = scalex;
		_scaleY = scaley;
		_scaleZ = scalez;
	}
	return this;
}

Platform *Platform::Translate(float tx, float ty, float tz) {
	platformMesh->Translate(OpenMesh::Vec3f(tx, ty, tz));
	for (unsigned int i = 0; i < edges.size(); i++) {
		edges[i]->Translate(tx,ty,tz);
	}

	if (collapsible) {
		initialX = tx;
		initialY = ty;
		initialZ =	tz;
	}

	return this;
}

Platform *Platform::Rotate(float angle, float x, float y, float z) {
	platformMesh->Rotate(angle, x, y, z);
	for (unsigned int i = 0; i < edges.size(); i++) {
		edges[i]->Rotate(angle,x,y,z);
	}
	if (collapsible) {
		_angle = angle;
		xaxis = x;
		yaxis = y;
		zaxis = z;
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


bool Platform::isShrinking() {

	return shrinking;
}


void Platform::setMoving(int _beat, float _deltaX, float _deltaY, float _deltaZ) {
	beats = _beat;
	moving = true;
	deltaX = _deltaX;
	deltaY = _deltaY;
	deltaZ = _deltaZ;
	color[0] = 1.0f;
	color[1] = 1.0f;
	color[2] = 0.0f;
	color[3] = 1.0f;
}

int Platform::getBeat() {
	return beats;
}

void Platform::setShrinking(bool state, float scaleX, float scaleY, float scaleZ) {
	shrinking = state;
	_scaleX = scaleX;
	_scaleY = scaleY;
	_scaleZ = scaleZ;
	color[0] = 0.0f;
	color[1] = 1.0f;
	color[2] = 0.0f;
	color[3] = 1.0f;
}

OpenMesh::Vec3f Platform::getDirection() {
	return OpenMesh::Vec3f(deltaX, deltaY, deltaZ);
}

OpenMesh::Vec3f Platform::getShrinking() {
	return OpenMesh::Vec3f(_scaleX, _scaleY, _scaleZ);
}


void Platform::move(uint64_t deltaPoint) {
	deltaPoint%=beats;
	OpenMesh::Vec3f direction = getDirection();
	if (deltaPoint < beats/2) {
		platformMesh->Translate(OpenMesh::Vec3f(direction[0], direction[1], direction[2]));
		for (unsigned int i = 0; i < edges.size(); i++)
			edges[i]->Translate(direction[0], direction[1], direction[2]);
	}
	else {
		platformMesh->Translate(OpenMesh::Vec3f(-direction[0], -direction[1], -direction[2]));
		for (unsigned int i = 0; i < edges.size(); i++)
			edges[i]->Translate(-direction[0], -direction[1], -direction[2]);
	}
}

void Platform::shrink(uint64_t deltaPoint) {

	OpenMesh::Vec3f shrinking = getShrinking();

	if (deltaPoint < 5) {
		platformMesh->Scale(OpenMesh::Vec3f(shrinking[0], shrinking[1], shrinking[2]));
		for (unsigned int i = 0; i < edges.size(); i++)
			edges[i]->Scale(shrinking[0], shrinking[1], shrinking[2]);
	}
	else {
		platformMesh->Scale(OpenMesh::Vec3f(1.0f/shrinking[0], 1.0f/shrinking[1], 1.0f/shrinking[2]));
		for (unsigned int i = 0; i < edges.size(); i++)
			edges[i]->Scale(1.0f/shrinking[0], 1.0f/shrinking[1], 1.0f/shrinking[2]);
	}
}


bool Platform::moveWChar(uint64_t deltaPoint, float charX, float charY, float charZ) {
	move(deltaPoint);
	return withInBounds(charX, charY, charZ);

}

bool Platform::withInBounds(float charX, float charY, float charZ) {
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

	if (xBounds.high < xBounds.low) {
		float temp = xBounds.high;
		xBounds.high = xBounds.low;
		xBounds.low = temp;
	}

	if (zBounds.high < zBounds.low) {
		float temp = zBounds.high;
		zBounds.high = zBounds.low;
		zBounds.low = temp;
	}
	if (yBounds.high < yBounds.low) {
		float temp = yBounds.high;
		yBounds.high = yBounds.low;
		yBounds.low = temp;
	}
	xBounds.low -=0.15;
	xBounds.high +=0.15;
	zBounds.low -=0.15;
	zBounds.low +=0.15;

	if (xBounds.low < charX && xBounds.high > charX) {
		if (zBounds.low < charZ && zBounds.high > charZ) {
			if (charY - yBounds.high < 0.54) {
				return true;
			}
		}
	}

	return false;
}

void Platform::setCollapsible() {
	collapsible = true;


	color[0] = 170.0f/255.0f;
	color[1] = 240.0f/255.0f;
	color[2] = 141.0f/255.0f;
	color[3] = 1.0f;

}

bool Platform::checkIfGood(float limit) {
	PlatformEdge * e= edges[0];
	OpenMesh::Vec3f loc = e->getStartPoint();
	if (loc[2] < limit) {
		return false;
	}
	return true;
}

void Platform::collapse(bool onGround, float charX, float charY, float charZ) {
	if (withInBounds(charX, charY, charZ) && onGround) {
		Translate(0, -0.2, 0);
		runningTotalX += 0;
		runningTotalY -= 0.2;
		runningTotalZ += 0;
	}
}

void Platform::reset() {
	//platformMesh->SetOrigin(OpenMesh::Vec3f(initialX, initialY, initialZ));
	Translate(-runningTotalX, -runningTotalY, -runningTotalZ);
	runningTotalX = 0;
	runningTotalY = 0;
	runningTotalZ = 0;
}

bool Platform::isCollapsible() {
	return collapsible;
}

void Platform::GenerateAllEdges() {
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
	

		PlatformEdge *edge = new PlatformEdge(startPoint, endPoint);
		edges.push_back(edge);

	}
}

void Platform::deform(bool onGround, float charX, float charY, float charZ) {
/*	if (!deformed) {
		if (onGround) {
			if (withInBounds(charX, charY, charZ)) {
				for (unsigned int i = 0; i < vertices.size(); i++)
				{
					OpenMesh::Vec3f coord = vertices[i]->getCoordinates();
					OpenMesh::VertexHandle vh = vertices[i]->getVH();
					std::cout << "The old y " << coord[1] << std::endl;
					coord[0] = -1;
					coord[1] = -1;
					coord[2] = -1;
					std::cout << "The new y " << coord[1] << std::endl;
					platformMesh->set_point(vh, coord);
					if (i >2)
						break;
				}
				deformed = true;
			}
		}	
	}*/
}

void Platform::subdivide() {
/*	platformMesh->LoopSubdivideP(100);
	collapsible = true;
	color[0] = 1.0f;
	color[1] = 0.0f;
	color[2] = 0.0f;
	color[3] = 1.0f;
	for(unsigned int i = 0; i < edges.size(); i++) {
		edges[i]->Scale(0,0,0);
	}
	GenerateAllEdges();
	GenerateVertices();*/
	
}
