#ifndef POLYMESH_H
#define POLYMESH_H

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

#include <Shad/Shader.h>
#include <PolyMesh/AnimationRoutine.h>
#include <PolyMesh/Physics.h>

#include <GL/glew.h>
#include <GL/glut.h>

#define RADIANS(angle) (angle*2*((float)M_PI)/360.0f)

struct PolyTraits : public OpenMesh::DefaultTraits
{  
	VertexAttributes(OpenMesh::Attributes::Status);  
	FaceAttributes(OpenMesh::Attributes::Status);  
	EdgeAttributes(OpenMesh::Attributes::Status);
};

class PolyMesh : public OpenMesh::PolyMesh_ArrayKernelT<PolyTraits>
{
public:

	static std::vector<PolyMesh *> Meshes;
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
	int CollisionGroup;
	bool Cloth;

	PolyMesh();

	PolyMesh *ApplyTexture(const unsigned char *, int, int);

	PolyMesh *AttachShader(std::string);

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

	void Delete();

private:
	bool Animated;
	bool Lighting;
	GLuint ShaderID;

	std::vector<GLuint> indices;

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