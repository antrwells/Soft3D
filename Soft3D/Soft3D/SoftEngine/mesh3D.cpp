#include "mesh3D.h"
#include "SoftApp.h"
#include "renderer.h"
#include "nodeCamera.h"
#include "rendererNitro.h"
mesh3D::mesh3D() {

	m_Renderer = new renderer;

}

void mesh3D::AddVertex(vertex vert) {

	vertices.push_back(vert);

}

void mesh3D::AddTriangle(triangle tri) {

	triangles.push_back(tri);

}

void mesh3D::render(renderer* rend,matrix4 mat,nodeCamera* cam,nodeLight* l) {

	auto bb = SoftApp::m_This->getBackBuffer();
	auto db = SoftApp::m_This->getDepthBuffer();

	for (int tri = 0; tri < triangles.size(); tri++) {

		auto t = triangles[tri];
		color col(1, 1, 1, 1);

		rend->renderTriangle(vertices[t.v0],vertices[t.v1],vertices[t.v2],mat,cam, l,m_ColorMap, m_Color);
		//rendererNitro::drawTriangle(vertices[t.v0], vertices[t.v1], vertices[t.v2], m_Color, m_ColorMap,mat,cam,l);
	}

}