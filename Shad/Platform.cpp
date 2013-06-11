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
	elevator = false;

	color[0] = 1.0f;
	color[1] = 1.0f;
	color[2] = 1.0f;
	color[3] = 0.6f;

	runningTotalX = 0;
	runningTotalY = 0;
	runningTotalZ = 0;

	counter = 0;
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
	int l = _beat/=2;
	limitX = _deltaX*l;
	limitY = _deltaY*l;
	limitZ = _deltaZ*l;
	runningTotalX = 0;
	runningTotalY = 0;
	runningTotalZ = 0;
}

int Platform::getBeat() {
	return beats;
}


void Platform::setShrinking(int _beat, float scale) {
	scaleBeat = _beat;
	shrinking = true;
	_shrinkX = scale;
	_shrinkY = scale;
	_shrinkZ = scale;
	color[0] = 1.0f;
	color[1] = 0.0f;
	color[2] = 0.0f;
	color[3] = 1.0f;
}

OpenMesh::Vec3f Platform::getDirection() {
	return OpenMesh::Vec3f(deltaX, deltaY, deltaZ);
}

OpenMesh::Vec3f Platform::getShrinking() {
	return OpenMesh::Vec3f(_shrinkX, _shrinkY, _shrinkZ);
}


void Platform::move(uint64_t deltaPoint) {
	deltaPoint%=beats;

	if (counter == beats) {
		counter = 0;
	}
	OpenMesh::Vec3f direction = getDirection();
	if (counter < beats/2) {
	//	if (runningTotalX <= limitX && runningTotalY <= limitY && runningTotalZ <= limitZ) {
			platformMesh->Translate(OpenMesh::Vec3f(direction[0], direction[1], direction[2]));
			for (unsigned int i = 0; i < edges.size(); i++)
				edges[i]->Translate(direction[0], direction[1], direction[2]);
			runningTotalX += direction[0];
			runningTotalY += direction[1];
			runningTotalZ += direction[2];
	//	}
	}
	else {
	//	if (runningTotalX >= limitX && runningTotalY >= limitY && runningTotalZ >= limitZ) {

			platformMesh->Translate(OpenMesh::Vec3f(-direction[0], -direction[1], -direction[2]));
			for (unsigned int i = 0; i < edges.size(); i++)
				edges[i]->Translate(-direction[0], -direction[1], -direction[2]);
			runningTotalX -= direction[0];
			runningTotalY -= direction[1];
			runningTotalZ -= direction[2];
	//	}
	}
	counter++;
}

void Platform::UpdateCounter() {
	counter++;
}

void Platform::shrink(uint64_t deltaPoint) {
	deltaPoint%=scaleBeat; 
	OpenMesh::Vec3f shrinking = getShrinking();
	btVector3 centerOfMass = platformMesh->RigidBody->getCenterOfMassPosition();

	if (counter < scaleBeat/2) {
		platformMesh->Scale(shrinking);
		for (unsigned int i = 0; i < edges.size(); i++)
			edges[i]->SpecialScale(shrinking[0], shrinking[1], shrinking[2],OpenMesh::Vec3f(centerOfMass.x(),centerOfMass.y(),centerOfMass.z()));
	}
	else {
		OpenMesh::Vec3f shrink(1.0f/shrinking[0],1.0f/shrinking[1],1.0f/shrinking[2]);
		platformMesh->Scale(shrink);
		for (unsigned int i = 0; i < edges.size(); i++)
			edges[i]->SpecialScale(1.0f/shrinking[0], 1.0f/shrinking[1], 1.0f/shrinking[2],OpenMesh::Vec3f(centerOfMass.x(),centerOfMass.y(),centerOfMass.z()));
	}
	counter++;
	if (counter == scaleBeat) {
		counter = 0;
	}
}


bool Platform::carry(bool onGround, float charX, float charY, float charZ) {
	if (carryCheckX <= limitX && carryCheckY <= limitY && carryCheckZ <= limitZ)
		collapse(onGround, charX, charY, charZ);
	else
		finish = true;
	return withInBounds(charX, charY, charZ);

}

bool Platform::stopped() {
	return finish;
}

bool Platform::moveWChar(uint64_t deltaPoint, float charX, float charY, float charZ) {
	move(deltaPoint);
	return withInBounds(charX, charY, charZ);

}

int Platform::getCounter() {
	return counter;
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
	xBounds.low -=0.2;
	xBounds.high +=0.2;
	zBounds.low -=0.2;
	zBounds.low +=0.2;

	if (xBounds.low < charX && xBounds.high > charX) {
		if (zBounds.low < charZ && zBounds.high > charZ) {
			if (abs(charY - yBounds.high) < 1 && charY > yBounds.high) {
				return true;
			}
		}
	}
	return false;
}

void Platform::setElevator(float xMove, float yMove, float zMove, float stopX, float stopY, float stopZ) {
	collapsible = true;
	elevator = true;
	finish = false;
	deltaX = xMove;
	deltaY = yMove;
	deltaZ = zMove;
	limitX = abs(stopX);
	limitY = abs(stopY);
	limitZ = abs(stopZ);
	runningTotalX = 0;
	runningTotalY = 0;
	runningTotalZ = 0;
	carryCheckX = 0;
	carryCheckY = 0;
	carryCheckZ = 0;

	color[0] = 130.0f/255.0f;
	color[1] = 2230.0f/255.0f;
	color[2] = 231.0f/255.0f;
	color[3] = 1.0f;
}


void Platform::setCollapsible() {
	collapsible = true;

	deltaX = 0.0f;
	deltaY = -0.2f;
	deltaZ = 0.0f;

	color[0] = 0.0;
	color[1] = 1.0;
	color[2] = 0.0;

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
		Translate(deltaX, deltaY, deltaZ);
		runningTotalX += deltaX;
		runningTotalY += deltaY;
		runningTotalZ += deltaZ;
		carryCheckX += abs(deltaX);
		carryCheckY += abs(deltaY);
		carryCheckZ += abs(deltaZ);
	}
}

void Platform::reset() {
	//platformMesh->SetOrigin(OpenMesh::Vec3f(initialX, initialY, initialZ));
	Translate(-runningTotalX, -runningTotalY, -runningTotalZ);
	runningTotalX = 0;
	runningTotalY = 0;
	runningTotalZ = 0;
	carryCheckX = 0;
	carryCheckY = 0;
	carryCheckZ = 0;
	finish = false;
}

bool Platform::isCollapsible() {
	return collapsible;
}


void Platform::deform(bool onGround, float charX, float charY, float charZ) {
/*	if (!deformed) {
		if (onGround) {
			if (withInBounds(charX, charY, charZ)) {
				for (unsigned int i = 0; i < vertices.size(); i++)
				{
					OpenMesh::Vec3f coord = vertices[i]->getCoordinates();
					OpenMesh::VertexHandle vh = vertices[i]->getVH();
					coord[0] = -1;
					coord[1] = -1;
					coord[2] = -1;
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
