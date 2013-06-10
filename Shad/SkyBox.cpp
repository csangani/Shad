#include <Shad/SkyBox.h>

SkyBox::SkyBox(float cx, float cy, float cz, float size) :
	cx_(cx), cy_(cy), cz_(cz), size_(size)
{
	glGenTextures(6, textures_);
}

GLuint SkyBox::addTexture(std::string path, SkyBoxFace face)
{
	/* load image from file */
	bitmap_image image = bitmap_image(path);
	image.rgb_to_bgr();
	/* bind and create texture */
	glBindTexture(GL_TEXTURE_2D, textures_[face]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	/* clean up */
	glBindTexture(GL_TEXTURE_2D, 0);

	return textures_[face];
}

void SkyBox::draw()
{
	glUseProgram(0);

	glDepthMask(false);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	/* get back-bottom-left corner from center */
	float x = cx_ - size_ / 2.f;
	float y = cy_ - size_ / 2.f;
	float z = cz_ - size_ / 2.f;

	glEnable(GL_TEXTURE_2D);
	
	glEnable(GL_LIGHTING);

	float Clear[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Clear);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Clear);

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Clear);

			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Clear);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Clear);

	/* Draw Front side */
	glBindTexture(GL_TEXTURE_2D, textures_[SkyBoxFront]);
	glBegin(GL_QUADS);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y,	   z+size_);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y+size_, z+size_);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+size_, y+size_, z+size_); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+size_, y,	   z+size_);
	glEnd();

	/* Draw Back side */
	glBindTexture(GL_TEXTURE_2D, textures_[SkyBoxBack]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y,		z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+size_, y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+size_, y+size_,  z); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y+size_,	z);
	glEnd();

	/* Draw Left side */
	glBindTexture(GL_TEXTURE_2D, textures_[SkyBoxLeft]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y,		z+size_);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y+size_,	z);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y+size_,	z+size_); 		
	glEnd();

	/* Draw Right side */
	glBindTexture(GL_TEXTURE_2D, textures_[SkyBoxRight]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+size_, y,		z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+size_, y,		z+size_);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+size_, y+size_,	z+size_); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+size_, y+size_,	z);
	glEnd();

	/* Draw Up side */
	glBindTexture(GL_TEXTURE_2D, textures_[SkyBoxTop]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,       y+size_,  z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+size_, y+size_,  z); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+size_, y+size_,	z+size_);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+size_,	z+size_);
	glEnd();

	/* Draw Down side */
	glBindTexture(GL_TEXTURE_2D, textures_[SkyBoxBottom]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z+size_);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+size_, y,		z+size_);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+size_, y,		z); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y,		z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glPopMatrix();

	glDepthMask(true);
}