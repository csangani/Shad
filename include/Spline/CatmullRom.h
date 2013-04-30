#include <cstdint>
#include <map>

#include <OpenMesh/Core/Geometry/VectorT.hh>

#ifndef CATMULLROM_H
#define CATMULLROM_H

class CatmullRom
{
public:
	CatmullRom() : _point(OpenMesh::Vec3f(0.0f, 0.0f, 0.0f)) {}

	CatmullRom(OpenMesh::Vec3f point) : _point(point) {}

	CatmullRom(float x, float y, float z) : _point(OpenMesh::Vec3f(x, y, z)) {}

	OpenMesh::Vec3f Vec3f();

	CatmullRom static Interpolate(std::map<uint64_t, CatmullRom> CPL, uint64_t ts);
	
	static uint64_t begin(std::map<uint64_t, CatmullRom>);
	static uint64_t end(std::map<uint64_t, CatmullRom>);

private:
	OpenMesh::Vec3f _point;
};

#endif