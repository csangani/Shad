#ifndef CHARACTER_H
#define CHARACTER_H

#include <PolyMesh/PolyMesh.h>

class Character : public PolyMesh {
public:
	btKinematicCharacterController *RigidBody;
	btRigidBody *Dummy;
	PolyMesh *Arms;

	uint64_t AnimationTime;

	Character();
	~Character();

	Character *GenerateCharacter();
	
	btVector3 GetPosition();
	
	Character *SyncDummy();
	Character *GenerateArms(std::string);
};

#endif