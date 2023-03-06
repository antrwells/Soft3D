#include "sceneGraph.h"

sceneGraph::sceneGraph() {

	m_RootNode = new node;
	m_Cam = new nodeCamera;

	m_Cam->setPosition(0, 0, 0);
	m_Cam->setRotation(0, 0, 0);

}

void sceneGraph::render() {

	m_RootNode->render(m_Cam);

}