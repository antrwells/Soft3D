#pragma once
#include <vector>
#include "mesh3D.h"
#include "node.h"
class nodeCamera;

class nodeEntity : public node
{
public:

	nodeEntity();
	void AddMesh(mesh3D* mesh);
	void render(nodeCamera* cam);

private:

	std::vector<mesh3D*> m_Meshes;
	

};

