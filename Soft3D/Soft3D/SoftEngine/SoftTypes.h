#pragma once

struct v3d
{
	float x, y, z;
	v3d() {
		x = y = z = 0.0f;
	}
	v3d(float p_x, float p_y, float p_z)
	{
		x = p_x;
		y = p_y;
		z = p_z;
	}
};;

struct vertex {

	v3d pos;
	v3d tex_coord;
	v3d color;
	v3d normal;
	vertex() {
		pos = v3d(0, 0, 0);
		tex_coord = v3d(0, 0, 0);
		color = v3d(1, 1, 1);
		normal = v3d(0, 1, 0);
	}
};

struct triangle {

	int v0, v1, v2;
	triangle() {
		v0 = v1 = v2 = 0;
	}

};

class SoftTypes
{
};

