#pragma once
#include <vector>
#include "mesh3D.h"
#include "node.h"
class nodeCamera;
class nodeLight;

class nodeEntity : public node
{
public:

	nodeEntity();
	void AddMesh(mesh3D* mesh);
	void render(renderer* render,nodeCamera* cam,nodeLight* light);
	mesh3D* getMesh(int id) {
		return m_Meshes[id];
	}
	std::vector<mesh3D*> getMeshes() {
		return m_Meshes;
	}

private:

	std::vector<mesh3D*> m_Meshes;
	

};

