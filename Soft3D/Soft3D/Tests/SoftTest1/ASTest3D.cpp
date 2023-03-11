#include "ASTest3D.h"
#include "Importer.h"
#include "sceneGraph.h"
#include "nodeEntity.h"
#include "gameInput.h"
#include "pixelMap.h"
#include "AudioSystem.h"
#include "QuickDraw.h"
#include "Texture2D.h"
void ASTest3D::Init() {
	auto imp = new Importer;

	ent1 = imp->importEntity("data/light1.fbx");
	tex1 = new pixelMap("data/tex1.png");
	auto mh = ent1->getMeshes();
	for (int i = 0; i < mh.size(); i++) {
		auto m = mh[i];
		m->setColorMap(tex1);
	}

	g1 = new sceneGraph;
	g1->addNode(ent1);
	cam = g1->getCam();
	cam->setPos(0, 5, -8);
	cam->setRotation(0, 0, 0);
	l1 = new nodeLight;
	l1->setPos(4, -10, 8);
	g1->addLight(l1);
	
//	auto m1 = ent1->getMesh(0);//
//	m1->setColorMap(tex1);
}
float aang2 = 0;

float acamX = 0, acamY = 0;
void ASTest3D::Update() {
	aang2 += 0.7f;
	acamY += gameInput::mouseDeltaX * 0.1;
	acamX += gameInput::mouseDeltaY * 0.1;


	cam->setRotation(acamX, acamY, 0);

	cam->move(-gameInput::moveX * 0.1, 0, gameInput::moveY * 0.1);

	//ent1->setPos(50, 0,200);



	float lx, lz;

	lx = cosf(deg2rad(aang2)) * 15;
	lz = sinf(deg2rad(aang2)) * 15;

	l1->setPos(lx, 7, lz);
	
	ent1->setRotation(0,0, 0);
}

void ASTest3D::Render() {


	g1->render();
}