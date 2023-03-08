#pragma once
#include <glad/gl.h>

#include "Rect.h"
#include "Color.h"
//#include <glm/glm.hpp>
#include<vector>


class ShaderEffect;
class Texture2D;

struct Draw {

	float x[4];
	float y[4];
	float rx[4];
	float ry[4];
	float z;
	float r, g, b, a;
	bool flipH = false;

};

struct DrawList {

	Texture2D* mImage;
	Texture2D* mAux;
	std::vector<Draw*> mDraws;

};

class QuickDraw
{
public:

	QuickDraw();
	void SetViewport(Rect vp) {
		mViewPort = vp;
	}
	void DrawRect(Texture2D* tex,Rect rect, Color col);
	Draw* DrawRectRot(Texture2D* tex, Rect rect, Color col, Texture2D* aux, float rot = 0.0f, float scale = 1.0f, bool flipH = false);
	Draw* DrawSprite(Texture2D* tex, float x, float y, float w, float h,Color col, Texture2D* aux, float rot = 0.0f, float scale = 1.0f);
	void Begin();
	void End();
	void End(ShaderEffect* shader);
	DrawList* FindList(Texture2D* texture);
	void DrawTex(int x, int y, int w, int h, Texture2D* tex, float r, float g, float b, float a);

private:
	Rect mViewPort;
	ShaderEffect* mDrawBasic;
	GLuint vao, vbo, ebo;
	std::vector<DrawList*> mLists;
	float mZ = 0.0f;

};

