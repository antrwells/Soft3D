#pragma once
#include "SoftTypes.h"
#include <vector>

class renderer;

class mesh3D
{
public:

	mesh3D();
	void AddVertex(vertex vert);
	void AddTriangle(triangle tri);
	void render();

private:

	std::vector<vertex> vertices;
	std::vector<triangle> triangles;
	renderer* m_Renderer;

};

