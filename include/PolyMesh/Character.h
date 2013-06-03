#include <PolyMesh/PolyMesh.h>

class Character : public PolyMesh {
public:
	btKinematicCharacterController *RigidBody;

	Character();

	Character *GenerateCharacter();
};