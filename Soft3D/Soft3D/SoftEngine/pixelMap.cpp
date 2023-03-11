#include "pixelMap.h"
#include "SoftTypes.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
pixelMap::pixelMap(std::string path) {

	
	m_Data = (unsigned char*)stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, 3);

	//m_Data = (unsigned char*)malloc(m_Width * m_Height * m_Channels * 4);


	if (m_Data == NULL) {
		printf("Error in loading the image\n");
		exit(1);
	}
	else {
		printf("Opened pixmap properly.\n");
	}

}

pixelMap::pixelMap(int w, int h,int channels) 
{

		m_Data = (unsigned char*)malloc(w * h * channels);
		m_Blank = (unsigned char *)malloc(w * h * channels);
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				

				int loc = y * w * channels + (x * channels);
				m_Blank[loc] = 0;
				m_Blank[loc + 1] = 0;
				m_Blank[loc + 2] = 0;

			}
		}

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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, form, GL_UNSIGNED_BYTE, (const void*)m_Data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);





}

void pixelMap::setPixel(int x, int y, color col) {
	if (y < 0 || x < 0 || x >= m_Width || y >= m_Height)
	{
		return;
	}
	if (m_Channels == 3) {

		int loc = (y * m_Width * m_Channels) + (x * m_Channels);
	
		m_Data[loc] = (unsigned char)(col.r * 255.0f);
		m_Data[loc + 1] = (unsigned char)(col.g * 255.0f);
		m_Data[loc + 2] = (unsigned char)(col.b * 255.0f);

		//m_Data[loc + 1] = (char)(col.g * 255);
		//m_Data[loc + 2] = (char)(col.b * 255);


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
		if (col.r < 0 || col.r>1)
		{
			int vv = 5;
		}
		int loc = (y * m_Width * m_Channels) + (x * m_Channels);
		char val = (char)(col.r * 255);
		m_Data[loc] = val;

	}


}

color pixelMap::getPixel(int x, int y) {

	int loc = (y * m_Width * m_Channels) + (x * m_Channels);



	if (m_Channels == 3)
	{
		
		unsigned char cr, cg, cb;

		cr = m_Data[loc];
		cg = m_Data[loc + 1];
		cb = m_Data[loc + 2];
		
		return color(((float)(cr))/255.0f,((float)(cg))/255.0f,((float)(cb))/255.0f);

	}
	else if (m_Channels == 4) {

	}
	else if (m_Channels == 1) {


	}


}

void pixelMap::fill(color col) {

	std::memcpy(m_Data, m_Blank, m_Width * m_Height * m_Channels);
	return;


	for (int y = 0; y < m_Height; y++) {
		for (int x = 0; x < m_Width; x++) {

	
			setPixel(x, y, col);

		}
	}

}

void pixelMap::Display(int x, int y, int w, int h) {


	glActiveTexture(GL_TEXTURE0);
	glClientActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_DisplayHandle);
	int form = GL_RGB;
	if (m_Channels == 4) {
		form = GL_RGBA;
	}
	if (m_Channels == 1) {
		form = GL_R;
	}
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, m_Width, m_Height, 0, form, GL_UNSIGNED_BYTE, (const void*)m_Data);
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

	//glBindTexture(GL_TEXTURE_2D, 0);
}


void pixelMap::drawPixmap(pixelMap* m, int dx, int dy, color c)
{

	for (int y = 0; y < m->getHeight(); y++) {
		
		int tl = (dy+y) * m_Width * m_Channels;
		tl = tl + dx * m_Channels;
		memcpy(m_Data + tl, (const void*)(m->getData()+(y*m->getWidth()*m->getChannels())), m->getWidth() * m->getChannels());


		//for (int x = 0; x < m->getWidth(); x++) {
	//		setPixel(dx + x, dy + y, m->getPixel(x, y));
		//}
	}

}

color pixelMap::getPixUV(float u, float v, float w) {

	float x, y;

//	u = u * (u > 1) + (u * (u < 1));

	if(u < 0 || u>1) return color(0, 0, 0);
	if (v < 0 || v>1) return color(0, 0, 0);

	x = u * (float)m_Width;
	y = v * (float)m_Height;



	return getPixel(x, y);


}