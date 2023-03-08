#include "Test1.h"

#include "Importer.h"
#include "sceneGraph.h"
#include "nodeEntity.h"
#include "gameInput.h"
#include "pixelMap.h"

void Test1::Init() {

	auto imp = new Importer;

	ent1 = imp->importEntity("data/box1.fbx");
	g1 = new sceneGraph;
	g1->addNode(ent1);
	cam = g1->getCam();
	cam->setPos(0, 0, -8);
	cam->setRotation(0, 0, 0);
	auto l1 = new nodeLight;
	l1->setPos(0, 0, 3);
	g1->addLight(l1);
	tex1 = new pixelMap("data/tex1.png");
	auto m1 = ent1->getMesh(0);
	m1->setColorMap(tex1);
}
float ang2 = 0;

float camX=0, camY=0;

void Test1::Render() {

	//printf("Rendering entity.\n");
	//ent1->render();
	ang2 += 0.1f;
	camY += gameInput::mouseDeltaX * 0.1;
	camX += gameInput::mouseDeltaY*0.1;
	
	
	cam->setRotation(camX,camY, 0);

	cam->move(-gameInput::moveX * 0.1, 0, gameInput::moveY * 0.1);

	//ent1->setPos(50, 0,200);
	


	ent1->setRotation(ang2*2, ang2, 0);
	g1->render();

	auto bb = SoftApp::getBackBuffer();
	//bb->drawPixmap(tex1, 20, 100, color(1, 1, 1, 1));



}