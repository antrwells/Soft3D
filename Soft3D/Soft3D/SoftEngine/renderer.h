#pragma once
#include "SoftTypes.h"
#include <vector>
#include <thread>
#include <mutex>

class nodeCamera;
class nodeLight;
class pixelMap;

struct RenderThread {

	std::vector<rtri> tris;
	std::thread m_Thread;
	bool done = false;

};

class renderer
{
public:

	renderer();
	virtual void renderTriangle(vertex& v0,vertex& v1,vertex& v2,matrix4& model_mat,nodeCamera* cam,nodeLight* l,pixelMap* pix, color& col);
	//static void drawLine(int x1, int y1, int x2, int y2, color col);
	//static void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, color col);
	//void drawTriangle(vertex& v0, vertex& v1, vertex& v2, pixelMap* map, color col);
	void fillTriangleTex(int x1, int y1,float u1,float v1,float w1, int x2, int y2,float u2,float v2,float w2, int x3, int y3,float u3,float v3,float w3,pixelMap* map, color col);
	//static
	virtual void beginRender();
	virtual void endRender();
	
protected:
	std::vector<rtri> render_tris;
	matrix4 m_Projection;
	static std::vector<RenderThread*> renderThreads;
	static std::mutex bufferLock;
	static renderer* mThis;
};

