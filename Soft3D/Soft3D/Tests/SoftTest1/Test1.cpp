#include "Test1.h"

#include "Importer.h"
#include "sceneGraph.h"
#include "nodeEntity.h"
#include "gameInput.h"
#include "pixelMap.h"
#include "AudioSystem.h"
#include "QuickDraw.h"
#include "Texture2D.h"
#include "ASTitleScreen.h"
void Test1::Init() {

	InitRenderer(320,200);

	PushState(new ASTitleScreen);

	
}
float ang2 = 0;

float camX=0, camY=0;


void Test1::Update(float et) {

	

}

void Test1::Render() {

	


}

void Test1::RenderAfter3D() {
	return;
	
}