#include "quad.h"

using namespace std;

void
vDrawQuad(float fRGB[])
{
	glBegin(GL_QUADS);	/* this is like an opening brace
						for the following code */

	// the colour must always be defined before vertices
	glColor3f(fRGB[RED_INDEX], fRGB[RED_INDEX], fRGB[RED_INDEX]);
	glVertex3f(0.0, 0.0, 0.0);

	// the openGL interpolates the colours
	glColor3f(fRGB[GREEN_INDEX], fRGB[GREEN_INDEX], fRGB[GREEN_INDEX]);
	glVertex3f(-1.0, 0.0, 0.0);

	glColor3f(fRGB[BLUE_INDEX], fRGB[BLUE_INDEX], fRGB[BLUE_INDEX]);
	glVertex3f(-1.0, -1.0, 0.0);

	glColor3f(fRGB[RED_INDEX], fRGB[GREEN_INDEX], fRGB[BLUE_INDEX]);
	glVertex3f(0.0, -1.0, 0.0);

	glEnd();	/*this is like a closing brace for the
				previous code */

}