#pragma once
#include "SoftTypes.h"
#include <vector>
class nodeCamera;
class nodeLight;

class renderer
{
public:

	renderer();
	void renderTriangle(vertex v0,vertex v1,vertex v2, matrix4 mat,nodeCamera* cam,nodeLight* l, color col);
	static void drawLine(int x1, int y1, int x2, int y2, color col);
	static void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, color col);
	static std::vector<rtri> render_tris;
	static void beginRender();
	static void endRender();

private:

	matrix4 m_Projection;

};

