#ifndef PARTICLECLOTH_H
#define PARTICLECLOTH_H

#include <vector>

#include <PolyMesh/Character.h>

class ParticleCloth : public PolyMesh {
public:
	ParticleCloth (int length, int width, float segmentLength, BVEC3F& emitStart,
		BVEC3F& emitEnd, BVEC3F& lengthVector, float gravity, uint64_t life, Character* reference);
	~ParticleCloth();
	void SimulationStep();

private:
	int width, length;
	BVEC3F emitStart, emitEnd, lengthVector;
	float segmentLength;
	std::vector<std::vector<VertexHandle>> Points;
	std::vector<std::vector<btRigidBody *>> RigidBody;
	int startRow;
	uint64_t life, elapsed;
	Character *reference;
};

#endif