#pragma once
#include "SoftTypes.h"
#include "nodeCamera.h"
#include <vector>

class sceneGraph
{
public:

	sceneGraph();
	void addNode(node* n) {
		m_RootNode->addNode(n);
	}
	void render();

private:

	nodeCamera* m_Cam;
	node* m_RootNode;

};

