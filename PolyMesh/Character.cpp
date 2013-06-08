#include <PolyMesh\Character.h>

Character::Character() {
	__super::character = true;
}

Character *Character::GenerateCharacter() {
	btConvexHullShape *ConvexShape = new btConvexHullShape();
	for(VertexIter v_it = vertices_begin(); v_it != vertices_end(); ++v_it)
	{
		ConvexShape->addPoint(btVector3(point(v_it.handle())[0],point(v_it.handle())[1],point(v_it.handle())[2]));
	}
	btCollisionShape *ConvexHull = ConvexShape;

	btPairCachingGhostObject * GO = new btPairCachingGhostObject();

	btTransform transform;
	transform.setIdentity();
	GO->setWorldTransform(transform);
	GO->setCollisionShape(ConvexHull);
	GO->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	RigidBody = new btKinematicCharacterController(GO, ConvexShape, 0.35);
	Physics::DynamicsWorld->addCollisionObject(GO, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	Physics::DynamicsWorld->addAction(RigidBody);
	
	btVector3 localInertia(0.0f,0.0f,0.0f);
	btScalar m(0.0f);
	transform = RigidBody->getGhostObject()->getWorldTransform();
	btDefaultMotionState *myMotionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(m,myMotionState,ConvexHull,localInertia);
	Dummy = new btRigidBody(rbInfo);
	Dummy->setContactProcessingThreshold(0.05f);
	Physics::DynamicsWorld->addRigidBody(Dummy, btBroadphaseProxy::DebrisFilter, btBroadphaseProxy::DebrisFilter);
	Dummy->setUserPointer(this);
	return this;
}

Character *Character::SyncDummy() {
	Dummy->setWorldTransform(RigidBody->getGhostObject()->getWorldTransform());

	return this;
}

btVector3 Character::GetPosition() {
	return RigidBody->getGhostObject()->getWorldTransform().getOrigin();
}