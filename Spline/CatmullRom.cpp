
#include <map>

#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <Spline/CatmullRom.h>

CatmullRom CatmullRom::Interpolate(std::map<uint64_t, CatmullRom> CPL, uint64_t ts)
{
	if (CPL.size() < 4)
		return CatmullRom();
	std::map<uint64_t, CatmullRom>::iterator iter = CPL.begin();
	do {
		iter++;
	}
	while (iter->first < ts);

	iter--;
	iter--;

	CatmullRom t0 = iter->second; uint64_t ts0 = iter->first; iter++; if (iter == CPL.end()) return CPL.rbegin().operator++()->second;
	CatmullRom t1 = iter->second; uint64_t ts1 = iter->first; iter++; if (iter == CPL.end()) return CPL.rbegin().operator++()->second;
	CatmullRom t2 = iter->second; uint64_t ts2 = iter->first; iter++; if (iter == CPL.end()) return CPL.rbegin().operator++()->second;
	CatmullRom t3 = iter->second; uint64_t ts3 = iter->first;

	OpenMesh::Vec3f c0, c1, c2, c3;
	OpenMesh::Vec3f p0, p1, p2, p3;

	p0 = t0;
	p1 = t1;
	p2 = t2;
	p3 = t3;

	float u = ((float)ts - ts1)/(ts2 - ts1);

	c0 = p1;
	c1 = -p0/2.0f + p2/2.0f;
	c2 = p0 - p1*(5.0f/2.0f) + p2*2 - p3/2.0f;
	c3 = -p0/2.0f + p1*(3.0f/2.0f) - p2*(3.0f/2.0f) + p3/2.0f;

	return CatmullRom(c0 + c1*u + c2*powf(u,2) + c3*powf(u,3));
}

uint64_t CatmullRom::begin(std::map<uint64_t, CatmullRom> CPL) {
	if (CPL.size() > 0)
		return CPL.begin().operator++()->first;
	else
		return 0;
}

uint64_t CatmullRom::end(std::map<uint64_t, CatmullRom> CPL) {
	if (CPL.size() > 0)
		return CPL.rbegin().operator++()->first;
	else
		return 0;
}