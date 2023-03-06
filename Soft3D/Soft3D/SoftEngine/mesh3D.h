#pragma once
#include "SoftTypes.h"
#include <vector>

class renderer;
class nodeCamera;

class mesh3D
{
public:

	mesh3D();
	void AddVertex(vertex vert);
	void AddTriangle(triangle tri);
	void render(matrix4 mat,nodeCamera* cam);

private:

	std::vector<vertex> vertices;
	std::vector<triangle> triangles;
	renderer* m_Renderer;

};

