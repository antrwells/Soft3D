#include "Test1.h"

#include "Importer.h"
#include "sceneGraph.h"
#include "nodeEntity.h"
#include "gameInput.h"

void Test1::Init() {

	auto imp = new Importer;

	ent1 = imp->importEntity("data/head.fbx");
	g1 = new sceneGraph;
	g1->addNode(ent1);
	cam = g1->getCam();
	cam->setPos(0, 0, -8);
	cam->setRotation(0, 0, 0);
	auto l1 = new nodeLight;
	l1->setPos(0, 0, 3);
	g1->addLight(l1);

}
float ang2 = 0;

float camX=0, camY=0;

void Test1::Render() {

	//printf("Rendering entity.\n");
	//ent1->render();
	ang2 += 0.1f;
	camY -= gameInput::mouseDeltaX * 0.1;
	camX -= gameInput::mouseDeltaY*0.1;
	
	
	cam->setRotation(camX,camY, 0);
	//ent1->setPos(50, 0,200);
	ang2 = 0;


	ent1->setRotation(ang2*2, ang2, 0);
	g1->render();
}