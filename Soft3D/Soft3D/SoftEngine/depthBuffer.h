#pragma once
#include <memory>
class depthBuffer
{
public:

	depthBuffer(int w, int h) {
		db = (float*)malloc(w * h * 4);
		cb = (float*)malloc(w * h * 4);
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				cb[y * w + x] = 0;
			}
		}
		m_Width = w;
		m_Height = h;
	}

	float getDepth(int x, int y) {

		return db[y * m_Width + x];

	}

	void setDepth(int x, int y, float d)
	{
		db[y * m_Width + x] = d;
	}

	void clear() {
		memcpy(db, cb, m_Width * m_Height * 4);
		return;
		for (int y = 0; y < m_Height; y++) {
			for (int x = 0; x < m_Width; x++) {
				db[y * m_Width + x] = 0.0f;
			}
		}
	}
	float* getData() {
		return db;
	}
private:
	int m_Width, m_Height;
	float* db;
	float* cb;

};

