#include <OpenMesh/Core/Geometry/VectorT.hh>

namespace Game
{
	class Camera
	{
	public:
		OpenMesh::Vec3f VerticalAxis;

		Camera() : Pos(OpenMesh::Vec3f(10.0f, 0.0f, 10.0f)), Dir(OpenMesh::Vec3f(0.0f, 0.0f, -1.0f)), Dist(10.f), VerticalAxis(OpenMesh::Vec3f(0,0,-13)){}
		Camera(OpenMesh::Vec3f Position, OpenMesh::Vec3f Direction, float Distance) : Pos(Position), Dir(Direction), Dist(Distance) {}

		void UpdatePosition(OpenMesh::Vec3f Object, OpenMesh::Vec3f Direction);

		OpenMesh::Vec3f Position() const;
		void Position(OpenMesh::Vec3f&);

		OpenMesh::Vec3f Direction() const;
		void Direction(OpenMesh::Vec3f&);

		float Distance() const;
		void Distance(float&);

	private:
		OpenMesh::Vec3f Pos;
		OpenMesh::Vec3f Dir;
		float Dist;
	};
}