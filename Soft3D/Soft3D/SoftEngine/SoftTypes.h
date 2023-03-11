#pragma once
#include <math.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
double deg2rad(double degrees);

class pixelMap;

struct v3d
{
	float x, y, z, w=1.0f;
	v3d() {
		x = y = z = 0.0f;
		w = 1.0;
	}
	v3d(float p_x, float p_y, float p_z)
	{
		x = p_x;
		y = p_y;
		z = p_z;
		w = 1.0f;
	}

	v3d add(v3d r) {

		return v3d(x + r.x, y + r.y, z + r.z);

	}

	v3d mult(v3d r) {

		return v3d(x * r.x, y * r.y, z * r.z);

	}

	static v3d intersectPlane(v3d plane_p, v3d plane_n, v3d line_start, v3d line_end,float &t)
	{

		plane_n = plane_n.normalized();
		float plane_d = -plane_n.dot(plane_p);
		float ad = line_start.dot(plane_n);
		float bd = line_end.dot(plane_n);
		t = (-plane_d - ad) / (bd - ad);
		v3d lineStartToEnd = line_end.minus(line_start);
		v3d lineToIntersect = lineStartToEnd.multi(t);
		return line_start.add(lineToIntersect);

	}

	//static 

	v3d div(v3d r) {

		return v3d(x / r.x, y / r.y, z / r.z);

	}

	v3d div(float d)
	{
		return v3d(x / d, y / d, z / d);
	}

	v3d minus(v3d r) {
		
		return v3d(x - r.x, y - r.y, z - r.z);

	}

	void normalize() {

		float l = sqrtf(x * x + y * y + z * z);
		x /= l;
		y /= l;
		z /= l;


	}

	v3d normalized() {

		v3d norm = v3d(x, y, z);
		norm.normalize();
		return norm;

	}

	float length()
	{
		return sqrtf(dot(*this));
	}

	float dot(v3d r) {

		return x * r.x + y * r.y + z * r.z;

	}

	v3d multi(float v)
	{
		return v3d(x * v, y * v, z * v);

	}

	v3d cross(v3d r) {

		v3d l;
		l.x = y * r.z - z * r.y;
		l.y = z * r.x - x * r.z;
		l.z = x * r.y - y * r.x;
		return l;

	}

};;

struct vertex {

	v3d pos;
	v3d world_pos;
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

	color mult(float v)
	{
		return color(r * v, g * v, b * v, a * v);
	}
	color mult(color col) {

		return color(r * col.r, g * col.g, b * col.b, a * col.a);

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

	void toRotY(float ang) {

		ang = deg2rad(ang);
		m[0][0] = cosf(ang);
		m[0][2] = sinf(ang);
		m[2][0] = -sinf(ang);
		m[1][1] = 1.0f;
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

	void toTrans(float x, float y, float z)
	{

		m[0][0] = 1.0f;
		m[1][1] = 1.0f;
		m[2][2] = 1.0f;
		m[3][3] = 1.0f;
		m[3][0] = x;
		m[3][1] = y;
		m[3][2] = z;

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

	matrix4 multi(matrix4 m2)
	{
		matrix4 rm;
		for (int c = 0; c < 4; c++) {
			for (int r = 0; r < 4; r++) {
				rm.m[r][c] = m[r][0] * m2.m[0][c] + m[r][1] * m2.m[1][c] + m[r][2] * m2.m[2][c] + m[r][3] * m2.m[3][c];
				//rm.m[r][c] = m[r][0] * m

			}
		}
		return rm;
	
	}

	v3d multiply(v3d& v)
	{

		v3d r;

		r.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + v.w * m[3][0];
		r.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + v.w * m[3][1];
		r.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + v.w * m[3][2];
		r.w = v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + v.w * m[3][3];

		return r;

	}

	void toIdent() {
		m[0][0] = 1.0f;
		m[1][1] = 1.0f;
		m[2][2] = 1.0f;
		m[3][3] = 1.0f;
	}

	void toPointAt(v3d pos, v3d target, v3d up)
	{
		v3d newForward = target.minus(pos);
		newForward = newForward.normalized();

		v3d a = newForward.multi(up.dot(newForward));

		v3d newUp = up.minus(a);
		newUp = newUp.normalized();

		v3d newRight = newUp.cross(newForward);

		m[0][0] = newRight.x;	m[0][1] = newRight.y;	m[0][2] = newRight.z;	m[0][3] = 0.0f;
		m[1][0] = newUp.x;		m[1][1] = newUp.y;		m[1][2] = newUp.z;		m[1][3] = 0.0f;
		m[2][0] = newForward.x;	m[2][1] = newForward.y;	m[2][2] = newForward.z;	m[2][3] = 0.0f;
		m[3][0] = pos.x;	    m[3][1] = pos.y;		m[3][2] = pos.z;	    m[3][3] = 1.0f;



	}

	matrix4 inverse() {

		matrix4 m = *this;
		matrix4 matrix;
		matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
		matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
		matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
		matrix.m[3][3] = 1.0f;
		return matrix;

	}

};

struct rtri
{
	v3d p0, p1, p2;
	v3d t0, t1, t2;
	vertex v0, v1, v2;
	color c0, c1, c2;
	pixelMap* map;
	matrix4 world, view;
};

int Triangle_ClipAgainstPlane(v3d plane_p, v3d plane_n, rtri& in_tri, rtri& out_tri1, rtri& out_tri2);



class SoftTypes
{
};

