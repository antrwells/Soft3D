#pragma once
#include <vector>
#include "mesh3D.h"
class nodeEntity
{
public:

	void AddMesh(mesh3D* mesh);
	void render();

private:

	std::vector<mesh3D*> m_Meshes;

};

