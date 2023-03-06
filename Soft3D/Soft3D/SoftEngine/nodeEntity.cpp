#include "nodeEntity.h"


void nodeEntity::AddMesh(mesh3D* mesh)
{

	m_Meshes.push_back(mesh);

}


void nodeEntity::render() {

	for (int i = 0; i < m_Meshes.size(); i++) {

		auto mesh = m_Meshes[i];

		mesh->render();

	}

}