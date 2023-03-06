#include "mesh3D.h"

void mesh3D::AddVertex(vertex vert) {

	vertices.push_back(vert);

}

void mesh3D::AddTriangle(triangle tri) {

	triangles.push_back(tri);

}
