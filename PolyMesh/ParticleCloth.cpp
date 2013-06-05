#include <PolyMesh/ParticleCloth.h>

ParticleCloth::ParticleCloth (int length, int width, float segmentLength, BVEC3F& emitStart, BVEC3F& emitEnd, BVEC3F& lengthVector, float gravity, uint64_t life, Character* reference) : startRow(0), life(life), elapsed(0), width(width), length(length), reference(reference), emitStart(emitStart), emitEnd(emitEnd), lengthVector(lengthVector), segmentLength(segmentLength) {
	cloth = true;
	for (int i = 0; i < length; i++) {
		RigidBody.push_back(std::vector<btRigidBody *>());
		Points.push_back(std::vector<VertexHandle>());
		for (int j = 0; j < width; j++) {
			Points[i].push_back(add_vertex(OVECB(((emitEnd-emitStart).normalized() * (float)j + lengthVector.normalized() * (float)i) * segmentLength + reference->RigidBody->getGhostObject()->getWorldTransform().getOrigin())));
			btScalar mass(0.001f);
			btVector3 localInertia(0,0,0);
			btConvexShape *shape = new btSphereShape(0.001f);
			shape->calculateLocalInertia(mass, localInertia);
			btTransform transform;
			transform.setOrigin(((emitEnd-emitStart).normalized() * (float)j + lengthVector.normalized() * (float)i) * segmentLength + reference->RigidBody->getGhostObject()->getWorldTransform().getOrigin());
			btMotionState *state = new btDefaultMotionState(transform);
			RigidBody[i].push_back(new btRigidBody(mass, state, shape, localInertia));
			RigidBody[i][j]->setGravity(BVEC3F(0,-gravity,0));
			RigidBody[i][j]->setActivationState(DISABLE_DEACTIVATION);
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

void ParticleCloth::StepSimulation() {
	elapsed += (uint64_t)FRAME_PERIOD;
	if (elapsed > life) {
		startRow = (startRow + 1) % length;
		elapsed = 0;
		for (int i = 0; i < width; i++) {
			RigidBody[length-1][i]->getWorldTransform().setOrigin(((emitEnd-emitStart).normalized() * (float)i + reference->Dummy->getLinearVelocity().normalized()) * segmentLength + reference->RigidBody->getGhostObject()->getWorldTransform().getOrigin() +BVEC3F(0,sinf(Character::Time * 0.001f),0));
		}
	}

	for (int _i = 0; _i < length; _i++)
	{
		int i = (_i + startRow) % length;
		for (int j = 0; j < width; j++) {
			set_point(Points[_i][j], OpenMesh::Vec3f(
				RigidBody[i][j]->getCenterOfMassPosition().getX(),
				RigidBody[i][j]->getCenterOfMassPosition().getY(),
				RigidBody[i][j]->getCenterOfMassPosition().getZ()));
		}
	}
}