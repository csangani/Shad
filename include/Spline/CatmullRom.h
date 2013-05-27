#include <Spline/stdint.h>
#include <map>

#include <OpenMesh/Core/Geometry/VectorT.hh>

#ifndef CATMULLROM_H
#define CATMULLROM_H

class CatmullRom : public OpenMesh::Vec3f
{
public:
	CatmullRom() {}
	CatmullRom(OpenMesh::Vec3f &Vec) : OpenMesh::Vec3f(Vec) {}

	CatmullRom static Interpolate(std::map<uint64_t, CatmullRom> CPL, uint64_t ts);
	
	static uint64_t begin(std::map<uint64_t, CatmullRom>);
	static uint64_t end(std::map<uint64_t, CatmullRom>);
};

#endif