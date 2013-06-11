#include <PolyMesh\Character.h>

Character::Character() : AnimationTime(0) {
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
	Physics::DynamicsWorld->addRigidBody(Dummy, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::DebrisFilter);
	Dummy->setUserPointer(this);
	return this;
}

Character::~Character() {
	Physics::DynamicsWorld->removeCollisionObject(RigidBody->getGhostObject());
	Physics::DynamicsWorld->removeAction(RigidBody);
	Physics::DynamicsWorld->removeRigidBody(Dummy);
	Physics::DynamicsWorld->removeRigidBody(Arms->RigidBody);
	delete Dummy->getCollisionShape();
	delete Dummy;
	delete Arms->RigidBody->getCollisionShape();
	delete Arms->RigidBody;
	delete Arms;
	delete RigidBody->getGhostObject();
	delete RigidBody;
}

Character *Character::SyncDummy() {
	Dummy->setWorldTransform(RigidBody->getGhostObject()->getWorldTransform());
	btTransform armTranslation;
	armTranslation.setIdentity();
	armTranslation.setOrigin(btVector3(0,-0.08f,0));
	btTransform armRotation;
	armRotation.setIdentity();
	armRotation.setRotation(btQuaternion(BVEC3F(0,1,0),RADIANS(180)));
	Arms->RigidBody->setWorldTransform(armTranslation * RigidBody->getGhostObject()->getWorldTransform() * armRotation);
	return this;
}

btVector3 Character::GetPosition() {
	return RigidBody->getGhostObject()->getWorldTransform().getOrigin();
}

Character *Character::GenerateArms(std::string filePath) {
	Arms = (new PolyMesh())->LoadObj(filePath)->GenerateRigidBody()->Scale(OpenMesh::Vec3f(0.7f, 0.7f, 0.7f));

		/* Enable physics */
	btConvexHullShape *ConvexShape = new btConvexHullShape();
	for(VertexIter v_it = vertices_begin(); v_it != vertices_end(); ++v_it)
	{
		ConvexShape->addPoint(btVector3(point(v_it.handle())[0],point(v_it.handle())[1],point(v_it.handle())[2]));
	}
	btCollisionShape *ConvexHull = ConvexShape;
	btVector3 localInertia(0.0f,0.0f,0.0f);
	btScalar m(0);
	bool isDynamic = (m != 0.0f);
	if (isDynamic)
		ConvexHull->calculateLocalInertia(m,localInertia);
	btTransform transform;
	transform.setIdentity();
	btDefaultMotionState *myMotionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(m,myMotionState,ConvexHull,localInertia);
	Arms->RigidBody = new btRigidBody(rbInfo);
	Arms->RigidBody->setContactProcessingThreshold(0.0f);
	Physics::DynamicsWorld->addRigidBody(Arms->RigidBody, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::DebrisFilter);
	Arms->RigidBody->setUserPointer(this);
	return this;
}