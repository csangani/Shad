#include <Spline/stdint.h>
#include <map>

#ifndef SPLINE_HH
#define SPLINE_HH

template <typename T>
class Spline
{
public:

	Spline() : _CPL(std::map<uint64_t, T>()) {}

	Spline(std::map<uint64_t, T> CPL) : _CPL(CPL)	{}

	uint64_t begin()
	{
		return T::begin(_CPL);
	}

	uint64_t end()
	{
		return T::end(_CPL);
	}

	T operator()(const uint64_t& u)
	{
		return T::Interpolate(_CPL, u);
	}

	void AddControlPoint(T vec, uint64_t ts)
	{
		_CPL.insert(std::pair<uint64_t,T>(ts,vec));
	}

	int size()
	{
		return _CPL.size();
	}

private:
	std::map<uint64_t, T> _CPL;
};

#endif