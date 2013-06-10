#include <cstdlib>

#include <PolyMesh/bitmap_image.h>

#define USE_XBOX_CONTROLLER

#define NUM_THREADS 8

#define NUM_BLUR_FRAMES 5

#define OBJECT "assets\\obj\\Platformer-tri.obj"
#define TEXTURE "assets\\bmp\\alien-mothership-green.bmp"
#define DEFAULT_SHADER "assets\\shaders\\default"
#define NORMAL_SHADER "assets\\shaders\\normal"
#define PHONG_SHADER "assets\\shaders\\phong"
#define TOON_SHADER "assets\\shaders\\toon"

#define MENU_START_TEXTURE "assets\\bmp\\menu_start_game.bmp"
#define MENU_QUIT_TEXTURE "assets\\bmp\\menu_quit.bmp"
#define MENU_INVERT_YES_TEXTURE "assets\\bmp\\menu_invert_yes.bmp"
#define MENU_INVERT_NO_TEXTURE "assets\\bmp\\menu_invert_no.bmp"

#define SKYBOX_FRONT_TEXTURE "assets\\bmp\\skybox_front.bmp"
#define SKYBOX_BACK_TEXTURE "assets\\bmp\\skybox_back.bmp"
#define SKYBOX_LEFT_TEXTURE "assets\\bmp\\skybox_left.bmp"
#define SKYBOX_RIGHT_TEXTURE "assets\\bmp\\skybox_right.bmp"
#define SKYBOX_TOP_TEXTURE "assets\\bmp\\skybox_top.bmp"
#define SKYBOX_BOTTOM_TEXTURE "assets\\bmp\\skybox_bottom.bmp"

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

float Diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };

float Ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };

float Shininess[] = { 0.2f };

float CapeSpecular[] = { 0.1f, 0.1f, 0.1f, 0.9f };

float CapeDiffuse[] = { .4f, .4f, .4f, 0.9f };

float CapeAmbient[] = { .4f, .4f, .4f, 0.9f };

float CapeShininess[] = { 0.2f };

float ClothColor[] = { .5f, .5f, .5f, 0.7f };

bitmap_image image;

bitmap_image space_image;

bitmap_image cloth_image;
bitmap_image cape_image;

bitmap_image menu_start_image;
bitmap_image menu_quit_image;
bitmap_image menu_invert_yes_image;
bitmap_image menu_invert_no_image;
