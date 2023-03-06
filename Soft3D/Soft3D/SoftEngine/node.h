#pragma once
#include "SoftTypes.h"
#include <vector>
class nodeCamera;
class nodeLight;

class node
{
public:
	node();
	void setRotation(float p, float y, float r);
	
	void setPos(float x, float y, float z) {
		m_Position = v3d(x, y, z);
	}

	matrix4 getRotationMat() {
		return m_Rotation;
	}
	void addNode(node* n) {
		m_Nodes.push_back(n);
		n->setRoot(this);
	}
	void setRoot(node* n)
	{
		m_Root = n;
	}
	std::vector<node*> getNodes() {
		return m_Nodes;
	}
	virtual void render(nodeCamera* cam,nodeLight* light)
	{
		for (int i = 0; i < m_Nodes.size(); i++) {
			m_Nodes[i]->render(cam,light);
		}
	}
	v3d getPos() {
		return m_Position;
	}

protected:
	matrix4 m_Rotation;
	v3d m_Position;
	std::vector<node*> m_Nodes;
	node* m_Root = nullptr;
};

