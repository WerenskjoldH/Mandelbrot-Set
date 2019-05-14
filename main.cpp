/*
	Project: Mandelbrot Set
	Name: Hunter Werenskjold
	Date: 5-14-29
	Description: Plots the Mandelbrot Set, this can very simply be altered to a Julia Set if need be
*/

#include "GLBitmap.h"

#define WINODW_WIDTH 600
#define WINODW_HEIGHT 600
#define SET_ITERATIONS 200
#define ZOOM_FACTOR 1.5f

/// STRUCT DEFINITIONS ///

struct complex
{
	complex(float r, float i) : r(r), i(i) {}

	float r, i;

	float magnitudeSquared() {
		return r*r + i*i;
	}

	complex& operator+(const complex t) const {
		return complex(r + t.r, i + t.i);
	}

	complex& operator*(const complex t) const {
		return complex(r * t.r - i * t.i, i * t.r + r * t.i);
	}
};

/// GLOBAL VARIABLES ///

GLBitmap bitmap(WINODW_WIDTH, WINODW_HEIGHT);

/// FUNCTION DEFINITIONS ///

inline int evaluateConvergence(float x, float y, int maxIterations) {
	float r = -ZOOM_FACTOR * (WINODW_WIDTH / 2.f - x) / (WINODW_WIDTH / 2.f) - 0.75f;
	float i = -ZOOM_FACTOR * (WINODW_HEIGHT / 2.f - y) / (WINODW_HEIGHT / 2.f);

	complex c(r, i);
	complex z(0, 0);

	double tR = 0;
	double tI = 0;

	for (int it = 0; it < maxIterations; it++)
		z = z * z + c;

	if (z.magnitudeSquared() <= 4.f)
		return 1;
	return 0;
}

void plotJuliaSet(uchar* pixels) {
	for (int x = 0; x < WINODW_WIDTH; x++)
		for (int y = 0; y < WINODW_HEIGHT; y++)
		{
			int index = y * WINODW_HEIGHT + x;

			pixels[index * 4 + 0] = 127 * evaluateConvergence(x, y, SET_ITERATIONS) + 127 * evaluateConvergence(x, y, 4);
			pixels[index * 4 + 1] = 50;
			pixels[index * 4 + 2] = 255 * evaluateConvergence(x, y, 16);
			pixels[index * 4 + 3] = 255;
		}
}

void display(void) {
	GLBitmap* bitmap = *(bitmap->getBitmapPtr());
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	plotJuliaSet(bitmap->getPixels());
	glDrawPixels(bitmap->getWidth(), bitmap->getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, bitmap->getPixels());
	glFlush();
}


static void keyboard(uchar key, int x, int y) {
	if (key == 27)
	{
		bitmap.quit();
	}
}

int main(void) {
	bitmap.renderAndQuit(display, keyboard);
}
