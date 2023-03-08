#include "SoftTypes.h"
double deg2rad(double degrees) {
	return degrees * M_PI / 180.0;
}

int Triangle_ClipAgainstPlane(v3d plane_p, v3d plane_n, rtri& in_tri, rtri& out_tri1, rtri& out_tri2)
{

	// Make sure plane normal is indeed normal
	plane_n = plane_n.normalized();

	// Return signed shortest distance from point to plane, plane normal must be normalised
	auto dist = [&](v3d& p)
	{
		v3d n = p.normalized();
		return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - plane_n.dot(plane_p));
	};

	// Create two temporary storage arrays to classify points either side of plane
	// If distance sign is positive, point lies on "inside" of plane
	v3d* inside_points[3];  int nInsidePointCount = 0;
	v3d* outside_points[3]; int nOutsidePointCount = 0;
	v3d* inside_tex[3]; int nInsideTexCount = 0;
	v3d* outside_tex[3]; int nOutsideTexCount = 0;



	// Get signed distance of each point in triangle to plane
	float d0 = dist(in_tri.p0);
	float d1 = dist(in_tri.p1);
	float d2 = dist(in_tri.p2);


	if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p0; inside_tex[nInsideTexCount++] = &in_tri.t0; }
	else {
		outside_points[nOutsidePointCount++] = &in_tri.p0; outside_tex[nOutsideTexCount++] = &in_tri.t0;
	}
	if (d1 >= 0) {
		inside_points[nInsidePointCount++] = &in_tri.p1; inside_tex[nInsideTexCount++] = &in_tri.t1;
	}
	else {
		outside_points[nOutsidePointCount++] = &in_tri.p1;  outside_tex[nOutsideTexCount++] = &in_tri.t1;
	}
	if (d2 >= 0) {
		inside_points[nInsidePointCount++] = &in_tri.p2; inside_tex[nInsideTexCount++] = &in_tri.t2;
	}
	else {
		outside_points[nOutsidePointCount++] = &in_tri.p2;  outside_tex[nOutsideTexCount++] = &in_tri.t2;
	}
	// Now classify triangle points, and break the input triangle into 
	// smaller output triangles if required. There are four possible
	// outcomes...

	if (nInsidePointCount == 0)
	{
		// All points lie on the outside of plane, so clip whole triangle
		// It ceases to exist

		return 0; // No returned triangles are valid
	}

	if (nInsidePointCount == 3)
	{
		// All points lie on the inside of plane, so do nothing
		// and allow the triangle to simply pass through
		out_tri1 = in_tri;

		return 1; // Just the one returned original triangle is valid
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2)
	{
		// Triangle should be clipped. As two points lie outside
		// the plane, the triangle simply becomes a smaller triangle

		// Copy appearance info to new triangle
	//	out_tri1.col = in_tri.col;
	//	out_tri1.sym = in_tri.sym;

		// The inside point is valid, so keep that...
		// The inside point is valid, so keep that...
		//out_tri1.p[0] = *inside_points[0];
		out_tri1.t0 = *inside_tex[0];

		out_tri1.p0 = *inside_points[0];

		// but the two new points are at the locations where the 
		// original sides of the triangle (lines) intersect with the plane
		float t = 0;
		out_tri1.p1 = v3d::intersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
		out_tri1.t1.x = t * (outside_tex[0]->x - inside_tex[0]->x) + inside_tex[0]->x;
		out_tri1.t1.y = t * (outside_tex[0]->y - inside_tex[0]->y) + inside_tex[0]->y;
		out_tri1.t1.w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

		out_tri1.p2 = v3d::intersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1], t);
		out_tri1.t2.x = t * (outside_tex[1]->x - inside_tex[0]->x) + inside_tex[0]->x;
		out_tri1.t2.y = t * (outside_tex[1]->y - inside_tex[0]->y) + inside_tex[0]->y;
		out_tri1.t2.w = t * (outside_tex[1]->w - inside_tex[0]->w) + inside_tex[0]->w;
		//out_tri1.p1 = v3d::intersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
		//out_tri1.p2 = v3d::intersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1]);
		out_tri1.c0 = in_tri.c0;
		out_tri1.map = in_tri.map;

		return 1; // Return the newly formed single triangle
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1)
	{
		// Triangle should be clipped. As two points lie inside the plane,
		// the clipped triangle becomes a "quad". Fortunately, we can
		// represent a quad with two new triangles

		// Copy appearance info to new triangles
	//	out_tri1.col = in_tri.col;
		//out_tri1.sym = in_tri.sym;

	//	out_tri2.col = in_tri.col;
	//	out_tri2.sym = in_tri.sym;

		// The first triangle consists of the two inside points and a new
		// point determined by the location where one side of the triangle
		// intersects with the plane
		out_tri1.p0 = *inside_points[0];
		out_tri1.p1 = *inside_points[1];
		out_tri1.t0 = *inside_tex[0];
		out_tri1.t1 = *inside_tex[1];

		float t;
		out_tri1.p2 = v3d::intersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
		out_tri1.t2.x = t * (outside_tex[0]->x - inside_tex[0]->x) + inside_tex[0]->x;
		out_tri1.t2.y = t * (outside_tex[0]->y - inside_tex[0]->y) + inside_tex[0]->y;
		out_tri1.t2.w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

		out_tri2.p0 = *inside_points[1];
		out_tri2.t0 = *inside_tex[1];
		out_tri2.p1 = out_tri1.p2;
		out_tri2.t1 = out_tri1.t2;
		out_tri2.p2 = v3d::intersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0], t);
		out_tri2.t2.x = t * (outside_tex[0]->x - inside_tex[1]->x) + inside_tex[1]->x;
		out_tri2.t2.y = t * (outside_tex[0]->y - inside_tex[1]->y) + inside_tex[1]->y;
		out_tri2.t2.w = t * (outside_tex[0]->w - inside_tex[1]->w) + inside_tex[1]->w;

		out_tri1.map = in_tri.map;
		out_tri2.map = in_tri.map;
		//out_tri1.p0 = *inside_points[0];
		//out_tri1.p1 = *inside_points[1];
		//out_tri1.p2 = v3d::intersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);

		// The second triangle is composed of one of he inside points, a
		// new point determined by the intersection of the other side of the 
		// triangle and the plane, and the newly created point above
		
		out_tri2.c0 = in_tri.c0;
		out_tri1.c0 = in_tri.c0;
		return 2; // Return two newly formed triangles which form a quad
	}
}