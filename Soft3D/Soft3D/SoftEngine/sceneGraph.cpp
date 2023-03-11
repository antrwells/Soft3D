#include "sceneGraph.h"
#include "rendererNitro.h"

sceneGraph::sceneGraph() {

	m_RootNode = new node;
	m_Cam = new nodeCamera;

	m_Cam->setPos(0, 0, 0);
	m_Cam->setRotation(0, 0, 0);
	m_Renderer = new renderer;

}

void sceneGraph::render() {

	auto l1 = m_Lights[0];
	m_Renderer->beginRender();
	m_RootNode->render(m_Renderer,m_Cam,l1);
	m_Renderer->endRender();

}