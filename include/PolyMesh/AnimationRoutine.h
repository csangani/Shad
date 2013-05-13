#include <string>

#include <Spline/Spline.hpp>
#include <Spline/Quaternion.h>
#include <Spline/CatmullRom.h>

class AnimationRoutine
{
public:
	AnimationRoutine() : _CRSpline(Spline<CatmullRom>()), _QTSpline(Spline<Quaternion>()), _PrevT(CatmullRom(OpenMesh::Vec3f(0.0f, 0.0f, 0.0f))), _PrevR(Quaternion(0.0f, 0.0f, 0.0f, 0.0f)) {}
	AnimationRoutine(std::string filePath) : _CRSpline(Spline<CatmullRom>()), _QTSpline(Spline<Quaternion>()), _PrevT(CatmullRom(OpenMesh::Vec3f(0.0f, 0.0f, 0.0f))), _PrevR(Quaternion(0.0f, 0.0f, 0.0f, 0.0f)) { LoadAni(filePath); }
	AnimationRoutine(Spline<CatmullRom> CRSpline) : _CRSpline(CRSpline), _QTSpline(Spline<Quaternion>()), _PrevT(CatmullRom(OpenMesh::Vec3f(0.0f, 0.0f, 0.0f))), _PrevR(Quaternion(0.0f, 0.0f, 0.0f, 0.0f)) {}
	AnimationRoutine(Spline<Quaternion> QTSpline) : _CRSpline(Spline<CatmullRom>()), _QTSpline(QTSpline), _PrevT(CatmullRom(OpenMesh::Vec3f(0.0f, 0.0f, 0.0f))), _PrevR(Quaternion(0.0f, 0.0f, 0.0f, 0.0f)) {}
	AnimationRoutine(Spline<CatmullRom> CRSpline, Spline<Quaternion> QTSpline) : _CRSpline(CRSpline), _QTSpline(QTSpline), _PrevT(CatmullRom(OpenMesh::Vec3f(0.0f, 0.0f, 0.0f))), _PrevR(Quaternion(0.0f, 0.0f, 0.0f, 0.0f)) {}

	AnimationRoutine * AddTranslationalKeyFrame(float, float, float, uint64_t);
	AnimationRoutine * AddRotationalKeyFrame(float, float, float, float, uint64_t);
	AnimationRoutine * LoadAni(std::string);

	CatmullRom T(uint64_t);
	Quaternion R(uint64_t);

	CatmullRom TD(uint64_t);
	Quaternion RD(uint64_t);

	uint64_t TDuration();
	uint64_t RDuration();

private:
	Spline<CatmullRom> _CRSpline;
	Spline<Quaternion> _QTSpline;

	CatmullRom _PrevT;
	Quaternion _PrevR;
};