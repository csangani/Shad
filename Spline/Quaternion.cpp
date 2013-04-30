#include <Spline/Quaternion.h>
#include <math.h>

// Conjugate
Quaternion Quaternion::operator--() const {
	return Quaternion(_w, -_x, -_y, -_z);
}

// Negation
Quaternion Quaternion::operator-() const {
	return Quaternion(-_w, -_x, -_y, -_z);
}

// Dot product
float Quaternion::operator*(const Quaternion& q) const {
	return q._w*_w + q._x*_x + q._y*_y + q._z*_z;
}

// Cross product
Quaternion Quaternion::operator%(const Quaternion& q) const {
	return Quaternion(
		_w*q._w - _x*q._x - _y*q._y - _z*q._z,
		_w*q._x + _x*q._w + _y*q._z - _z*q._y,
		_w*q._y - _x*q._z + _y*q._w + _z*q._x,
		_w*q._z + _x*q._y - _y*q._x + _z*q._w);
}

// Rotate
Quaternion Quaternion::Rotate(Quaternion q) const {
	return q % (*this) % --q;
}

// Scalar multiplication
Quaternion Quaternion::operator*(const float& scalar) const {
	return Quaternion(_w*scalar, _x*scalar, _y*scalar, _z*scalar);
}

// Vector addition
Quaternion Quaternion::operator+(const Quaternion& other) const {
	return Quaternion(_w + other._w, _x + other._x, _y + other._y, _z + other._z);
}

// Vector subtraction
Quaternion Quaternion::operator-(const Quaternion& other) const {
	return Quaternion(_w - other._w, _x - other._x, _y - other._y, _z - other._z);
}

// Assignment
void Quaternion::operator=(const Quaternion& other) {
	_w = other._w;
	_x = other._x;
	_y = other._y;
	_z = other._z;
}

// Control Point List header
uint64_t Quaternion::begin(std::map<uint64_t, Quaternion> CPL) {
	if (CPL.size() > 0)
		return CPL.begin()->first;
	else
		return 0;
}

// Control Point List footer
uint64_t Quaternion::end(std::map<uint64_t, Quaternion> CPL) {
	if (CPL.size() > 0)
		return CPL.rbegin()->first;
	else
		return 0;
}

float Quaternion::w() const {
	return _w;
}

float Quaternion::x() const {
	return _x;
}

float Quaternion::y() const {
	return _y;
}

float Quaternion::z() const {
	return _z;
}

// Magnitude
float Quaternion::mod() const
{
	return sqrtf(_w*_w + _x*_x + _y*_y + _z*_z);
}

// SLERP
Quaternion Quaternion::Interpolate(std::map<uint64_t, Quaternion> CPL, uint64_t ts) {
	if (CPL.size() < 2)
		return Quaternion();

	std::map<uint64_t, Quaternion>::iterator iter = CPL.begin();
	if (ts >= CPL.rbegin()->first)
		return CPL.rbegin()->second;
	if (ts <= CPL.begin()->first)
		return CPL.begin()->second;
	while (iter->first < ts)
		iter++;
	iter--;

	Quaternion q1 = iter->second; uint64_t ts1 = iter->first; iter++;
	Quaternion q2 = iter->second; uint64_t ts2 = iter->first;

	float u = ((float)ts - ts1)/(ts2 - ts1);

	if (q1 * q2 < 0)
		q1 = -q1;

	float a = acosf((q1 * q2) / (q1.mod() * q2.mod()));
	if (a > 0.001f)
		return q1 * (sinf((1-u)*a)/sinf(a)) + q2 * (sinf(u*a)/sinf(a));
	else
		return q1 * (1-u) + q2 * u;
}