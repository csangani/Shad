#include <Shad/Camera.h>

namespace Game
{
	void Camera::UpdatePosition(OpenMesh::Vec3f Object, OpenMesh::Vec3f Direction)
	{
		Pos = Object - Direction.normalized() * Dist;
	}

	OpenMesh::Vec3f Camera::Position() const
	{
		return Pos;
	}

	void Camera::Position(OpenMesh::Vec3f& Position)
	{
		Pos = Position;
	}

	OpenMesh::Vec3f Camera::Direction() const
	{
		return Dir;
	}

	void Camera::Direction(OpenMesh::Vec3f& Direction)
	{
		Dir = Direction;
	}

	float Camera::Distance() const
	{
		return Dist;
	}

	void Camera::Distance(float& Distance)
	{
		Dist = Distance;
	}
}