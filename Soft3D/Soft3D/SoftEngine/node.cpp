#include "node.h"

node::node() {

	m_Rotation = matrix4();

}

void node::setRotation(float p, float y, float r) {

	matrix4 pm, ym, rm;

	pm.toRotX(p);
	ym.toRotY(y);
	rm.toRotZ(r);

	matrix4 rot;
	rot.toIdent();

	rot = rot.multi(ym);
	rot = rot.multi(pm);

	m_Rotation = rot;



}