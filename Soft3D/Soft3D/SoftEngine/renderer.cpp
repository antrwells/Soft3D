#include "renderer.h"
#include "SoftApp.h"
#include <iostream>
#include "pixelMap.h"
#include "nodeCamera.h"
#include "nodeLight.h"
#include <algorithm>
#include <list>
#include "pixelMap.h"
#include <chrono>
#include "depthBuffer.h"
#include <windows.h>


const int RenderThreads = 11;

 renderer::renderer() {

	 m_Projection = matrix4();
	 m_Projection.toProjection(70, (float)SoftApp::m_This->getHeight() / (float)SoftApp::m_This->getWidth(), 0.00001, 20);
	 mThis = this;

}
 



void renderer::renderTriangle(vertex& v0,vertex& v1,vertex& v2,matrix4& model_mat,nodeCamera* cam,nodeLight* light,pixelMap* pix, color& col) {

	int sw = SoftApp::m_This->getWidth();
	int sh = SoftApp::m_This->getHeight();

//	printf("Rendered Tri\n");
	vertex pv0, pv1, pv2;

	
	pv0.pos = model_mat.multiply(v0.pos);
	pv1.pos = model_mat.multiply(v1.pos);
	pv2.pos = model_mat.multiply(v2.pos);
	pv0.tex_coord = v0.tex_coord;
	pv1.tex_coord = v1.tex_coord;
	pv2.tex_coord = v2.tex_coord;

	v3d mid_tri = pv0.pos.add(pv1.pos).add(pv2.pos);
	mid_tri.div(3.0f);


	v3d camPos = cam->getPos();

//	pv0.pos = pv0.pos.minus(camPos);
//	pv1.pos = pv1.pos.minus(camPos);
//	pv2.pos = pv2.pos.minus(camPos);

	//pv0.pos.z += 
	//pv1.pos.z += 3.0f;
	//pv2.pos.z += 3.0f;



	v3d normal, line1, line2;
	v3d l_normal;
	line1 = pv1.pos.minus(pv0.pos);
	line2 = pv2.pos.minus(pv0.pos);
	normal = line1.cross(line2);

	normal.normalize();

	l_normal = normal;

	l_normal = pv0.normal.add(pv1.normal).add(pv2.normal);
	l_normal = normal.div(3.0f);
	l_normal.normalize();

	auto cam_mat = cam->getWorldMat();
	pv0.pos = cam_mat.multiply(pv0.pos);
	pv1.pos = cam_mat.multiply(pv1.pos);
	pv2.pos = cam_mat.multiply(pv2.pos);


	rtri triv;
	triv.p0 = pv0.pos;
	triv.p1 = pv1.pos;
	triv.p2 = pv2.pos;
	triv.t0 = pv0.tex_coord;
	triv.t1 = pv1.tex_coord;
	triv.t2 = pv2.tex_coord;
	triv.map = pix;



//	v3d normal, line1, line2;

	line1 = pv1.pos.minus(pv0.pos);
	line2 = pv2.pos.minus(pv0.pos);
	normal = line1.cross(line2);

	normal.normalize();



	camPos = v3d(0, 0, 0);
	v3d camRay = pv0.pos.minus(camPos);


	

	if (normal.dot(camRay)<0.0f)
	{

		v3d light_dir = light->getPos();
		//light_dir.x = -light_dir.x;
		//light_dir.y = -light_dir.y;
		//light_dir.z = -light_dir.z;

		light_dir = light_dir.minus(mid_tri);

		light_dir.normalize();




		float dp = light_dir.dot(l_normal);
		if (dp < 0) dp = 0;
	
		int nClippedTriangles = 0;
		rtri clipped[2];
		
		nClippedTriangles = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triv, clipped[0], clipped[1]);

		

		for (int n = 0; n < nClippedTriangles; n++)
		{
			rtri tp;
			// Project triangles from 3D --> 2D
			tp.p0 = m_Projection.multiply(clipped[n].p0);
			tp.p1 = m_Projection.multiply(clipped[n].p1);
			tp.p2 = m_Projection.multiply(clipped[n].p2);
			tp.t0 = clipped[n].t0;
			tp.t1 = clipped[n].t1;
			tp.t2 = clipped[n].t2;


			tp.t0.x = tp.t0.x / tp.p0.w;
			tp.t1.x = tp.t1.x / tp.p1.w;
			tp.t2.x = tp.t2.x / tp.p2.w;
			tp.t0.y = tp.t0.y / tp.p0.w;
			tp.t1.y = tp.t1.y / tp.p1.w;
			tp.t2.y = tp.t2.y / tp.p2.w;
			tp.t0.w = 1.0f / tp.p0.w;
			tp.t1.w = 1.0f / tp.p1.w;
			tp.t2.w = 1.0f / tp.p2.w;

			
			//triProjected.col = clipped[n].col;
			//triProjected.sym = clipped[n].sym;


			tp.p0 = tp.p0.div(tp.p0.w);
			tp.p1 = tp.p1.div(tp.p1.w);
			tp.p2 = tp.p2.div(tp.p2.w);


			tp.p0.x *= -1.0f;
			tp.p1.x *= -1.0f;
			tp.p2.x *= -1.0f;
			tp.p0.y *= -1.0f;
			tp.p1.y *= -1.0f;
			tp.p2.y *= -1.0f;


			v3d vOffset(1, 1, 0);
			
			tp.p0 = tp.p0.add(vOffset);
			tp.p1 = tp.p1.add(vOffset);
			tp.p2 = tp.p2.add(vOffset);

			//pv0.pos = pv0.pos.add(vOffset);
			//pv1.pos = pv1.pos.add(vOffset);
			//pv2.pos = pv2.pos.add(vOffset);

			//pv0.pos.x += 1.0f; pv0.pos.y += 1.0f;
			//pv1.pos.x += 1.0f; pv1.pos.y += 1.0f;
		//	pv2.pos.x += 1.0f; pv2.pos.y += 1.0f;

			tp.p0.x *= 0.5f * (float)sw;
			tp.p0.y *= 0.5f * (float)sh;
			tp.p1.x *= 0.5f * (float)sw;
			tp.p1.y *= 0.5f * (float)sh;
			tp.p2.x *= 0.5f * (float)sw;
			tp.p2.y *= 0.5f * (float)sh;
			tp.map = clipped[n].map;
			
		

			tp.c0 = col;
			tp.c0.r *= dp;
			tp.c0.g *= dp;
			tp.c0.b *= dp;
			//return;

			render_tris.push_back(tp);
			
			//fillTriangle(tp.p0.x, tp.p0.y, tp.p1.x, tp.p1.y, tp.p2.x, tp.p2.y, col);
			//col = color(0, 0, 0, 0);

			

		

	
				

		}

		int bb = 5;
		//printf("RTRI:%d\n", render_tris.size());


		/*
		pv0.pos = m_Projection.multiply(pv0.pos);
		pv1.pos = m_Projection.multiply(pv1.pos);
		pv2.pos = m_Projection.multiply(pv2.pos);

		pv0.pos = pv0.pos.div(pv0.pos.w);
		pv1.pos = pv1.pos.div(pv1.pos.w);
		pv2.pos = pv2.pos.div(pv2.pos.w);

		v3d vOffset(1, 1, 0);
		pv0.pos = pv0.pos.add(vOffset);
		pv1.pos = pv1.pos.add(vOffset);
		pv2.pos = pv2.pos.add(vOffset);

		//pv0.pos.x += 1.0f; pv0.pos.y += 1.0f;
		//pv1.pos.x += 1.0f; pv1.pos.y += 1.0f;
	//	pv2.pos.x += 1.0f; pv2.pos.y += 1.0f;

		pv0.pos.x *= 0.5f * (float)sw;
		pv0.pos.y *= 0.5f * (float)sh;
		pv1.pos.x *= 0.5f * (float)sw;
		pv1.pos.y *= 0.5f * (float)sh;
		pv2.pos.x *= 0.5f * (float)sw;
		pv2.pos.y *= 0.5f * (float)sh;
		*/


		//col = col.mult(dp);

		//fillTriangle(pv0.pos.x, pv0.pos.y, pv1.pos.x, pv1.pos.y, pv2.pos.x, pv2.pos.y, col);
		//col = color(0, 0, 0, 0);
		//drawLine(pv0.pos.x, pv0.pos.y, pv1.pos.x, pv1.pos.y, col);

		//drawLine(pv1.pos.x, pv1.pos.y, pv2.pos.x, pv2.pos.y, col);
		//drawLine(pv2.pos.x, pv2.pos.y, pv0.pos.x, pv0.pos.y, col);

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


void renderer::fillTriangleTex(int x1, int y1, float u1, float v1, float w1, int x2, int y2, float u2, float v2, float w2, int x3, int y3, float u3, float v3, float w3, pixelMap* map, color col)
{
	auto bb = SoftApp::m_This->getBackBuffer();
	auto db = SoftApp::m_This->getDepthBuffer();
	float* db_b = db->getData();
	int db_w = bb->getWidth();
	unsigned char* bb_b = bb->getData();
	int sw = SoftApp::m_This->getWidth();
	int sh = SoftApp::m_This->getHeight();
	int map_w = map->getWidth();
	int map_h = map->getHeight();
	unsigned char* map_b = map->getData();
	float tr, tg, tb;

	if (y2 < y1)
	{
		std::swap(y1, y2);
		std::swap(x1, x2);
		std::swap(u1, u2);
		std::swap(v1, v2);
		std::swap(w1, w2);
	}

	if (y3 < y1)
	{
		std::swap(y1, y3);
		std::swap(x1, x3);
		std::swap(u1, u3);
		std::swap(v1, v3);
		std::swap(w1, w3);
	}

	if (y3 < y2)
	{
		std::swap(y2, y3);
		std::swap(x2, x3);
		std::swap(u2, u3);
		std::swap(v2, v3);
		std::swap(w2, w3);
	}

	int dy1 = y2 - y1;
	int dx1 = x2 - x1;
	float dv1 = v2 - v1;
	float du1 = u2 - u1;
	float dw1 = w2 - w1;

	int dy2 = y3 - y1;
	int dx2 = x3 - x1;
	float dv2 = v3 - v1;
	float du2 = u3 - u1;
	float dw2 = w3 - w1;

	float tex_u, tex_v, tex_w;

	float dax_step = 0, dbx_step = 0,
		du1_step = 0, dv1_step = 0,
		du2_step = 0, dv2_step = 0,
		dw1_step = 0, dw2_step = 0;

	if (dy1) dax_step = dx1 / (float)abs(dy1);
	if (dy2) dbx_step = dx2 / (float)abs(dy2);

	if (dy1) du1_step = du1 / (float)abs(dy1);
	if (dy1) dv1_step = dv1 / (float)abs(dy1);
	if (dy1) dw1_step = dw1 / (float)abs(dy1);

	if (dy2) du2_step = du2 / (float)abs(dy2);
	if (dy2) dv2_step = dv2 / (float)abs(dy2);
	if (dy2) dw2_step = dw2 / (float)abs(dy2);

	if (dy1)
	{
		for (int i = y1; i <= y2; i++)
		{
			int ax = x1 + (float)(i - y1) * dax_step;
			int bx = x1 + (float)(i - y1) * dbx_step;

			float tex_su = u1 + (float)(i - y1) * du1_step;
			float tex_sv = v1 + (float)(i - y1) * dv1_step;
			float tex_sw = w1 + (float)(i - y1) * dw1_step;

			float tex_eu = u1 + (float)(i - y1) * du2_step;
			float tex_ev = v1 + (float)(i - y1) * dv2_step;
			float tex_ew = w1 + (float)(i - y1) * dw2_step;

			if (ax > bx)
			{
				std::swap(ax, bx);
				std::swap(tex_su, tex_eu);
				std::swap(tex_sv, tex_ev);
				std::swap(tex_sw, tex_ew);
			}

			tex_u = tex_su;
			tex_v = tex_sv;
			tex_w = tex_sw;

			float tstep = 1.0f / ((float)(bx - ax));
			float t = 0.0f;

			for (int j = ax; j < bx; j++)
			{
				tex_u = (1.0f - t) * tex_su + t * tex_eu;
				tex_v = (1.0f - t) * tex_sv + t * tex_ev;
				tex_w = (1.0f - t) * tex_sw + t * tex_ew;
				//if (tex_w > pDepthBuffer[i * ScreenWidth() + j])
				//{
				// b
				// 


				float pz = db_b[i * db_w + j]; //((float)db->getDepth(j, i));
			
				if (tex_w > pz) {

				//	color texc = map->getPixUV(tex_u / tex_w, tex_v / tex_w);

					int tx, ty;

					tx = (tex_u / tex_w) * map_w;
					ty = (tex_v / tex_w) * map_h;
					
					int tloc = ty * map_w * 3 + (tx * 3);

					tr = map_b[tloc++];
					tg = map_b[tloc++];
					tb = map_b[tloc++];

					tr = (tr / 255.0f) * col.r;
					tg = (tg / 255.0f) * col.g;
					tb = (tb / 255.0f) * col.b;
				
				//	color texc = color(tr / 255.0f, tg / 255.0f, tb / 255.0f);
						
					int bloc = (i * sw * 3) + (j * 3);

					bb_b[bloc++] = tr * 255;
					bb_b[bloc++] = tg * 255;
					bb_b[bloc] = tb * 255;

			//		bb->setPixel(j, i,texc.mult(col));
				
					//db->setDepth(j, i, tex_w);
					db_b[i * db_w + j] = tex_w;

				}
				//Draw(j, i, tex->SampleGlyph(tex_u / tex_w, tex_v / tex_w), tex->SampleColour(tex_u / tex_w, tex_v / tex_w));
					//pDepthBuffer[i * ScreenWidth() + j] = tex_w;
				//}


				t += tstep;
			}

		}
	}

	dy1 = y3 - y2;
	dx1 = x3 - x2;
	dv1 = v3 - v2;
	du1 = u3 - u2;
	dw1 = w3 - w2;

	if (dy1) dax_step = dx1 / (float)abs(dy1);
	if (dy2) dbx_step = dx2 / (float)abs(dy2);

	du1_step = 0, dv1_step = 0;
	if (dy1) du1_step = du1 / (float)abs(dy1);
	if (dy1) dv1_step = dv1 / (float)abs(dy1);
	if (dy1) dw1_step = dw1 / (float)abs(dy1);

	if (dy1)
	{
		for (int i = y2; i <= y3; i++)
		{
			int ax = x2 + (float)(i - y2) * dax_step;
			int bx = x1 + (float)(i - y1) * dbx_step;

			float tex_su = u2 + (float)(i - y2) * du1_step;
			float tex_sv = v2 + (float)(i - y2) * dv1_step;
			float tex_sw = w2 + (float)(i - y2) * dw1_step;

			float tex_eu = u1 + (float)(i - y1) * du2_step;
			float tex_ev = v1 + (float)(i - y1) * dv2_step;
			float tex_ew = w1 + (float)(i - y1) * dw2_step;

			if (ax > bx)
			{
				std::swap(ax, bx);
				std::swap(tex_su, tex_eu);
				std::swap(tex_sv, tex_ev);
				std::swap(tex_sw, tex_ew);
			}

			tex_u = tex_su;
			tex_v = tex_sv;
			tex_w = tex_sw;

			float tstep = 1.0f / ((float)(bx - ax));
			float t = 0.0f;

			for (int j = ax; j < bx; j++)
			{
				tex_u = (1.0f - t) * tex_su + t * tex_eu;
				tex_v = (1.0f - t) * tex_sv + t * tex_ev;
				tex_w = (1.0f - t) * tex_sw + t * tex_ew;

				//if (tex_w > pDepthBuffer[i * ScreenWidth() + j])
			//	{
				float pz = db_b[i * db_w + j];

				if (tex_w > pz) {
					int tx, ty;

					tx = (tex_u / tex_w) * map_w;
					ty = (tex_v / tex_w) * map_h;

					int tloc = ty * map_w * 3 + (tx * 3);

					tr = map_b[tloc++];
					tg = map_b[tloc++];
					tb = map_b[tloc++];

					tr = (tr / 255.0f) * col.r;
					tg = (tg / 255.0f) * col.g;
					tb = (tb / 255.0f) * col.b;

					//	color texc = color(tr / 255.0f, tg / 255.0f, tb / 255.0f);

					int bloc = (i * sw * 3) + (j * 3);

					bb_b[bloc++] = tr * 255;
					bb_b[bloc++] = tg * 255;
					bb_b[bloc] = tb * 255;

					//		bb->setPixel(j, i,texc.mult(col));

							//db->setDepth(j, i, tex_w);
					db_b[i * db_w + j] = tex_w;


				}
				//bb->setPixel(j, i, map->getPixUV(tex_u / tex_w, tex_v / tex_w, 1.0f).mult(col));
				//bb->setPixel(j, i, col);


				//Draw(j, i, tex->SampleGlyph(tex_u / tex_w, tex_v / tex_w), tex->SampleColour(tex_u / tex_w, tex_v / tex_w));
				//	pDepthBuffer[i * ScreenWidth() + j] = tex_w;
			//	}
				t += tstep;
			}
		}
	}
}

/*
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
*/

/*
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

*/

void renderer::beginRender() {

	render_tris.clear();
//	printf("Render Began\n");

}


void th_renderThread(renderer* render,RenderThread *tr,int thread_id)
{
	

	//printf("Thread:%d\n", thread_id);
	for (auto& tp : tr->tris) {

		color col = tp.c0;
		render->fillTriangleTex(tp.p0.x, tp.p0.y, tp.t0.x, tp.t0.y, tp.t0.w, tp.p1.x, tp.p1.y, tp.t1.x, tp.t1.y, tp.t1.w, tp.p2.x, tp.p2.y, tp.t2.x, tp.t2.y, tp.t2.w, tp.map, col);

	}

	tr->done = true;


}


void renderer::endRender() {
	int sw = SoftApp::m_This->getWidth();
	int sh = SoftApp::m_This->getHeight();

	std::vector<rtri> final_tris;

//	printf("Render Ended\n");

//	for (int i = 0; i < render_tris.size(); i++) {
	for(auto &triToRaster : render_tris)
	{


		//  ensure we only test new triangles generated against planes
		rtri clipped[2];
		std::list<rtri> listTriangles;

		// Add initial triangle
		listTriangles.push_back(triToRaster);
		int nNewTriangles = 1;

		for (int p = 0; p < 4; p++)
		{
			int nTrisToAdd = 0;
			while (nNewTriangles > 0)
			{
				// Take triangle from front of queue
				rtri test = listTriangles.front();
				listTriangles.pop_front();
				nNewTriangles--;

				// Clip it against a plane. We only need to test each 
				// subsequent plane, against subsequent new triangles
				// as all triangles after a plane clip are guaranteed
				// to lie on the inside of the plane. I like how this
				// comment is almost completely and utterly justified
				switch (p)
				{
				case 0:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 1:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, (float)sh - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 2:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 3:	nTrisToAdd = Triangle_ClipAgainstPlane({ (float)sw- 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				}

				// Clipping may yield a variable number of triangles, so
				// add these new ones to the back of the queue for subsequent
				// clipping against next planes
				for (int w = 0; w < nTrisToAdd; w++)
					listTriangles.push_back(clipped[w]);
			}
			nNewTriangles = listTriangles.size();
		}



		// Draw the transformed, viewed, clipped, projected, sorted, clipped triangles
		for (auto& tp : listTriangles)
		{
			//final_tris.push_back(tp);
			color col = tp.c0;

			fillTriangleTex(tp.p0.x, tp.p0.y,tp.t0.x,tp.t0.y,tp.t0.w,  tp.p1.x, tp.p1.y,tp.t1.x,tp.t1.y,tp.t1.w, tp.p2.x, tp.p2.y,tp.t2.x,tp.t2.y,tp.t2.w,tp.map, col);
			//FillTriangle(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y, t.sym, t.col);
			//DrawTriangle(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y, PIXEL_SOLID, FG_BLACK);
		}

		//auto tp = render_tris[i];
		//color col = tp.c0;
		//fillTriangle(tp.p0.x, tp.p0.y, tp.p1.x, tp.p1.y, tp.p2.x, tp.p2.y, col);


	}
	
	return;

	int num_tris = final_tris.size() / RenderThreads;

	renderThreads.clear();

	for (int i = 0; i < RenderThreads; i++) {

		auto rt = new RenderThread;

		renderThreads.push_back(rt);

	}

	int tri_i = 0;
	int rt_i = 0;

	for (auto& tri : final_tris) {

		auto rt = renderThreads[rt_i];

		rt->tris.push_back(tri);

		tri_i++;

		if (tri_i >= num_tris) {
			tri_i = 0;
			rt_i++;
			if (rt_i == RenderThreads) break;
		}
	}

	for (int i = 0; i < renderThreads.size(); i++) {

		auto rt = renderThreads[i];

		rt->m_Thread=  std::thread(th_renderThread,mThis,rt, i);
		
	
		DWORD_PTR dw = SetThreadAffinityMask(rt->m_Thread.native_handle(), DWORD_PTR(1) << (i+1));
		if (dw == 0)
		{
			DWORD dwErr = GetLastError();
			std::cerr << "SetThreadAffinityMask failed, GLE=" << dwErr << '\n';
		}

		//thr.detach();
		//for (auto& tp : rt->tris)
		//{
		//	color col = tp.c0;

		//	fillTriangleTex(tp.p0.x, tp.p0.y, tp.t0.x, tp.t0.y, tp.t0.w, tp.p1.x, tp.p1.y, tp.t1.x, tp.t1.y, tp.t1.w, tp.p2.x, tp.p2.y, tp.t2.x, tp.t2.y, tp.t2.w, tp.map, col);
		//}

	}

	if (renderThreads.size() > 0) {

		for (int i = 0; i < renderThreads.size(); i++) {
			renderThreads[i]->m_Thread.join();
			delete renderThreads[i];
		}

		return;
		using namespace std::chrono_literals;
		//int dc = 0;
		while (true) {
			int dc = 0;
			//std::this_thread::sleep_for(1ms);

			for (int i = 0; i < renderThreads.size(); i++) {

				if (renderThreads[i]->done)
				{
					dc++;
				}

			}
			if (dc == renderThreads.size()) {
				for (int i = 0; i < renderThreads.size(); i++)
				{
					delete renderThreads[i];
				}
				break;
			}
		}
		//int vv = 5;

	}

}


//std::vector<rtri> renderer::render_tris;

std::vector<RenderThread*> renderer::renderThreads;
std::mutex renderer::bufferLock;
renderer* renderer::mThis = nullptr;