#include <PolyMesh/ParticleCloth.h>

ParticleCloth::ParticleCloth (int length, int width, float segmentLength, BVEC3F& emitStart, BVEC3F& emitEnd, BVEC3F& lengthVector, float gravity, uint64_t life, Character* reference) : startRow(0), life(life), elapsed(0), width(width), length(length), reference(reference), emitStart(emitStart), emitEnd(emitEnd), lengthVector(lengthVector), segmentLength(segmentLength) {
	particleCloth = true;
	request_vertex_texcoords2D();
	for (int i = 0; i < length; i++) {
		RigidBody.push_back(std::vector<btRigidBody *>());
		Points.push_back(std::vector<VertexHandle>());
		for (int j = 0; j < width; j++) {
			Points[i].push_back(add_vertex(OVECB(((emitEnd-emitStart).normalized() * (float)j + lengthVector.normalized() * (float)i) * segmentLength + emitStart + reference->RigidBody->getGhostObject()->getWorldTransform().getOrigin())));
			if ( j < width - 1) {
				Points[i].push_back(add_vertex(OVEC3F(0,0,0)));
				Points[i].push_back(add_vertex(OVEC3F(0,0,0)));
				Points[i].push_back(add_vertex(OVEC3F(0,0,0)));
				Points[i].push_back(add_vertex(OVEC3F(0,0,0)));
			}
			set_texcoord2D(Points[i][j/5],OpenMesh::Vec2f(1.0f/(length-1)*i,(1.0f/(width-1)*j)));
			btScalar mass(0.1f);
			btVector3 localInertia(0,0,0);
			btCollisionShape *shape = new btSphereShape(0.001f);
			shape->calculateLocalInertia(mass, localInertia);
			btTransform transform;
			transform.setIdentity();
			transform.setOrigin(((emitEnd-emitStart).normalized() * (float)j + lengthVector.normalized() * (float)i) * segmentLength + emitStart + reference->RigidBody->getGhostObject()->getWorldTransform().getOrigin());
			btDefaultMotionState *state = new btDefaultMotionState(transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, state, shape, localInertia);
			RigidBody[i].push_back(new btRigidBody(rbInfo));
			Physics::DynamicsWorld->addRigidBody(RigidBody[i][j], btBroadphaseProxy::DebrisFilter, btBroadphaseProxy::DebrisFilter | btBroadphaseProxy::StaticFilter);
			RigidBody[i][j]->setGravity(BVEC3F(0,-gravity,0));
			RigidBody[i][j]->setActivationState(DISABLE_DEACTIVATION);
			RigidBody[i][j]->setUserPointer(this);
			RigidBody[i][j]->setContactProcessingThreshold(0.0f);
			RigidBody[i][j]->setSleepingThresholds(0.2f,0.2f);
			RigidBody[i][j]->setDamping(0.3f, 0.3f);
		}
	}

	/* Add interpolated points */

	/* Add faces for rendering */
	for (int i = 0; i < length-1; i++) {
		for (int j = 0; j < 5*width-5; j++) {
			add_face(Points[i][j], Points[i+1][j], Points[i][j+1]);
			add_face(Points[i][j+1], Points[i+1][j], Points[i+1][j+1]);
		}
	}
}

void ParticleCloth::SimulationStep() {
	elapsed += (uint64_t)FRAME_PERIOD;
	if (elapsed > life) {
		startRow = (startRow + length - 1) % length;
		elapsed = 0;
		for (int i = 0; i < width; i++) {
			btTransform transform;
			transform.setIdentity();
			BVEC3F RelPosition = ((emitEnd-emitStart).normalized() * (float)i ) * segmentLength + emitStart;
			BVEC3F FinPosition = RelPosition.rotate(reference->RigidBody->getGhostObject()->getWorldTransform().getRotation().getAxis(),reference->RigidBody->getGhostObject()->getWorldTransform().getRotation().getAngle()) +
				reference->RigidBody->getGhostObject()->getWorldTransform().getOrigin();
			transform.setOrigin(FinPosition);
			transform.setRotation(reference->RigidBody->getGhostObject()->getWorldTransform().getRotation());
			BVEC3F velocity(cosf(PolyMesh::Time/100.0f)*(0.7f+0.3f*sinf(PolyMesh::Time/200.0f)),-3+0.25f*sinf(PolyMesh::Time/100.0f)*sinf(cosf(i/2*i/2*i)*0.5f),10);
			velocity = velocity.rotate(reference->RigidBody->getGhostObject()->getWorldTransform().getRotation().getAxis(),reference->RigidBody->getGhostObject()->getWorldTransform().getRotation().getAngle());
			RigidBody[startRow][i]->setLinearVelocity(velocity); 
			RigidBody[startRow][i]->setCenterOfMassTransform(transform);
		}
	}

	for (int _i = 0; _i < length; _i++)
	{
		int i = (_i + startRow) % length;
		for (int j = 0; j < width; j++) {
			set_point(Points[_i][5*j], OVECB(RigidBody[i][j]->getCenterOfMassPosition()));
			if (j < width - 1) {
				BVEC3F Inter(0,0,0);
				Inter.setInterpolate3(RigidBody[i][j]->getCenterOfMassPosition(), RigidBody[i][j+1]->getCenterOfMassPosition(), 0.2f);
				set_point(Points[_i][5*j+1], OVECB(Inter));
				Inter.setInterpolate3(RigidBody[i][j]->getCenterOfMassPosition(), RigidBody[i][j+1]->getCenterOfMassPosition(), 0.4f);
				set_point(Points[_i][5*j+2], OVECB(Inter));
				Inter.setInterpolate3(RigidBody[i][j]->getCenterOfMassPosition(), RigidBody[i][j+1]->getCenterOfMassPosition(), 0.6f);
				set_point(Points[_i][5*j+3], OVECB(Inter));
				Inter.setInterpolate3(RigidBody[i][j]->getCenterOfMassPosition(), RigidBody[i][j+1]->getCenterOfMassPosition(), 0.8f);
				set_point(Points[_i][5*j+4], OVECB(Inter));
			}
		}
	}

	GenerateNormals();
}