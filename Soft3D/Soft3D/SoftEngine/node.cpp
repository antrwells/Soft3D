#include "node.h"

node::node() {

	m_Rotation = matrix4();

}

void node::setRotation(float p, float y, float r) {

	m_Rotation.toRotX(p);

}