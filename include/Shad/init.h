#include <cstdlib>

#include <PolyMesh/bitmap_image.h>

//#define USE_XBOX_CONTROLLER

#define NUM_THREADS 8

#define NUM_BLUR_FRAMES 5

#define OBJECT "assets\\obj\\Platformer-tri.obj"
#define TEXTURE "assets\\bmp\\alien-mothership-green.bmp"
#define DEFAULT_SHADER "assets\\shaders\\default"
#define NORMAL_SHADER "assets\\shaders\\normal"
#define PHONG_SHADER "assets\\shaders\\phong"
#define TOON_SHADER "assets\\shaders\\toon"
#define ANTIALIAS_SHADER "assets\\shaders\\antialias"

#define MENU_START_TEXTURE "assets\\bmp\\menu_start_game.bmp"
#define MENU_QUIT_TEXTURE "assets\\bmp\\menu_quit.bmp"

#define AMBIENT 'a'
#define SPECULAR 's'
#define DIFFUSE 'd'
#define EMISSION 'e'
#define SHININESS 'h'
#define SUBDIVIDEP 'x'
#define UP '+'
#define DOWN '-'
#define SHADEMODE 'm'
#define DRAWMODE 'p'
#define RESET 'r'

#define JUMP ' '

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

float CapeSpecular[] = { 0.1f, 0.1f, 0.1f, 0.9f };

float CapeDiffuse[] = { 0.6f, 0.1f, 0.1f, 0.9f };

float CapeAmbient[] = { 0.6f, 0.1f, 0.1f, 0.9f };

float CapeShininess[] = { 0.0f };

float ClothColor[] = { 1.0f, 0.0f, 0.0f, 0.7f };

bitmap_image image;

bitmap_image space_image;

bitmap_image cloth_image;
bitmap_image cape_image;

bitmap_image menu_start_image;
bitmap_image menu_quit_image;