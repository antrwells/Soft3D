#include "Test1.h"

#include "Importer.h"
#include "sceneGraph.h"
#include "nodeEntity.h"

void Test1::Init() {

	auto imp = new Importer;

	ent1 = imp->importEntity("data/test2.fbx");
	g1 = new sceneGraph;
	g1->addNode(ent1);
	auto cam = g1->getCam();
	cam->setPos(0, 0, -3);

	auto l1 = new nodeLight;
	l1->setPos(0, 0, 3);
	g1->addLight(l1);

}

void Test1::Render() {

	//printf("Rendering entity.\n");
	//ent1->render();
	g1->render();

}