#pragma once
#include "SoftTypes.h"
#include <vector>

class renderer;
class nodeCamera;
class nodeLight;
class pixelMap;

class mesh3D
{
public:

	mesh3D();
	void AddVertex(vertex vert);
	void AddTriangle(triangle tri);
	void render(renderer* rend,matrix4 mat,nodeCamera* cam,nodeLight* light);
	void setColorMap(pixelMap* map) {
		m_ColorMap = map;
	}
	void setColor(color c) {
		m_Color = c;
	}
	color getColor() {
		return m_Color;
	}
private:

	std::vector<vertex> vertices;
	std::vector<triangle> triangles;
	renderer* m_Renderer;
	pixelMap* m_ColorMap = nullptr;
	color m_Color;

};

