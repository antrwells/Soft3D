#pragma once
#include "SoftTypes.h"

class nodeCamera;
class nodeLight;

class renderer
{
public:

	renderer();
	void renderTriangle(vertex v0,vertex v1,vertex v2, matrix4 mat,nodeCamera* cam,nodeLight* l, color col);
	void drawLine(int x1, int y1, int x2, int y2, color col);
	void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, color col);

private:

	matrix4 m_Projection;

};

