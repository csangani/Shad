#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

#include <PolyMesh/AnimationRoutine.h>

#include <GL/glew.h>
#include <GL/glut.h>

struct PolyTraits : public OpenMesh::DefaultTraits
{  
	VertexAttributes(OpenMesh::Attributes::Status);  
	FaceAttributes(OpenMesh::Attributes::Status);  
	EdgeAttributes(OpenMesh::Attributes::Status);
};

class PolyMesh : OpenMesh::PolyMesh_ArrayKernelT<PolyTraits>
{
public:

	static std::vector<PolyMesh *> Meshes;
	static uint64_t Time;

	float max_x, min_x;
	float max_y, min_y;
	float max_z, min_z;

	float cen_x, cen_y, cen_z;

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

	PolyMesh();

	PolyMesh(std::string);

	PolyMesh::PolyMesh(std::string, GLenum);

	PolyMesh::PolyMesh(std::string, GLenum, GLenum);

	PolyMesh *ApplyTexture(const unsigned char *, int, int);

	PolyMesh *AttachShader(std::string);

	PolyMesh *PolyMesh::EnableLighting();

	PolyMesh *Scale(float, float, float);

	PolyMesh *Normalize();

	PolyMesh *Translate(float, float, float);

	PolyMesh *Center();
	
	PolyMesh *Rotate(float, float, float, float);
	PolyMesh *RotateAboutCenter(float, float, float, float);

	PolyMesh *GenerateNormals();

	PolyMesh *LoopSubdivide();
	PolyMesh *LoopSubdivideP(int);

	PolyMesh *Draw();

	PolyMesh *LoadObj(std::string);

	PolyMesh *Animate(AnimationRoutine *, uint64_t, bool);

	PolyMesh *DrawAnimation();

	void Delete();

private:
	bool Animated;
	bool Lighting;
	GLuint ShaderID;

	float ModelView[16];

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