#include "Texture2D.h"

#include "stb_image.h"
static std::vector<Texture2D*> mLUT;
#include <filesystem>
#include "VFile.h"
/*
Texture2D::Texture2D(FT_GlyphSlot slot) {

	mWidth = slot->bitmap.width;
	mHeight = slot->bitmap.rows;

	glGenTextures(1, &mHandle);
	glBindTexture(GL_TEXTURE_2D, mHandle);

	int form = GL_RED;

	//GLfloat maxAnisotropy = 0.0f;
	//glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexImage2D(GL_TEXTURE_2D, 0, form, mWidth, mHeight, 0, form, GL_UNSIGNED_BYTE,slot->bitmap.buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
	//glGenerateMipmap(GL_TEXTURE_2D);


}
*/

Texture2D::Texture2D(std::string path) {

	for (int i = 0; i < mLUT.size(); i++) {
		if (mLUT[i]->GetPath() == path) {

			mHandle = mLUT[i]->GetHandle();
			mWidth = mLUT[i]->GetWidth();
			mHeight = mLUT[i]->GetHeight();
			mPath = mLUT[i]->GetPath();
			return;

		}
	}
	mPath = path;
	mLUT.push_back(this);
	int width, height, channels;

	std::string lut_path = path + ".lut";

	unsigned char* img;

	if (std::filesystem::exists(lut_path))
	{

		VFile* file = new VFile(lut_path.c_str(), FileMode::Read);
		width = file->ReadInt();
		height = file->ReadInt();
		channels = file->ReadInt();
		img = (unsigned char*)file->ReadBytes(width * height * channels);
		file->Close();
	}
	else {

		img = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (img == NULL) {
			printf("Error in loading the image\n");
			exit(1);
		}

		VFile* file = new VFile(lut_path.c_str(), FileMode::Write);

		file->WriteInt(width);
		file->WriteInt(height);
		file->WriteInt(channels);
		file->WriteBytes((void*)img, width * height * channels);
		file->Close();
	}


	mPath = path;
	printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);
	
	mWidth = width;
	mHeight = height;

	glGenTextures(1, &mHandle);
	glBindTexture(GL_TEXTURE_2D, mHandle);

	int form = GL_RGB;
	if (channels == 4) {
		form = GL_RGBA;
	}
	GLfloat maxAnisotropy = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexImage2D(GL_TEXTURE_2D, 0, form, width, height, 0, form, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_SHARPEN_SGIS);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
	glGenerateMipmap(GL_TEXTURE_2D);




}

Texture2D::Texture2D(int w, int h) {

	mWidth = w;
	mHeight = h;

	glGenTextures(1, &mHandle);
	glBindTexture(GL_TEXTURE_2D, mHandle);

	int form = GL_RGB;
	//if (channels == 4) {
	//	form = GL_RGA;
	
	GLfloat maxAnisotropy = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexImage2D(GL_TEXTURE_2D, 0, form, w, h, 0, form, GL_UNSIGNED_BYTE,nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
	//glGenerateMipmap(GL_TEXTURE_2D);

}

void Texture2D::Bind(int unit) {

	glActiveTexture(GL_TEXTURE0+unit);
	glBindTexture(GL_TEXTURE_2D, mHandle);
	

}

void Texture2D::Release(int unit) {

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D,0);

}

void Texture2D::Copy(int x, int y) {
	//glFinish();
	Bind(0);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, x, y, mWidth, mHeight);
	Release(0);

}


Texture2D::Texture2D(int width, int height, void* data,int form) {


	mWidth = width;
	mHeight = height;

	glGenTextures(1, &mHandle);
	glBindTexture(GL_TEXTURE_2D, mHandle);

	//int form = form;


	glTexImage2D(GL_TEXTURE_2D, 0, form, width, height, 0, form, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glGenerateMipmap(GL_TEXTURE_2D);

}

Texture2D::Texture2D(int width, int height, void* data) {


	mWidth = width;
	mHeight = height;

	glGenTextures(1, &mHandle);
	glBindTexture(GL_TEXTURE_2D, mHandle);

	int form = GL_RGBA;
	
	glTexImage2D(GL_TEXTURE_2D, 0, form, width, height, 0, form, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glGenerateMipmap(GL_TEXTURE_2D);

}

Texture2D::~Texture2D() {

	glDeleteTextures(1, &mHandle);
	

}