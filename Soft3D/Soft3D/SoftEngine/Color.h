#pragma once
struct Color
{
public:
	float r, g, b, a;
	Color() {
		r = g = b = 0;
		a = 1.0;
	}
	Color(float vr, float vg, float vb, float va) {
		r = vr;
		g = vg;
		b = vb;
		a = va;
	}
};

