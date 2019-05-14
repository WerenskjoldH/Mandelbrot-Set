#ifndef GLBITMAP_H
#define GLBITMAP_H

/*
This was created based on NVIDIA Corporation's cpu_bitmap.h
*/

#include <GL\glut.h>

typedef unsigned char uchar;

class GLBitmap
{
public:
	GLBitmap(int width, int height, void *dataBlock = NULL) : width(width), height(height), dataBlock(dataBlock)
	{
		pixels = new uchar[width * height * 4];
	}

	~GLBitmap()
	{
		delete[] pixels;
	}

	uchar* getPixels() const
	{
		return pixels;
	}

	int imageSize() const
	{
		return width * height * 4;
	}

	int getWidth() const
	{
		return width;
	}

	int getHeight() const
	{
		return height;
	}

	void changeRedisplayBool(bool tf)
	{
		shouldRedisplay = tf;
	}

	void quit()
	{
		GLBitmap* bitmap = *(getBitmapPtr());
		if (bitmap->dataBlock != NULL && bitmap->exitFunct != NULL)
			bitmap->exitFunct(bitmap->dataBlock);
		exit(0);
	}

	static GLBitmap** getBitmapPtr()
	{
		static GLBitmap *glBitmap;
		return &glBitmap;
	}

	static void keyboard(uchar key, int x, int y)
	{
		if (key == 27)
		{
			GLBitmap* bitmap = *(getBitmapPtr());
			if (bitmap->dataBlock != NULL && bitmap->exitFunct != NULL)
				bitmap->exitFunct(bitmap->dataBlock);
			exit(0);
		}
	}

	static void draw(void) {
		GLBitmap*   bitmap = *(getBitmapPtr());
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawPixels(bitmap->width, bitmap->height, GL_RGBA, GL_UNSIGNED_BYTE, bitmap->pixels);
		glFlush();
	}

	static void idle()
	{
		glutPostRedisplay();
	}

	// D replaces draw
	// I replaces keyboard input
	void renderAndQuit(void(*d)() = NULL, void(*ki)(uchar key, int x, int y) = NULL, void(*e)(void*) = NULL)
	{
		GLBitmap** bitmap = getBitmapPtr();
		*bitmap = this;
		exitFunct = e;
		int cinit = 1;
		char* emptyCommands = "";
		glutInit(&cinit, &emptyCommands);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
		glutInitWindowSize(width, height);
		glutCreateWindow("Bitmap");
		

		if (d != NULL)
			glutDisplayFunc(d);
		else
			glutDisplayFunc(draw);

		if( ki != NULL)
			glutKeyboardFunc(ki);
		else
			glutKeyboardFunc(keyboard);

		if (shouldRedisplay)
			glutIdleFunc(idle);

		glutMainLoop();
	}

private:
	uchar *pixels;

	int width, height;

	bool shouldRedisplay = false;

	void* dataBlock;

	void(*exitFunct)(void*);
};

#endif