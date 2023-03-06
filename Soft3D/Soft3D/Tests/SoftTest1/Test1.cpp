#include "Test1.h"

#include "Importer.h"
#include "nodeEntity.h"

void Test1::Init() {

	auto imp = new Importer;

	ent1 = imp->importEntity("data/test1.fbx");


}

void Test1::Render() {

	//printf("Rendering entity.\n");
	ent1->render();

}