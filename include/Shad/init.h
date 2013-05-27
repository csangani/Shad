#include <cstdlib>

#include <PolyMesh/bitmap_image.h>

#define FRAME_RATE 60

#define FRAME_PERIOD (1000.0/FRAME_RATE)

#define NUM_THREADS 8

#define OBJECT "assets\\obj\\cube.obj"
#define TEXTURE "assets\\bmp\\alien-mothership-green.bmp"
#define DEFAULT_SHADER "assets\\shaders\\default"
#define NORMAL_SHADER "assets\\shaders\\normal"
#define PHONG_SHADER "assets\\shaders\\phong"
#define TOON_SHADER "assets\\shaders\\toon"

#define AMBIENT 'a'
#define SPECULAR 's'
#define DIFFUSE 'd'
#define EMISSION 'e'
#define SHININESS 'h'
#define SUBDIVIDE ' '
#define SUBDIVIDEP 'x'
#define UP '+'
#define DOWN '-'
#define SHADEMODE 'm'
#define DRAWMODE 'p'
#define RESET 'r'

void Boost(float *array, int size, float delta)
{
	if (array != NULL)
		for(int i = 0; i < size; i++)
		{
			if(array[i] != NULL)
				array[i] += delta;
		}
}


float Specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };

float Diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };

float Ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };

float Shininess[] = { 0.5f };

bitmap_image image;

bitmap_image space_image;

bitmap_image cloth_image;
