#include <PolyMesh\Cloth.h>
#include <omp.h>

Cloth::Cloth(float mass, float drag, OpenMesh::Vec3f RowVec,OpenMesh::Vec3f ColVec,OpenMesh::Vec3f Origin,int rows, int columns, float stretch, float bend, float segmentlength) : rows(rows), columns(columns), stretch (stretch), bend(bend), segmentLength(segmentlength), drag(drag) {
	__super::Cloth = true;
	btVector3 bOrigin(Origin[0], Origin[1], Origin[2]);
	btVector3 bRowVec(RowVec[0], RowVec[1], RowVec[2]);
	btVector3 bColVec(ColVec[0], ColVec[1], ColVec[2]);
	request_vertex_texcoords2D();
	/* Add vertices and rigid bodies */
	for (int i = 0; i < rows; i++) {
		Points.push_back(std::vector<VertexHandle>());
		RigidBody.push_back(std::vector<btRigidBody *>());
		for (int j = 0; j < columns; j++) {
			Points[i].push_back(add_vertex(Origin + RowVec.normalized() * (float)i * segmentLength + ColVec.normalized() * (float)j * segmentLength));
			set_texcoord2D(Points[i][j],OpenMesh::Vec2f(1.0f/(rows-1)*i,(1.0f/(columns-1)*j)));
			btCollisionShape *Sphere = new btSphereShape(0.00001f);
			btVector3 localInertia(0.0f, 0.0f, 0.0f);
			btScalar m(mass);
			if (m != 0.0f)
				Sphere->calculateLocalInertia(m, localInertia);
			btTransform transform;
			transform.setIdentity();
			transform.setOrigin(bOrigin + bRowVec.normalized() * (float)i * segmentLength + bColVec.normalized() * (float)j * segmentLength);
			btDefaultMotionState *MS = new btDefaultMotionState(transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(m,MS,Sphere,localInertia);
			btRigidBody *body = new btRigidBody(rbInfo);
			body->setContactProcessingThreshold(0.0f);
			Physics::DynamicsWorld->addRigidBody(body);
			RigidBody[i].push_back(body);
			body->setActivationState(DISABLE_DEACTIVATION);
			body->setDamping(0.2f,0.2f);
			body->setFriction(0.9f);
			body->setHitFraction(0.9f);
			body->setRollingFriction(0.9f);
			body->setRestitution(0.0f);
			body->setGravity(btVector3(0,-1.0f,0));
			body->setSleepingThresholds(0.2f,0.2f);
		}
	}

	/* Add faces for rendering */
	for (int i = 0; i < rows-1; i++) {
		for (int j = 0; j < columns-1; j++) {
			add_face(Points[i][j], Points[i+1][j], Points[i][j+1]);
			add_face(Points[i][j+1], Points[i+1][j], Points[i+1][j+1]);
		}
	}
}

int Cloth::Pin(int row, int col, PolyMesh *M, VertexHandle H) {
	PinList.insert(std::pair<int,PinnedPoint>(columns*row + col, PinnedPoint(H, M)));
	RigidBody[row][col]->setMassProps(0.0f, btVector3(0,0,0));
	return columns*row + col;
}

void Cloth::Unpin(int handle){
	PinList.erase(handle);
}

void Cloth::SimulationStep() {
	float diagonal = segmentLength * sqrtf(2);

	for (std::map<int, PinnedPoint>::iterator iter = PinList.begin(); iter != PinList.end(); iter++)
	{
		btTransform transform;
		transform.setIdentity();
		OpenMesh::Vec3f origin = iter->second.m->point(iter->second.h);
		transform.setOrigin(btVector3(origin[0], origin[1], origin[2]) + iter->second.m->RigidBody->getCenterOfMassPosition());
		RigidBody[iter->first/columns][iter->first%columns]->setCenterOfMassTransform(transform);
	}

	// Synchronize mesh points and collision objects
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < columns; j++) {
			set_point(Points[i][j], OpenMesh::Vec3f(
				RigidBody[i][j]->getCenterOfMassPosition().getX(),
				RigidBody[i][j]->getCenterOfMassPosition().getY(),
				RigidBody[i][j]->getCenterOfMassPosition().getZ()));
		}
	}
	// Set forces
#pragma omp parallel for default (none) firstprivate(diagonal)
	for (int k = 0; k < rows * columns; k++) {
		int i = k / columns;
		int j = k % columns;
		// Stretch Top
		if (i > 0) {
			float dist = (RigidBody[i-1][j]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i-1][j]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			RigidBody[i][j]->applyForce(dir * (dist - segmentLength) * stretch, btVector3(0,0,0));
		}
		// Stretch Bottom
		if (i < rows-1) {
			float dist = (RigidBody[i+1][j]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i+1][j]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			RigidBody[i][j]->applyForce(dir * (dist - segmentLength) * stretch, btVector3(0,0,0));
		}
		// Stretch Left
		if (j > 0) {
			float dist = (RigidBody[i][j-1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i][j-1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			RigidBody[i][j]->applyForce(dir * (dist - segmentLength) * stretch, btVector3(0,0,0));
		}
		// Stretch Right
		if (j < columns-1) {
			float dist = (RigidBody[i][j+1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i][j+1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			RigidBody[i][j]->applyForce(dir * (dist - segmentLength) * stretch, btVector3(0,0,0));
		}
		// Stretch Top Right
		if (i > 0 && j < columns-1) {
			float dist = (RigidBody[i-1][j+1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i-1][j+1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			RigidBody[i][j]->applyForce(dir * (dist - diagonal) * stretch, btVector3(0,0,0));
		}
		// Stretch Bottom Right
		if (i < rows-1 && j < columns-1) {
			float dist = (RigidBody[i+1][j+1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i+1][j+1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			RigidBody[i][j]->applyForce(dir * (dist - diagonal) * stretch, btVector3(0,0,0));
		}
		// Stretch Top Left
		if (i > 0 && j > 0) {
			float dist = (RigidBody[i-1][j-1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i-1][j-1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			RigidBody[i][j]->applyForce(dir * (dist - diagonal) * stretch, btVector3(0,0,0));
		}
		// Stretch Bottom Left
		if (i < rows-1 && j > 0) {
			float dist = (RigidBody[i+1][j-1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i+1][j-1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			RigidBody[i][j]->applyForce(dir * (dist - diagonal) * stretch, btVector3(0,0,0));
		}
		// Bend Top
		if (i > 1) {
			float dist = (RigidBody[i-2][j]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i-2][j]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			RigidBody[i][j]->applyForce(dir * (dist - 2.0f*segmentLength) * bend, btVector3(0,0,0));
		}
		// Bend Bottom
		if (i < rows-2) {
			float dist = (RigidBody[i+2][j]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i+2][j]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			RigidBody[i][j]->applyForce(dir * (dist - 2.0f*segmentLength) * bend, btVector3(0,0,0));
		}
		// Bend Left
		if (j > 1) {
			float dist = (RigidBody[i][j-2]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i][j-2]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			RigidBody[i][j]->applyForce(dir * (dist - 2.0f*segmentLength) * bend, btVector3(0,0,0));
		}
		// Bend Right
		if (j < columns-2) {
			float dist = (RigidBody[i][j+2]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i][j+2]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			RigidBody[i][j]->applyForce(dir * (dist - 2.0f*segmentLength) * bend, btVector3(0,0,0));
		}
	}
	
#pragma omp parallel for
	for (int k = 0; k < (rows-1) * (columns-1); k++) {
		int i = k / (columns-1);
		int j = k % (columns-1);
		// Upper Triangle
		btVector3 Normal = (RigidBody[i][j]->getCenterOfMassPosition()-RigidBody[i][j+1]->getCenterOfMassPosition()).cross
			(RigidBody[i+1][j]->getCenterOfMassPosition() - RigidBody[i][j]->getCenterOfMassPosition()).normalized();
		btVector3 PlaneVelocity = Normal * (RigidBody[i][j]->getLinearVelocity() + RigidBody[i+1][j]->getLinearVelocity() + RigidBody[i][j+1]->getLinearVelocity()).dot(Normal);
		RigidBody[i][j]->applyForce(-PlaneVelocity * drag,btVector3(0,0,0));
		RigidBody[i+1][j]->applyForce(-PlaneVelocity * drag,btVector3(0,0,0));
		RigidBody[i][j+1]->applyForce(-PlaneVelocity * drag,btVector3(0,0,0));
		// Lower Triangle
		Normal = (RigidBody[i+1][j]->getCenterOfMassPosition()-RigidBody[i][j+1]->getCenterOfMassPosition()).cross
			(RigidBody[i+1][j+1]->getCenterOfMassPosition() - RigidBody[i+1][j]->getCenterOfMassPosition()).normalized();
		PlaneVelocity =  Normal * (RigidBody[i+1][j+1]->getLinearVelocity() + RigidBody[i+1][j]->getLinearVelocity() + RigidBody[i][j+1]->getLinearVelocity()).dot(Normal);
		RigidBody[i+1][j+1]->applyForce(-PlaneVelocity * drag,btVector3(0,0,0));
		RigidBody[i+1][j]->applyForce(-PlaneVelocity * drag,btVector3(0,0,0));
		RigidBody[i][j+1]->applyForce(-PlaneVelocity * drag,btVector3(0,0,0));
	}

	GenerateNormals();
}