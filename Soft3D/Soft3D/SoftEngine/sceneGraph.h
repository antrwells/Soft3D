#pragma once
#include "SoftTypes.h"
#include "nodeCamera.h"
#include "nodeLight.h"
#include <vector>
#include "renderer.h"
#include "rendererNitro.h"

class sceneGraph
{
public:

	sceneGraph();
	void addNode(node* n) {
		m_RootNode->addNode(n);
	}
	void render();
	nodeCamera* getCam() {
		return m_Cam;
	}
	void addLight(nodeLight* light) {
		m_Lights.push_back(light);
	}
	std::vector<nodeLight*> getLights() {
		return m_Lights;
	}

private:

	nodeCamera* m_Cam;
	node* m_RootNode;
	std::vector<nodeLight*> m_Lights;
	renderer* m_Renderer = nullptr;

};

