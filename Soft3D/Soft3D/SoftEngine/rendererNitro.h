#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include "SoftTypes.h"
#include "renderer.h"
class nodeCamera;
class nodeLight;
class pixelMap;
class rendererNitro : public renderer
{
public:


	rendererNitro();

	void beginRender();
	void renderTriangle(vertex& v0, vertex& v1, vertex& v2, matrix4& model_mat, nodeCamera* cam, nodeLight* l, pixelMap* pix, color& col);
	void endRender();
	void drawTri(vertex& v0, vertex& v1, vertex& v2, color col, pixelMap* pix);



private:
	matrix4 m_Projection;
	static std::vector<rtri> final_tris;
	static rendererNitro* mThis;
};

