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
	return this;
}