#include <fstream>
#include <sstream>
#include <limits>

#include <Spline/Spline.hpp>
#include <Spline/Quaternion.h>
#include <Spline/CatmullRom.h>

#include <PolyMesh/AnimationRoutine.h>

AnimationRoutine * AnimationRoutine::AddTranslationalKeyFrame(float x, float y, float z, uint64_t timestamp)
{
	_CRSpline.AddControlPoint(CatmullRom(OpenMesh::Vec3f(x, y, z)), timestamp);
	return this;
}

AnimationRoutine * AnimationRoutine::AddRotationalKeyFrame(float w, float x, float y, float z, uint64_t timestamp)
{
	_QTSpline.AddControlPoint(Quaternion(2*(w*((float)M_PI))/360.0f, x, y, z), timestamp);
	return this;
}

AnimationRoutine * AnimationRoutine::LoadAni(std::string FilePath)
{
	std::ifstream file(FilePath.c_str());

	if (file.good())
	{
		std::stringstream buffer;

		buffer << file.rdbuf();

		std::string token;
		buffer >> token;

		while (buffer.good())
		{
			if (token.compare("#") == 0)
			{
				buffer.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			else if (token.compare("t") == 0)
			{
				float x, y, z;

				uint64_t timestamp;

				buffer >> x >> y >> z >> timestamp;

				_CRSpline.AddControlPoint(CatmullRom(OpenMesh::Vec3f(x, y, z)), timestamp);
			}
			else if (token.compare("r") == 0)
			{
				float w, x, y, z;

				uint64_t timestamp;

				buffer >> w >> x >> y >> z >> timestamp;

				_QTSpline.AddControlPoint(Quaternion(2*(w*((float)M_PI))/360.0f, x, y, z), timestamp);
			}
			else
			{
				buffer.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			buffer>>token;
		}
	}

	file.close();

	return this;
}

CatmullRom AnimationRoutine::T(uint64_t timestamp)
{
	_PrevT = _CRSpline(timestamp);
	return _PrevT;
}

Quaternion AnimationRoutine::R(uint64_t timestamp)
{
	_PrevR = _QTSpline(timestamp);
	return _PrevR;
}

CatmullRom AnimationRoutine::TD(uint64_t timestamp)
{
	CatmullRom Prev = _PrevT;
	_PrevT = _CRSpline(timestamp);
	return _PrevT - Prev;
}

Quaternion AnimationRoutine::RD(uint64_t timestamp)
{
	Quaternion Prev = _PrevR;
	_PrevR = _QTSpline(timestamp);
	return _PrevR - Prev;
}

uint64_t AnimationRoutine::TDuration()
{
	return _CRSpline.end() - _CRSpline.begin();
}

uint64_t AnimationRoutine::RDuration()
{
	return _QTSpline.end() - _QTSpline.begin();
}