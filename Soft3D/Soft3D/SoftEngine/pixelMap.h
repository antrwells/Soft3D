#pragma once
#include <memory>
#include "glad/gl.h"
struct color;

class pixelMap
{
public:

	pixelMap(int w, int h,int channels);
	void setPixel(int x, int y, color col);
	color getPixel(int x, int y);
	void fill(color c);
	void Display(int x, int y, int w, int h);


private:

	int m_Channels = 3;
	char* m_Data = nullptr;
	int m_Width = 0;
	int m_Height = 0;
	GLuint m_DisplayHandle;
};

