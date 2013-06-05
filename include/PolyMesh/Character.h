#include <PolyMesh/PolyMesh.h>

class Character : public PolyMesh {
public:
	btKinematicCharacterController *RigidBody;
	btRigidBody *Dummy;

	Character();

	Character *GenerateCharacter();
	
	Character *SyncDummy();
};