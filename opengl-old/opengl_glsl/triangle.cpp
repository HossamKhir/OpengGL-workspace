#include "triangle.h"

using namespace std;

void
vDrawTriangle(float fRGB[])
{
	// to draw polygons
	glBegin(GL_TRIANGLES);	// GL_TRIANGLES is a flag for triangles

	glColor3f(fRGB[RED_INDEX], fRGB[GREEN_INDEX], fRGB[BLUE_INDEX]);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	glEnd();	// represents end of vertices of the polygon sent
}

void
vDrawTriangle(float fRGB[],
	float fX,
	float fY)
{
	static float sfX = 0.0f;
	static float sfY = -1.0f;

	// to draw polygons
	glBegin(GL_TRIANGLES);	// GL_TRIANGLES is a flag for triangles

	glColor3f(fRGB[RED_INDEX], fRGB[GREEN_INDEX], fRGB[BLUE_INDEX]);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(sfX, sfY, 0.0f);
	glVertex3f(fX, fY, 0.0f);

	glEnd();	// represents end of vertices of the polygon sent

	sfX = fX;
	sfY = fY;
}