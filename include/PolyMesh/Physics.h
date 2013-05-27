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

#define BIT(X) (1<<X)
	static enum collisionFilters {
		Ghost = BIT(0),
		Static = BIT(1),
		Dynamic = BIT(2),
		Cloth = BIT(3),
		Camera = BIT(4)
	};
};