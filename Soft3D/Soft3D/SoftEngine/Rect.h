#pragma once
struct Rect
{
public:
	float x, y, width, height;
	Rect() {
		x = y = width = height = 0;
	}
	Rect(float rx, float ry, float rwidth, float rheight) {
		x = rx;
		y = ry;
		width = rwidth;
		height = rheight;
	}
};

