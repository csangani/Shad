#include <string>

#include <Spline/Spline.hpp>
#include <Spline/Quaternion.h>
#include <Spline/CatmullRom.h>

class AnimationRoutine
{
public:
	AnimationRoutine() : _CRSpline(Spline<CatmullRom>()), _QTSpline(Spline<Quaternion>()) {}
	AnimationRoutine(std::string filePath) : _CRSpline(Spline<CatmullRom>()), _QTSpline(Spline<Quaternion>()) { LoadAni(filePath); }
	AnimationRoutine(Spline<CatmullRom> CRSpline) : _CRSpline(CRSpline), _QTSpline(Spline<Quaternion>()) {}
	AnimationRoutine(Spline<Quaternion> QTSpline) : _CRSpline(Spline<CatmullRom>()), _QTSpline(QTSpline) {}
	AnimationRoutine(Spline<CatmullRom> CRSpline, Spline<Quaternion> QTSpline) : _CRSpline(CRSpline), _QTSpline(QTSpline) {}

	AnimationRoutine * AddTranslationalKeyFrame(float, float, float, uint64_t);
	AnimationRoutine * AddRotationalKeyFrame(float, float, float, float, uint64_t);
	AnimationRoutine * LoadAni(std::string);

	CatmullRom T(uint64_t);
	Quaternion R(uint64_t);

	uint64_t TDuration();
	uint64_t RDuration();

private:
	Spline<CatmullRom> _CRSpline;
	Spline<Quaternion> _QTSpline;
};