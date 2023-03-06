#pragma once
#include <math.h>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
double deg2rad(double degrees);

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

struct color {

	float r, g, b, a;
	color() {
		r = g = b = 0.0f;
		a = 1.0f;
	}
	color(float p_r, float p_g, float p_b) {
	
		r = p_r;
		g = p_g;
		b = p_b;
		a = 1.0f;

	}
	color(float p_r, float p_g, float p_b, float p_a)
	{
		r = p_r;
		g = p_g;
		b = p_b;
		a = p_a;
	}

};

struct matrix4 {

	float m[4][4] = { 0 };
	matrix4() {

	}

	void toRotX(float ang) {

		ang = deg2rad(ang);
		m[0][0] = 1.0f;
		m[1][1] = cosf(ang);
		m[1][2] = sinf(ang);
		m[2][1] = -sinf(ang);
		m[2][2] = cosf(ang);
		m[3][3] = 1.0f;

	}

	void toRotZ(float ang) {

		ang = deg2rad(ang);
		m[0][0] = cosf(ang);
		m[0][1] = sinf(ang);
		m[1][0] = -sinf(ang);
		m[1][1] = cosf(ang);
		m[2][2] = 1;
		m[3][3] = 1;
	}

	void toProjection(float fov,float aspect,float znear, float zfar) {

		float fovRad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.14159f);

		m[0][0] = aspect * fovRad;
		m[1][1] = fovRad;
		m[2][2] = zfar / (zfar - znear);
		m[3][2] = (-zfar * znear) / (zfar - znear);
		m[2][3] = 1.0f;
		m[3][3] = 0.0f;

	}

	v3d multiply(v3d& v)
	{

		v3d r;

		r.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0];
		r.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1];
		r.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2];

		float w = v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + m[3][3];

		if (w != 0.0f)
		{
			r.x /= w;
			r.y /= w;
			r.z /= w;
		}

		return r;

	}

};

class SoftTypes
{
};

