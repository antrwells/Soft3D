#pragma once
#include <memory>
#include <string>
#include "glad/gl.h"
struct color;

class pixelMap
{
public:

	pixelMap(std::string path);
	pixelMap(int w, int h,int channels);
	void setPixel(int x, int y, color col);
	color getPixel(int x, int y);
	color getPixUV(float u, float v, float w = 1);
	void fill(color c);
	void drawPixmap(pixelMap* m, int x, int y, color c);
	void Display(int x, int y, int w, int h);
	int getWidth() {
		return m_Width;
	}
	int getHeight() {
		return m_Height;
	}
	int getChannels() {
		return m_Channels;
	}
	unsigned char* getData() {
		return m_Data;
	}



private:

	int m_Channels = 3;
	unsigned char* m_Data = nullptr;
	unsigned char* m_Blank = nullptr;
	int m_Width = 0;
	int m_Height = 0;
	GLuint m_DisplayHandle;
	bool fp = false;
};

