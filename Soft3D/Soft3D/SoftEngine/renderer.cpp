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

		fillTriangle(pv0.pos.x, pv0.pos.y, pv1.pos.x, pv1.pos.y, pv2.pos.x, pv2.pos.y, col);
		col = color(0, 0, 0, 0);
		drawLine(pv0.pos.x, pv0.pos.y, pv1.pos.x, pv1.pos.y, col);

		drawLine(pv1.pos.x, pv1.pos.y, pv2.pos.x, pv2.pos.y, col);
		drawLine(pv2.pos.x, pv2.pos.y, pv0.pos.x, pv0.pos.y, col);

	}


}

// Helper function to determine if a point (x, y) is inside a triangle defined by three points (x1, y1), (x2, y2), and (x3, y3)
bool isInsideTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y) {
	// Calculate the barycentric coordinates of the point with respect to the triangle
	double alpha = ((y2 - y3) * (x - x3) + (x3 - x2) * (y - y3)) / ((y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3));
	double beta = ((y3 - y1) * (x - x3) + (x1 - x3) * (y - y3)) / ((y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3));
	double gamma = 1.0 - alpha - beta;

	// Check if the barycentric coordinates are within the range [0, 1]
	return (alpha >= 0.0 && alpha <= 1.0 && beta >= 0.0 && beta <= 1.0 && gamma >= 0.0 && gamma <= 1.0);
}

void renderer::fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, color col)
{
	auto bb = SoftApp::m_This->getBackBuffer();
	int sw = SoftApp::m_This->getWidth();
	int sh = SoftApp::m_This->getHeight();
	auto SWAP = [](int& x, int& y) { int t = x; x = y; y = t; };
	auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) bb->setPixel(i, ny, col); };

	int t1x, t2x, y, minx, maxx, t1xp, t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int signx1, signx2, dx1, dy1, dx2, dy2;
	int e1, e2;
	// Sort vertices
	if (y1 > y2) { SWAP(y1, y2); SWAP(x1, x2); }
	if (y1 > y3) { SWAP(y1, y3); SWAP(x1, x3); }
	if (y2 > y3) { SWAP(y2, y3); SWAP(x2, x3); }

	t1x = t2x = x1; y = y1;   // Starting points
	dx1 = (int)(x2 - x1); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y2 - y1);

	dx2 = (int)(x3 - x1); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
	else signx2 = 1;
	dy2 = (int)(y3 - y1);

	if (dy1 > dx1) {   // swap values
		SWAP(dx1, dy1);
		changed1 = true;
	}
	if (dy2 > dx2) {   // swap values
		SWAP(dy2, dx2);
		changed2 = true;
	}

	e2 = (int)(dx2 >> 1);
	// Flat top, just process the second half
	if (y1 == y2) goto next;
	e1 = (int)(dx1 >> 1);

	for (int i = 0; i < dx1;) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			i++;
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) t1xp = signx1;//t1x += signx1;
				else          goto next1;
			}
			if (changed1) break;
			else t1x += signx1;
		}
		// Move line
	next1:
		// process second line until y value is about to change
		while (1) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;//t2x += signx2;
				else          goto next2;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next2:
		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawline(minx, maxx, y);    // Draw line from min to max points found on the y
		// Now increase y
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y == y2) break;

	}
next:
	// Second half
	dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y3 - y2);
	t1x = x2;

	if (dy1 > dx1) {   // swap values
		SWAP(dy1, dx1);
		changed1 = true;
	}
	else changed1 = false;

	e1 = (int)(dx1 >> 1);

	for (int i = 0; i <= dx1; i++) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) { t1xp = signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if (i < dx1) i++;
		}
	next3:
		// process second line until y value is about to change
		while (t2x != x3) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;
				else          goto next4;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next4:

		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawline(minx, maxx, y);
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y > y3) return;
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