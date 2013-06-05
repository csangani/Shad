#include <Spline/stdint.h>
#include <map>

#include <OpenMesh/Core/Geometry/VectorT.hh>

#ifndef QUATERNION_H
#define QUATERNION_H

class Quaternion
{
public:
	Quaternion() : _w(1.0f), _x(0.0f), _y(0.0f), _z(0.0f){}
	Quaternion(float w, float x, float y, float z) : _w(w), _x(x), _y(y), _z(z){}
	
	Quaternion(float angle, OpenMesh::Vec3f axis);
	OpenMesh::Vec3f Quaternion::Rotate(const OpenMesh::Vec3f vec);

	Quaternion operator--() const;
	Quaternion operator-() const;
	float operator*(const Quaternion&) const;
	Quaternion operator%(const Quaternion&) const;
	Quaternion Rotate(Quaternion) const;
	Quaternion operator*(const float&) const;
	Quaternion operator+(const Quaternion&) const;
	Quaternion operator-(const Quaternion&) const;
	void operator=(const Quaternion&);

	static Quaternion Interpolate(std::map<uint64_t, Quaternion>, uint64_t);
	
	static uint64_t begin(std::map<uint64_t, Quaternion>);
	static uint64_t end(std::map<uint64_t, Quaternion>);

	float w() const;
	void w(float &);
	float x() const;
	void x(float &);
	float y() const;
	void y(float &);
	float z() const;
	void z(float &);
	float mod() const;

protected:
	float _w;
	float _x;
	float _y;
	float _z;
};

#endif