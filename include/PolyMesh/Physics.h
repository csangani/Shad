#include <btBulletDynamicsCommon.h>

class Physics
{
public:
	static btVector3 Gravity;
	static btDefaultCollisionConfiguration* CollisionConfiguration;
	static btCollisionDispatcher *Dispatcher;
	static btBroadphaseInterface *OverlappingPairCache;
	static btSequentialImpulseConstraintSolver *Solver;
	static btDiscreteDynamicsWorld *DynamicsWorld;
	static btAlignedObjectArray<btCollisionShape*> CollisionShapes;

	static void InitializePhysics();
};