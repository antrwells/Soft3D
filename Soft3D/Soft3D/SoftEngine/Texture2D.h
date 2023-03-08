#pragma once

#include <glad/gl.h>
#include <string>
#include <vector>

class Texture2D
{
public:
	Texture2D(int handle) {
		mHandle = handle;
	}
	Texture2D(int width, int height, void* data);
	Texture2D(int width, int height, void* data, int form);
	Texture2D(std::string path);
	Texture2D(int width, int height);
	void Copy(int x, int y);
	~Texture2D();
	void Bind(int unit);
	void Release(int unit);
	//Texture2D(FT_GlyphSlot* slot);
	int GetWidth() {
		return mWidth;
	}
	int GetHeight() {
		return mHeight;
	}
	GLuint GetHandle() {
		return mHandle;
	}
	std::string GetPath() {
		return mPath;
	}
private:
	GLuint mHandle;
	int mWidth;
	int mHeight;
	bool mAlpha;
	std::string mPath;

};

