#include "sceneGraph.h"

sceneGraph::sceneGraph() {

	m_RootNode = new node;
	m_Cam = new nodeCamera;

	m_Cam->setPos(0, 0, 0);
	m_Cam->setRotation(0, 0, 0);

}

void sceneGraph::render() {

	auto l1 = m_Lights[0];

	m_RootNode->render(m_Cam,l1);

}