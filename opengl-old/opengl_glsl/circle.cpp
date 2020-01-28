#include "circle.h"
using namespace std;

void
vDrawCircle(float fRGB[])
{
	// define center coordinates
	float xCenter = 0.0f;
	float yCenter = 0.0f;
	// define radius
	float radius = 1.f;

	// first point
	float rx = xCenter;
	float ry = yCenter - radius;

	for (int i = 0; i <= steps; i++)
	{
		float stepAngle = angle*i;

		/* coordinates of the following point by
		rotation */
		float newRx = radius*sinf(stepAngle);
		float newRy = -radius*cosf(stepAngle);

		vDrawTriangle(fRGB, newRx, newRy);

		// set the new point as the current point
		rx = newRx;
		ry = newRy;
	}
}