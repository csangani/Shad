#include <PolyMesh\Cloth.h>
#include <omp.h>

float ClothSpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f };

float ClothDiffuse[] = { 0.2f, 0.2f, 0.2f, 1.0f };

float ClothAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

float ClothShininess[] = { 0.2f };

Cloth::Cloth(float mass, float drag, float damping, OpenMesh::Vec3f RowVec,OpenMesh::Vec3f ColVec,OpenMesh::Vec3f Origin,int rows, int columns, float stretch, float bend, float segmentlength, BVEC3F& wind)
	: rows(rows), columns(columns), stretch (stretch), bend(bend), segmentLength(segmentlength), drag(drag), damping(damping), wind(wind) {
		__super::cloth = true;
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
				set_texcoord2D(Points[i][j],OpenMesh::Vec2f(i*1.0f/(rows-1),j*(1.0f/(columns-1))));
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
				Physics::DynamicsWorld->addRigidBody(body, btBroadphaseProxy::DebrisFilter, btBroadphaseProxy::StaticFilter);
				RigidBody[i].push_back(body);
				body->setActivationState(DISABLE_DEACTIVATION);
				body->setFriction(0.9f);
				body->setHitFraction(0.9f);
				body->setRollingFriction(0.9f);
				body->setRestitution(0.0f);
				body->setGravity(btVector3(0,-1.0f,0));
				body->setSleepingThresholds(0.2f,0.2f);
				body->setUserPointer(this);
			}
		}

		/* Add faces for rendering */
		for (int i = 0; i < rows-1; i++) {
			for (int j = 0; j < columns-1; j++) {
				add_face(Points[i][j], Points[i+1][j], Points[i][j+1]);
				add_face(Points[i][j+1], Points[i+1][j], Points[i+1][j+1]);
			}
		}

		MaterialSpecular = ClothSpecular;

		MaterialDiffuse = ClothDiffuse;

		MaterialAmbient = ClothAmbient;

		MaterialShininess = ClothShininess;

}

btPoint2PointConstraint *Cloth::Pin(int row, int col, btRigidBody *object, btVector3 *Pivot) {
	btPoint2PointConstraint *constraint = new btPoint2PointConstraint(*(object),*RigidBody[row][col],*Pivot, btVector3(0,0,0));
	constraint->setParam(BT_CONSTRAINT_STOP_CFM, 0);
	constraint->setParam(BT_CONSTRAINT_STOP_ERP, 0.7f);
	Physics::DynamicsWorld->addConstraint(constraint, true);
	Constraints.push_back(constraint);
	return constraint;
}

void Cloth::Unpin(btPoint2PointConstraint * handle){
	Constraints.remove(handle);
	Physics::DynamicsWorld->removeConstraint(handle);
	delete handle;
}

void Cloth::SimulationStep() {
	float diagonal = segmentLength * sqrtf(2);

	// Synchronize mesh points and collision objects
#pragma omp parallel for
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
			btVector3 relVel = (-RigidBody[i-1][j]->getLinearVelocity() + RigidBody[i][j]->getLinearVelocity())/2;
			btVector3 dampForce = -dir * relVel.dot(dir) * damping;
			RigidBody[i][j]->applyForce(dir * (dist - segmentLength) * stretch + dampForce, btVector3(0,0,0));
		}
		// Stretch Bottom
		if (i < rows-1) {
			float dist = (RigidBody[i+1][j]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i+1][j]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			btVector3 relVel = (-RigidBody[i+1][j]->getLinearVelocity() + RigidBody[i][j]->getLinearVelocity())/2;
			btVector3 dampForce = -dir * relVel.dot(dir) * damping;
			RigidBody[i][j]->applyForce(dir * (dist - segmentLength) * stretch + dampForce, btVector3(0,0,0));
		}
		// Stretch Left
		if (j > 0) {
			float dist = (RigidBody[i][j-1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i][j-1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			btVector3 relVel = (-RigidBody[i][j-1]->getLinearVelocity() + RigidBody[i][j]->getLinearVelocity())/2;
			btVector3 dampForce = -dir * relVel.dot(dir) * damping;
			RigidBody[i][j]->applyForce(dir * (dist - segmentLength) * stretch + dampForce, btVector3(0,0,0));
		}
		// Stretch Right
		if (j < columns-1) {
			float dist = (RigidBody[i][j+1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i][j+1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			btVector3 relVel = (-RigidBody[i][j+1]->getLinearVelocity() + RigidBody[i][j]->getLinearVelocity())/2;
			btVector3 dampForce = -dir * relVel.dot(dir) * damping;
			RigidBody[i][j]->applyForce(dir * (dist - segmentLength) * stretch + dampForce, btVector3(0,0,0));
		}
		// Stretch Top Right
		if (i > 0 && j < columns-1) {
			float dist = (RigidBody[i-1][j+1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i-1][j+1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			btVector3 relVel = (-RigidBody[i-1][j+1]->getLinearVelocity() + RigidBody[i][j]->getLinearVelocity())/2;
			btVector3 dampForce = -dir * relVel.dot(dir) * damping;
			RigidBody[i][j]->applyForce(dir * (dist - diagonal) * stretch + dampForce, btVector3(0,0,0));
		}
		// Stretch Bottom Right
		if (i < rows-1 && j < columns-1) {
			float dist = (RigidBody[i+1][j+1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i+1][j+1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			btVector3 relVel = (-RigidBody[i+1][j+1]->getLinearVelocity() + RigidBody[i][j]->getLinearVelocity())/2;
			btVector3 dampForce = -dir * relVel.dot(dir) * damping;
			RigidBody[i][j]->applyForce(dir * (dist - diagonal) * stretch + dampForce, btVector3(0,0,0));
		}
		// Stretch Top Left
		if (i > 0 && j > 0) {
			float dist = (RigidBody[i-1][j-1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i-1][j-1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			btVector3 relVel = (-RigidBody[i-1][j-1]->getLinearVelocity() + RigidBody[i][j]->getLinearVelocity())/2;
			btVector3 dampForce = -dir * relVel.dot(dir) * damping;
			RigidBody[i][j]->applyForce(dir * (dist - diagonal) * stretch + dampForce, btVector3(0,0,0));
		}
		// Stretch Bottom Left
		if (i < rows-1 && j > 0) {
			float dist = (RigidBody[i+1][j-1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i+1][j-1]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			btVector3 relVel = (-RigidBody[i+1][j-1]->getLinearVelocity() + RigidBody[i][j]->getLinearVelocity())/2;
			btVector3 dampForce = -dir * relVel.dot(dir) * damping;
			RigidBody[i][j]->applyForce(dir * (dist - diagonal) * stretch + dampForce, btVector3(0,0,0));
		}
		// Bend Top
		if (i > 1) {
			float dist = (RigidBody[i-2][j]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i-2][j]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			btVector3 relVel = (-RigidBody[i-2][j]->getLinearVelocity() + RigidBody[i][j]->getLinearVelocity())/2;
			btVector3 dampForce = -dir * relVel.dot(dir) * damping;
			RigidBody[i][j]->applyForce(dir * (dist - 2.0f*segmentLength) * bend + dampForce, btVector3(0,0,0));
		}
		// Bend Bottom
		if (i < rows-2) {
			float dist = (RigidBody[i+2][j]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i+2][j]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			btVector3 relVel = (-RigidBody[i+2][j]->getLinearVelocity() + RigidBody[i][j]->getLinearVelocity())/2;
			btVector3 dampForce = -dir * relVel.dot(dir) * damping;
			RigidBody[i][j]->applyForce(dir * (dist - 2.0f*segmentLength) * bend + dampForce, btVector3(0,0,0));
		}
		// Bend Left
		if (j > 1) {
			float dist = (RigidBody[i][j-2]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i][j-2]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			btVector3 relVel = (-RigidBody[i][j-2]->getLinearVelocity() + RigidBody[i][j]->getLinearVelocity())/2;
			btVector3 dampForce = -dir * relVel.dot(dir) * damping;
			RigidBody[i][j]->applyForce(dir * (dist - 2.0f*segmentLength) * bend + dampForce, btVector3(0,0,0));
		}
		// Bend Right
		if (j < columns-2) {
			float dist = (RigidBody[i][j+2]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).length();
			btVector3 dir = (RigidBody[i][j+2]->getCenterOfMassPosition()-RigidBody[i][j]->getCenterOfMassPosition()).normalized();
			btVector3 relVel = (-RigidBody[i][j+2]->getLinearVelocity() + RigidBody[i][j]->getLinearVelocity())/2;
			btVector3 dampForce = -dir * relVel.dot(dir) * damping;
			RigidBody[i][j]->applyForce(dir * (dist - 2.0f*segmentLength) * bend + dampForce, btVector3(0,0,0));
		}
	}

	BVEC3F Wind = wind * sinf(PolyMesh::Time/1000.0f);

#pragma omp parallel for
	for (int k = 0; k < (rows-1) * (columns-1); k++) {
		int i = k / (columns-1);
		int j = k % (columns-1);
		// Upper Triangle
		btVector3 Normal = (RigidBody[i][j]->getCenterOfMassPosition()-RigidBody[i][j+1]->getCenterOfMassPosition()).cross
			(RigidBody[i+1][j]->getCenterOfMassPosition() - RigidBody[i][j]->getCenterOfMassPosition()).normalized();
		btVector3 PlaneVelocity = Normal * (RigidBody[i][j]->getLinearVelocity() + RigidBody[i+1][j]->getLinearVelocity() + RigidBody[i][j+1]->getLinearVelocity()).dot(Normal);
		RigidBody[i][j]->applyForce(-PlaneVelocity * drag + Wind,btVector3(0,0,0));
		RigidBody[i+1][j]->applyForce(-PlaneVelocity * drag + Wind,btVector3(0,0,0));
		RigidBody[i][j+1]->applyForce(-PlaneVelocity * drag + Wind,btVector3(0,0,0));
		// Lower Triangle
		Normal = (RigidBody[i+1][j]->getCenterOfMassPosition()-RigidBody[i][j+1]->getCenterOfMassPosition()).cross
			(RigidBody[i+1][j+1]->getCenterOfMassPosition() - RigidBody[i+1][j]->getCenterOfMassPosition()).normalized();
		PlaneVelocity =  Normal * (RigidBody[i+1][j+1]->getLinearVelocity() + RigidBody[i+1][j]->getLinearVelocity() + RigidBody[i][j+1]->getLinearVelocity()).dot(Normal);
		RigidBody[i+1][j+1]->applyForce(-PlaneVelocity * drag + Wind,btVector3(0,0,0));
		RigidBody[i+1][j]->applyForce(-PlaneVelocity * drag + Wind,btVector3(0,0,0));
		RigidBody[i][j+1]->applyForce(-PlaneVelocity * drag + Wind,btVector3(0,0,0));
	}

	GenerateNormals();
}

Cloth::~Cloth() {
	PolyMesh::Meshes.remove(this);
	for (std::list<btPoint2PointConstraint *>::iterator i = Constraints.begin(); i != Constraints.end(); i++) {
		Physics::DynamicsWorld->removeConstraint(*i);
	}
	
	Constraints.clear();

	for (unsigned int i = 0; i < RigidBody.size(); i++) {
		for(unsigned int j = 0; j < RigidBody[i].size(); j++) {
			Physics::DynamicsWorld->removeRigidBody(RigidBody[i][j]);
			delete RigidBody[i][j]->getCollisionShape();
			delete (RigidBody[i][j]);
		}
	}
}