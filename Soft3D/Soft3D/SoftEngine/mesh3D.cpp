#include "mesh3D.h"
#include "SoftApp.h"

void mesh3D::AddVertex(vertex vert) {

	vertices.push_back(vert);

}

void mesh3D::AddTriangle(triangle tri) {

	triangles.push_back(tri);

}

void mesh3D::render() {

	auto bb = SoftApp::m_This->getBackBuffer();
	auto db = SoftApp::m_This->getDepthBuffer();

	for (int tri = 0; tri < triangles.size(); tri++) {

		auto t = triangles[tri];

	}

}