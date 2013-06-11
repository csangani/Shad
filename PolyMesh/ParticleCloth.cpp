#include <PolyMesh/ParticleCloth.h>

ParticleCloth::ParticleCloth (int length, int width, float segmentLength, BVEC3F& emitStart, BVEC3F& emitEnd, BVEC3F& lengthVector, float gravity, uint64_t life, Character* reference) : startRow(0), life(life), elapsed(0), width(width), length(length), reference(reference), emitStart(emitStart), emitEnd(emitEnd), lengthVector(lengthVector), segmentLength(segmentLength) {
	particleCloth = true;
	request_vertex_texcoords2D();
	for (int i = 0; i < length; i++) {
		RigidBody.push_back(std::vector<btRigidBody *>());
		Points.push_back(std::vector<VertexHandle>());
		for (int j = 0; j < width; j++) {
			Points[i].push_back(add_vertex(OVECB(((emitEnd-emitStart).normalized() * (float)j + lengthVector.normalized() * (float)i) * segmentLength + emitStart + reference->RigidBody->getGhostObject()->getWorldTransform().getOrigin())));
			set_texcoord2D(Points[i][j],OpenMesh::Vec2f(1.0f/(length-1)*i,(1.0f/(width-1)*j)));
			btScalar mass(0.1f);
			btVector3 localInertia(0,0,0);
			btCollisionShape *shape = new btSphereShape(0.01f);
			shape->calculateLocalInertia(mass, localInertia);
			btTransform transform;
			transform.setIdentity();
			transform.setOrigin(((emitEnd-emitStart).normalized() * (float)j + lengthVector.normalized() * (float)i) * segmentLength + emitStart + reference->RigidBody->getGhostObject()->getWorldTransform().getOrigin());
			btDefaultMotionState *state = new btDefaultMotionState(transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, state, shape, localInertia);
			RigidBody[i].push_back(new btRigidBody(rbInfo));
			Physics::DynamicsWorld->addRigidBody(RigidBody[i][j], btBroadphaseProxy::DebrisFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::CharacterFilter);
			RigidBody[i][j]->setGravity(BVEC3F(0,-gravity,0));
			RigidBody[i][j]->setActivationState(DISABLE_DEACTIVATION);
			RigidBody[i][j]->setUserPointer(this);
			RigidBody[i][j]->setContactProcessingThreshold(0.0f);
			RigidBody[i][j]->setFriction(0);
			RigidBody[i][j]->setDamping(0.1f, 0.1f);
		}
	}

	/* Add faces for rendering */
	for (int i = 0; i < length-1; i++) {
		for (int j = 0; j < width-1; j++) {
			add_face(Points[i][j], Points[i+1][j], Points[i][j+1]);
			add_face(Points[i][j+1], Points[i+1][j], Points[i+1][j+1]);
		}
	}
}

ParticleCloth::~ParticleCloth() {
	PolyMesh::Meshes.remove(this);
	for (unsigned int i = 0; i < RigidBody.size(); i++) {
		for(unsigned int j = 0; j < RigidBody[i].size(); j++) {
			Physics::DynamicsWorld->removeRigidBody(RigidBody[i][j]);
			delete RigidBody[i][j]->getCollisionShape();
			delete (RigidBody[i][j]);
		}
	}
}

void ParticleCloth::SimulationStep() {
	elapsed += (uint64_t)FRAME_PERIOD;
	if (elapsed > life) {
		startRow = (startRow + length - 1) % length;
		elapsed = 0;
#pragma omp parallel for
		for (int i = 0; i < width; i++) {
			btTransform transform;
			transform.setIdentity();
			BVEC3F RelPosition = ((emitEnd-emitStart).normalized() * (float)i ) * segmentLength + emitStart;
			BVEC3F FinPosition = RelPosition.rotate(reference->RigidBody->getGhostObject()->getWorldTransform().getRotation().getAxis(),reference->RigidBody->getGhostObject()->getWorldTransform().getRotation().getAngle()) +
				reference->RigidBody->getGhostObject()->getWorldTransform().getOrigin();
			transform.setOrigin(FinPosition);
			transform.setRotation(reference->RigidBody->getGhostObject()->getWorldTransform().getRotation());
			BVEC3F velocity(cosf(PolyMesh::Time/100.0f)*(0.7f+0.3f*sinf(PolyMesh::Time/200.0f)),-3+0.25f*sinf(PolyMesh::Time/100.0f)*sinf(cosf(i/2.0f*i/2.0f*i)*0.5f),2);
			velocity = velocity.rotate(reference->RigidBody->getGhostObject()->getWorldTransform().getRotation().getAxis(),reference->RigidBody->getGhostObject()->getWorldTransform().getRotation().getAngle());
			RigidBody[startRow][i]->setLinearVelocity(velocity); 
			RigidBody[startRow][i]->setCenterOfMassTransform(transform);
		}
	}

#pragma omp parallel for
	for (int _i = 0; _i < length; _i++)
	{
		int i = (_i + startRow) % length;
		for (int j = 0; j < width; j++) {
			set_point(Points[_i][j], OVECB(RigidBody[i][j]->getCenterOfMassPosition()));
		}
	}

	GenerateNormals();
}