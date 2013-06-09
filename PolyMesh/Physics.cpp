#include <PolyMesh\Physics.h>

btVector3 Physics::Gravity = btVector3(0,-20.0f,0);
btDefaultCollisionConfiguration* Physics::CollisionConfiguration;
btCollisionDispatcher *Physics::Dispatcher;
btBroadphaseInterface *Physics::OverlappingPairCache;
btSequentialImpulseConstraintSolver *Physics::Solver;
btDiscreteDynamicsWorld *Physics::DynamicsWorld;
btAlignedObjectArray<btCollisionShape*> Physics::CollisionShapes;

void Physics::InitializePhysics()
{
	CollisionConfiguration = new btDefaultCollisionConfiguration();
	Dispatcher = new btCollisionDispatcher(CollisionConfiguration);
	OverlappingPairCache = new btDbvtBroadphase();
	Solver = new btSequentialImpulseConstraintSolver;
	DynamicsWorld = new btDiscreteDynamicsWorld(Dispatcher, OverlappingPairCache, Solver, CollisionConfiguration);
	DynamicsWorld->getSolverInfo().m_solverMode |=SOLVER_USE_2_FRICTION_DIRECTIONS|SOLVER_RANDMIZE_ORDER;
	DynamicsWorld->getDispatchInfo().m_useContinuous=true;
	DynamicsWorld->setGravity(Gravity);
	OverlappingPairCache->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
}