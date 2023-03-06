#pragma once
#include "SoftTypes.h"
#include <vector>
class mesh3D
{
public:

	void AddVertex(vertex vert);
	void AddTriangle(triangle tri);
	
private:

	std::vector<vertex> vertices;
	std::vector<triangle> triangles;

};

