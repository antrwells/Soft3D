#include "rendererNitro.h"
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
#include <math.h>

rendererNitro* rendererNitro::mThis = nullptr;
std::vector<rtri> rendererNitro::final_tris;

float renW, renH;

struct scanData {

	int currentY;
	float ndotla;
	float ndotlb;
	float ndotlc;
	float ndotld;

	float ua;
	float ub;
	float uc;
	float ud;

	float va;
	float vb;
	float vc;
	float vd;

};

rendererNitro::rendererNitro() {

	m_Projection = matrix4();
	m_Projection.toProjection(70, (float)SoftApp::m_This->getHeight() / (float)SoftApp::m_This->getWidth(), 0.00001, 20);
	mThis = this;
	renW = SoftApp::m_This->getWidth();
	renH = SoftApp::m_This->getHeight();

}

void rendererNitro::beginRender() {

	render_tris.clear();

}

vertex project(vertex& v, matrix4 transMat, matrix4 world)
{

	

	v3d point2d = transMat.multiply(v.pos);
	v3d point3Dworld = world.multiply(v.pos);
	v3d normalWorld = world.multiply(v.normal);

	auto x = point2d.x * renW + renW / 2.0f;
	auto y = -point2d.y * renH + renH / 2.0f;

	vertex new_vert;

	new_vert.pos = v3d(x, y, point2d.z);
	new_vert.normal = normalWorld;
	new_vert.world_pos = point3Dworld;
	new_vert.tex_coord = v.tex_coord;

	return new_vert;

	

}

void rendererNitro::renderTriangle(vertex& v0, vertex& v1, vertex& v2, matrix4& model_mat, nodeCamera* cam, nodeLight* l, pixelMap* pix, color& col)
{
	rtri t;
	t.v0 = v0;
	t.v1 = v1;
	t.v2 = v2;
	t.map = pix;
	t.c0 = col;
	t.world = model_mat;
	t.view = cam->getWorldMat();
	render_tris.push_back(t);

}

/*
void rendererNitro::drawTriangle(vertex& v0, vertex& v1, vertex& v2, color& col, pixelMap* tex,matrix4& mat,nodeCamera* cam,nodeLight* l)
{

	matrix4 world = mat;
	matrix4 view = cam->getWorldMat();
	matrix4 proj = matrix4();
	proj.toProjection(70, (float)SoftApp::m_This->getHeight() / (float)SoftApp::m_This->getWidth(), 0.00001, 20);
	matrix4 worldView = world.multi(view);
	matrix4 transform = worldView.multi(proj);

	rtri t;
	t.v0 = v0;
	t.v1 = v1;
	t.v2 = v2;
	t.map = tex;
	final_tris.push_back(t);

}
*/


float ComputeNDotL(v3d vertex, v3d normal, v3d lightPosition)
{
	auto lightDirection = lightPosition.minus(vertex);

	normal.normalize();
	lightDirection.normalize();

	float dp = normal.dot(lightDirection);
	if (dp < 0.1) dp = 0.1;
	return dp;
}

float clamp(float v) {

	if (v < 0) v = 0;
	if (v > 1) v = 1;
	return v;

}

float Interpolate(float min, float max, float gradient)
{
	return min + (max - min) * clamp(gradient);
}

void processScanLine(scanData& data, vertex a, vertex b, vertex c, vertex d, color col, pixelMap* pix)
{

	auto bb = SoftApp::m_This->getBackBuffer();
	auto db = SoftApp::m_This->getDepthBuffer();
	float* db_b = db->getData();
	int db_w = bb->getWidth();
	unsigned char* bb_b = bb->getData();
	int sw = SoftApp::m_This->getWidth();
	int sh = SoftApp::m_This->getHeight();
	int map_w = pix->getWidth();
	int map_h = pix->getHeight();
	unsigned char* map_b = pix->getData();
	float tr, tg, tb;



	v3d pa = a.pos;
	v3d pb = b.pos;
	v3d pc = c.pos;
	v3d pd = d.pos;

	auto gradient1 = pa.y != pb.y ? (data.currentY - pa.y) / (pb.y - pa.y) : 1;
	auto gradient2 = pc.y != pd.y ? (data.currentY - pc.y) / (pd.y - pc.y) : 1;

	int sx = (int)Interpolate(pa.x, pb.x, gradient1);
	int ex = (int)Interpolate(pc.x, pd.x, gradient2);

	// starting Z & ending Z
	float z1 = Interpolate(pa.z, pb.z, gradient1);
	float z2 = Interpolate(pc.z, pd.z, gradient2);

	auto snl = Interpolate(data.ndotla, data.ndotlb, gradient1);
	auto enl = Interpolate(data.ndotlc, data.ndotld, gradient2);

	// Interpolating texture coordinates on Y
	auto su = Interpolate(data.ua, data.ub, gradient1);
	auto eu = Interpolate(data.uc, data.ud, gradient2);
	auto sv = Interpolate(data.va, data.vb, gradient1);
	auto ev = Interpolate(data.vc, data.vd, gradient2);

	for (auto x = sx; x < ex; x++)
	{
		float gradient = (x - sx) / (float)(ex - sx);

		// Interpolating Z, normal and texture coordinates on X
		auto z = Interpolate(z1, z2, gradient);
		auto ndotl = Interpolate(snl, enl, gradient);
		auto u = Interpolate(su, eu, gradient);
		auto v = Interpolate(sv, ev, gradient);

		color textureColor;

		//if (pix != nullptr)
			//textureColor = texture.Map(u, v);
	//	else
		//	textureColor = new Color4(1, 1, 1, 1);
		textureColor = color(1, 1, 1);

		// changing the native color value using the cosine of the angle
		// between the light vector and the normal vector
		// and the texture color
		//DrawPoint(new Vector3(x, data.currentY, z), color * ndotl * textureColor);
		bb->setPixel(x, data.currentY, color(ndotl,ndotl,ndotl));
	}


}


void rendererNitro::drawTri(vertex& v1, vertex& v2, vertex& v3, color col, pixelMap* pix)
{

	if (v1.pos.y > v2.pos.y)
	{
		auto temp = v2;
		v2 = v1;
		v1 = temp;
	}

	if (v2.pos.y > v3.pos.y)
	{
		auto temp = v2;
		v2 = v3;
		v3 = temp;
	}

	if (v1.pos.y > v2.pos.y)
	{
		auto temp = v2;

		v2 = v1;
		v1 = temp;
	}

	v3d p1 = v1.pos;
	v3d p2 = v2.pos;
	v3d p3 = v3.pos;

	v3d lightPos = v3d(0, 8, -8);

	float nl1 = ComputeNDotL(v1.world_pos, v1.normal, lightPos);
	float nl2 = ComputeNDotL(v2.world_pos, v2.normal, lightPos);
	float nl3 = ComputeNDotL(v3.world_pos, v3.normal, lightPos);

	scanData data;

	float dP1P2, dP1P3;

	// http://en.wikipedia.org/wiki/Slope
	// Computing slopes
	if (p2.y - p1.y > 0)
		dP1P2 = (p2.x - p1.x) / (p2.y - p1.y);
	else
		dP1P2 = 0;

	if (p3.y - p1.y > 0)
		dP1P3 = (p3.x - p1.y) / (p3.y - p1.y);
	else
		dP1P3 = 0;


	if (dP1P2 > dP1P3)
	{
		for (auto y = (int)p1.y; y <= (int)p3.y; y++)
		{
			data.currentY = y;

			if (y < p2.y)
			{
				data.ndotla = nl1;
				data.ndotlb = nl3;
				data.ndotlc = nl1;
				data.ndotld = nl2;

				data.ua = v1.tex_coord.x;
				data.ub = v3.tex_coord.x;
				data.uc = v1.tex_coord.x;
				data.ud = v2.tex_coord.x;

				data.va = v1.tex_coord.y;
				data.vb = v3.tex_coord.y;
				data.vc = v1.tex_coord.y;
				data.vd = v2.tex_coord.y;

				processScanLine(data, v1, v3, v1, v2,col,pix);
			}
			else
			{
				data.ndotla = nl1;
				data.ndotlb = nl3;
				data.ndotlc = nl2;
				data.ndotld = nl3;

				data.ua = v1.tex_coord.x;
				data.ub = v3.tex_coord.x;
				data.uc = v2.tex_coord.x;
				data.ud = v3.tex_coord.x;

				data.va = v1.tex_coord.y;
				data.vb = v3.tex_coord.y;
				data.vc = v2.tex_coord.y;
				data.vd = v3.tex_coord.y;


				processScanLine(data, v1, v3, v2, v3, col, pix);
			}
		}
	}
	else
	{
		for (auto y = (int)p1.y; y <= (int)p3.y; y++)
		{
			data.currentY = y;

			if (y < p2.y)
			{
				data.ndotla = nl1;
				data.ndotlb = nl2;
				data.ndotlc = nl1;
				data.ndotld = nl3;

				data.ua = v1.tex_coord.x;
				data.ub = v2.tex_coord.x;
				data.uc = v1.tex_coord.x;
				data.ud = v3.tex_coord.x;

				data.va = v1.tex_coord.y;
				data.vb = v2.tex_coord.y;
				data.vc = v1.tex_coord.y;
				data.vd = v3.tex_coord.y;


				processScanLine(data, v1, v2, v1, v3, col, pix);
			}
			else
			{
				data.ndotla = nl2;
				data.ndotlb = nl3;
				data.ndotlc = nl1;
				data.ndotld = nl3;

				data.ua = v2.tex_coord.x;;
				data.ub = v3.tex_coord.x;
				data.uc = v1.tex_coord.x;
				data.ud = v3.tex_coord.x;

				data.va = v2.tex_coord.y;
				data.vb = v3.tex_coord.y;
				data.vc = v1.tex_coord.y;
				data.vd = v3.tex_coord.y;


				processScanLine(data, v2, v3, v1, v3, col,pix);
			}
		}
	}

}

void rendererNitro::endRender() {


	for (auto& tri : render_tris) {

		matrix4 worldView = tri.world.multi(tri.view);
		matrix4 trans = worldView.multi(m_Projection);

		auto va = tri.v0;
		auto vb = tri.v1;
		auto vc = tri.v2;

		auto pa = project(va, trans, tri.world);
		auto pb = project(vb, trans, tri.world);
		auto pc = project(vc, trans, tri.world);

		int a = 5;

		drawTri(pa, pb, pc, color(1, 1, 1), tri.map);


	}

}