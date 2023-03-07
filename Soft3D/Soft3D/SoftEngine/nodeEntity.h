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
	void render(nodeCamera* cam,nodeLight* light);
	mesh3D* getMesh(int id) {
		return m_Meshes[id];
	}

private:

	std::vector<mesh3D*> m_Meshes;
	

};

