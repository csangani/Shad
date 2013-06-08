#ifndef POLYMESH_H
#define POLYMESH_H

#include <list>

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

#include <Shad/Shader.h>
#include <Shad/PlatformEdge.h>
#include <PolyMesh/AnimationRoutine.h>
#include <PolyMesh/Physics.h>

#include <GL/glew.h>
#include <GL/glut.h>

#define RADIANS(angle) (angle*((float)M_PI)/180.0f)
#define DEGREES(angle) (angle*180.0f/(float)M_PI)

#define BVECO(X) btVector3((X)[0], (X)[1], (X)[2])
#define OVECB(X) OpenMesh::Vec3f((X).x(), (X).y(), (X).z())
#define BVEC3F btVector3
#define OVEC3F OpenMesh::Vec3f

#define FRAME_RATE 60
#define FRAME_PERIOD (1000.0/FRAME_RATE)

struct PolyTraits : public OpenMesh::DefaultTraits
{  
	VertexAttributes(OpenMesh::Attributes::Status);  
	FaceAttributes(OpenMesh::Attributes::Status);  
	EdgeAttributes(OpenMesh::Attributes::Status);
};

class PolyMesh : public OpenMesh::PolyMesh_ArrayKernelT<PolyTraits>
{
public:

	static std::list<PolyMesh *> Meshes;
	static uint64_t Time;

	OpenMesh::Vec3f max;
	OpenMesh::Vec3f min;

	GLenum DrawMode;

	GLenum ShadeMode;

	const unsigned char * Texture;
	int TextureWidth, TextureHeight;
	GLuint TextureID;

	float *Color;

	float *MaterialSpecular, *MaterialAmbient, *MaterialDiffuse,
		*MaterialEmission, *MaterialShininess;
	GLenum MaterialFaceMode;

	AnimationRoutine *Animation;
	uint64_t AnimationDelay, AnimationOffset;
	bool AnimationRepeat;

	btRigidBody *RigidBody;
	bool cloth, character, platform, particleCloth;

	PolyMesh();

	~PolyMesh();

	PolyMesh *ApplyTexture(const unsigned char *, int, int);

	PolyMesh *AttachShader(std::string);

	PolyMesh *DetachShader();

	PolyMesh *PolyMesh::EnableLighting();

	PolyMesh *Scale(OpenMesh::Vec3f);

	PolyMesh *Normalize();

	PolyMesh *Translate(OpenMesh::Vec3f);

	PolyMesh *SetOrigin(OpenMesh::Vec3f origin);
	
	PolyMesh *Rotate(float, float, float, float);

	PolyMesh *GenerateNormals();

	PolyMesh *LoopSubdivide();
	PolyMesh *LoopSubdivideP(int);

	PolyMesh *Draw();

	PolyMesh *LoadObj(std::string);

	PolyMesh *Animate(AnimationRoutine *, uint64_t, bool);

	PolyMesh *DrawAnimation();

	PolyMesh *SetMass(float);

	PolyMesh *GenerateRigidBody();

	void Delete();

private:
	bool Animated;
	bool Lighting;
	GLuint ShaderID;
	std::vector<GLuint> ShaderStack;

	typedef struct LoopVertex 
	{
		VertexHandle newVH;
		Point point;
		TexCoord2D tex;
	};

	typedef struct LoopFace
	{
		int a, b, c, d, e, f;
	};
};

#endif