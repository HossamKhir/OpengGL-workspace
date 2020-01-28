#include "stack_of_matrices.h"
using namespace std;

void
vDrawSimpleSolarSystem(void)
{
	// declaring variables (not wars)
	static float fAngleEarth = 3.1415906;
	static float fAngleMoon = 0;
	float fRGBSun[] = { 1.0f, 1.0f, 0.0f };
	float fRGBEarth[] = { 0.0f, 0.3f, 1.0f };
	float fRGBMoon[] = { 0.5f, 0.5f, 0.5f };

	// setting up the matrix pipeline 
	glMatrixMode(GL_MODELVIEW);

	// load identity matrix
	glLoadIdentity();

	// some operations
	glScalef(0.1, 0.1, 1.0f);	// decreasing origin unit scale by 10; enhancing 

	// let's draw the sun
	vDrawCircle(fRGBSun);

	// let's draw the earth
	// just a new scope so the effects do not span beyond it
	{
		glPushMatrix();	// to save current coordinates in stack

		// since earth rotates around sun first rotation is done then followed by translation
		glRotatef(fAngleEarth, 0, 0, 1);	// rotation around z-axis

		glTranslatef(0, 5, 0);	// shifting the origin of the sun coordinates

		glScalef(0.6f, 0.6f, 0.0);	// scaling down the earth's size

		vDrawCircle(fRGBEarth);	// drawing the earth

		fAngleEarth += 1;

		// let's draw the moon
		// just a new scope so the effects do not span beyond it
		{
			// saving current coordinates
			glPushMatrix();

			// since the moon rotates around the earth; first rotation then translation
			glRotatef(fAngleMoon, 0, 0, 1);	// rotation around z-axis

			glTranslatef(0, 3, 0);	// shifting origin of the earth coordinates

			glScalef(0.5f, 0.5f, 0.0);	// scaling down the moon's size

			vDrawCircle(fRGBMoon);

			fAngleMoon += 3;

			// restoring previouis coordinates
			glPopMatrix();
		}

		glPopMatrix();	// to retreive the previous coordinates
	}

}