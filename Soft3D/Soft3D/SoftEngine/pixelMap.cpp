#include "pixelMap.h"
#include "SoftTypes.h"


pixelMap::pixelMap(int w, int h,int channels) 
{

		m_Data = (char*)malloc(w * h * channels);

		m_Channels = channels;

		m_Width = w;
		m_Height = h;
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &m_DisplayHandle);
		glBindTexture(GL_TEXTURE_2D, m_DisplayHandle);
		
		int form = GL_RGB;
		if (m_Channels == 4) {
			form = GL_RGBA;
		}
		if (m_Channels == 1) {
			form = GL_R;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, form, m_Width, m_Height, 0, form, GL_UNSIGNED_BYTE,(const void*)m_Data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D, 0);





}

void pixelMap::setPixel(int x, int y, color col) {

	if (m_Channels == 3) {

		int loc = (y * m_Width *m_Channels )+ (x * m_Channels);

		m_Data[loc] = (char)(col.r * 255);
		m_Data[loc + 1] = (char)(col.g * 255);
		m_Data[loc + 2] = (char)(col.b * 255);

	}
	else if (m_Channels == 4)
	{
		int loc = (y * m_Width * m_Channels) + (x * m_Channels);

		m_Data[loc] = (char)(col.r * 255);
		m_Data[loc + 1] = (char)(col.g * 255);
		m_Data[loc + 2] = (char)(col.b * 255);
		m_Data[loc + 3] = (char)(col.a * 255);
	}
	else if (m_Channels == 1)
	{
		int loc = (y * m_Width * m_Channels) + (x * m_Channels);
		m_Data[loc] = (char)(col.r * 255);
	}


}

color pixelMap::getPixel(int x, int y) {

	int loc = (y * m_Width * m_Channels) + (x * m_Channels);
	color r;

	if (m_Channels == 3)
	{
		r.r = ((float)m_Data[loc]) / 255.0f;
		r.g = ((float)m_Data[loc + 1]) / 255.0f;
		r.b = ((float)m_Data[loc + 2]) / 255.0f;
	}
	else if (m_Channels == 4) {
		r.r = ((float)m_Data[loc]) / 255.0f;
		r.g = ((float)m_Data[loc + 1]) / 255.0f;
		r.b = ((float)m_Data[loc + 2]) / 255.0f;
		r.a = ((float)m_Data[loc + 3]) / 255.0f;
	}
	else if (m_Channels == 1) {

		r.r = ((float)m_Data[loc]) / 255.0f;

	}

	return r;

}

void pixelMap::fill(color col) {

	for (int y = 0; y < m_Height; y++) {
		for (int x = 0; x < m_Width; x++) {

			setPixel(x, y, col);

		}
	}

}

void pixelMap::Display(int x, int y, int w, int h) {


	glBindTexture(GL_TEXTURE_2D, m_DisplayHandle);
	int form = GL_RGB;
	if (m_Channels == 4) {
		form = GL_RGBA;
	}
	if (m_Channels == 1) {
		form = GL_R;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, form, m_Width, m_Height, 0, form, GL_UNSIGNED_BYTE, (const void*)m_Data);

	glColor4f(1, 1, 1, 1);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2f(x, y);
	glTexCoord2f(1, 0);
	glVertex2f(x + w, y);
	glTexCoord2f(1, 1);
	glVertex2f(x + w, y + h);
	glTexCoord2f(0, 1);
	glVertex2f(x, y + h);


	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}