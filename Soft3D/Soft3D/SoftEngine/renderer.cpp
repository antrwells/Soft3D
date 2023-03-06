#include "renderer.h"
#include "SoftApp.h"
#include "pixelMap.h"

 renderer::renderer() {

	 m_Projection = matrix4();
	 m_Projection.toProjection(90, (float)SoftApp::m_This->getHeight() / (float)SoftApp::m_This->getWidth(), 0.1, 100);

}

void renderer::renderTriangle(vertex v0,vertex v1,vertex v2, color col) {

	int sw = SoftApp::m_This->getWidth();
	int sh = SoftApp::m_This->getHeight();


	vertex pv0, pv1, pv2;
	
	pv0.pos = m_Projection.multiply(v0.pos);
	pv1.pos = m_Projection.multiply(v1.pos);
	pv2.pos = m_Projection.multiply(v2.pos);

	pv0.pos.x += 1.0f; pv0.pos.y += 1.0f;
	pv1.pos.x += 1.0f; pv1.pos.y += 1.0f;
	pv2.pos.x += 1.0f; pv2.pos.y += 1.0f;

	pv0.pos.x *= 0.5f * (float)sw;
	pv0.pos.y *= 0.5f * (float)sh;
	pv1.pos.x *= 0.5f * (float)sw;
	pv1.pos.y *= 0.5f * (float)sh;
	pv2.pos.x *= 0.5f * (float)sw;
	pv2.pos.y *= 0.5f * (float)sh;



	drawLine(pv0.pos.x, pv0.pos.y, pv1.pos.x, pv1.pos.y, col);
	drawLine(pv1.pos.x, pv1.pos.y, pv2.pos.x, pv2.pos.y, col);
	drawLine(pv2.pos.x, pv2.pos.y, pv0.pos.x, pv0.pos.y, col);




}

void renderer::drawLine(int x1, int y1, int x2, int y2, color col) {

	auto bb = SoftApp::m_This->getBackBuffer();
	int sw = SoftApp::m_This->getWidth();
	int sh = SoftApp::m_This->getHeight();

	int xd, yd;

	xd = x2 - x1;
	yd = y2 - y1;

	float steps = 0;

	if (abs(xd) > abs(yd)) {
		steps = abs(xd);
	}
	else {
		steps = abs(yd);
	}

	float xi, yi;

	xi = xd / steps;
	yi =yd / steps;

	float xp, yp;
	xp = x1;
	yp = y1;

	for (int i = 0; i < steps; i++) {

		if (xp >= 0 && xp < sw && yp >= 0 && yp < sh) {
			bb->setPixel(xp, yp, color(1, 1, 1, 1));
		}

		xp += xi;
		yp += yi;
	}

}