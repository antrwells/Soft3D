#include "renderer.h"
#include "SoftApp.h"
#include "pixelMap.h"
#include "nodeCamera.h"
#include "nodeLight.h"
 renderer::renderer() {

	 m_Projection = matrix4();
	 m_Projection.toProjection(90, (float)SoftApp::m_This->getHeight() / (float)SoftApp::m_This->getWidth(), 0.1, 100);

}

void renderer::renderTriangle(vertex v0,vertex v1,vertex v2,matrix4 mat,nodeCamera* cam,nodeLight* light, color col) {

	int sw = SoftApp::m_This->getWidth();
	int sh = SoftApp::m_This->getHeight();


	vertex pv0, pv1, pv2;
	
	pv0.pos = mat.multiply(v0.pos);
	pv1.pos = mat.multiply(v1.pos);
	pv2.pos = mat.multiply(v2.pos);

	v3d camPos = cam->getPos();

	pv0.pos = pv0.pos.minus(camPos);
	pv1.pos = pv1.pos.minus(camPos);
	pv2.pos = pv2.pos.minus(camPos);

	//pv0.pos.z += 
	//pv1.pos.z += 3.0f;
	//pv2.pos.z += 3.0f;


	v3d normal, line1, line2;
	line1.x = pv1.pos.x - pv0.pos.x;
	line1.y = pv1.pos.y - pv0.pos.y;
	line1.z = pv1.pos.z - pv0.pos.z;

	line2.x = pv2.pos.x - pv0.pos.x;
	line2.y = pv2.pos.y - pv0.pos.y;
	line2.z = pv2.pos.z - pv0.pos.z;

	normal.x = line1.y * line2.z - line1.z * line2.y;
	normal.y = line1.z * line2.x - line1.x * line2.z;
	normal.z = line1.x * line2.y - line1.y * line2.x;

	float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
	normal.x /= l;
	normal.y /= l;
	normal.z /= l;
	camPos = v3d(0, 0, 0);
	if ( normal.x * (pv0.pos.x-camPos.x) +
		 normal.y * (pv0.pos.y-camPos.y) +
		 normal.z * (pv0.pos.z-(camPos.z)) < 0.0)
	{

		v3d light_dir = light->getPos();
		light_dir.x = -light_dir.x;
		light_dir.y = -light_dir.y;
		light_dir.z = -light_dir.z;
		light_dir.normalize();

		float dp = normal.dot(light_dir);
		printf("DP:%f\n", dp);


		pv0.pos = m_Projection.multiply(pv0.pos);
		pv1.pos = m_Projection.multiply(pv1.pos);
		pv2.pos = m_Projection.multiply(pv2.pos);

		pv0.pos.x += 1.0f; pv0.pos.y += 1.0f;
		pv1.pos.x += 1.0f; pv1.pos.y += 1.0f;
		pv2.pos.x += 1.0f; pv2.pos.y += 1.0f;

		pv0.pos.x *= 0.5f * (float)sw;
		pv0.pos.y *= 0.5f * (float)sh;
		pv1.pos.x *= 0.5f * (float)sw;
		pv1.pos.y *= 0.5f * (float)sh;
		pv2.pos.x *= 0.5f * (float)sw;
		pv2.pos.y *= 0.5f * (float)sh;


		col = col.mult(dp);

		drawLine(pv0.pos.x, pv0.pos.y, pv1.pos.x, pv1.pos.y, col);
		drawLine(pv1.pos.x, pv1.pos.y, pv2.pos.x, pv2.pos.y, col);
		drawLine(pv2.pos.x, pv2.pos.y, pv0.pos.x, pv0.pos.y, col);

	}


}

void renderer::drawLine(int x1, int y1, int x2, int y2, color col) {

	
	auto bb = SoftApp::m_This->getBackBuffer();
	int sw = SoftApp::m_This->getWidth();
	int sh = SoftApp::m_This->getHeight();

	int xd, yd;

	xd = x2 - x1;
	yd = y2 - y1;

	if (abs(xd) > sw * 2) return;
	if (abs(yd) > sh * 2) return;


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

	if (steps > 1) {
		for (int i = 0; i < steps; i++) {

			if (xp >= 0 && xp < sw && yp >= 0 && yp < sh) {
				bb->setPixel(xp, yp,col);
			}

			xp += xi;
			yp += yi;
		}
	}
}