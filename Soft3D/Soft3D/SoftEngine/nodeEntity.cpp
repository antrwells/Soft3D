#include "nodeEntity.h"
#include "nodeCamera.h"
#include "nodeLight.h"

nodeEntity::nodeEntity() {



}

void nodeEntity::AddMesh(mesh3D* mesh)
{

	m_Meshes.push_back(mesh);

}

float ang = 0;

void nodeEntity::render(nodeCamera* cam,nodeLight* light) {

	ang = ang + 1;
	setRotation(ang, 0, 0);

	for (int i = 0; i < m_Meshes.size(); i++) {

		auto mesh = m_Meshes[i];

		mesh->render(getRotationMat(),cam,light);

	}
	for (int i = 0; i < m_Nodes.size(); i++) {
		m_Nodes[i]->render(cam,light);
	}
}