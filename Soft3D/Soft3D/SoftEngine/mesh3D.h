#pragma once
#include "SoftTypes.h"
#include <vector>
class mesh3D
{
public:

	void AddVertex(vertex vert);
	void AddTriangle(triangle tri);
	void render();

private:

	std::vector<vertex> vertices;
	std::vector<triangle> triangles;

};

