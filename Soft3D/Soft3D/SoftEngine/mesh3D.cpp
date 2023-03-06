#include "mesh3D.h"
#include "SoftApp.h"
#include "renderer.h"

mesh3D::mesh3D() {

	m_Renderer = new renderer;

}

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
		m_Renderer->renderTriangle(vertices[t.v0],vertices[t.v1],vertices[t.v2], color(1, 1, 1, 1));

	}

}