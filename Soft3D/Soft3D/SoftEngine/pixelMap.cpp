#include "pixelMap.h"
#include "SoftTypes.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
pixelMap::pixelMap(std::string path) {

	
	auto np  = (unsigned char*)stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, 3);

	m_Data = (float*)malloc(m_Width * m_Height * m_Channels * 4);

	for (int y = 0; y < m_Height; y++) {
		for (int x = 0; x < m_Width; x++) {

			int loc = y * m_Width * m_Channels;
			loc = loc + (x * m_Channels);
			float r, g, b;
			r = (float)np[loc];
			g = (float)np[loc + 1];
			b = (float)np[loc + 2];

			//r = r + 128;
			//g = g + 128;
			//b = b + 128;



			float fr, fg, fb;

			fr = (float)(r) / 255.0f;
			fg = (float)(g) / 255.0f;
			fb = (float)(b) / 255.0f;

			if (fr < 0 || fr>1 || fg < 0 || fg>1 || fb < 0 || fb>1)
			{
				int aa = 5;
			}

			
			m_Data[loc] = fr;
			m_Data[loc + 1] = fg;
			m_Data[loc + 2] = fb;

			int aa = 5;

		}
	}

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

		m_Data = (float*)malloc(w * h * channels*4);

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

		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB32F, m_Width, m_Height, 0, form,GL_FLOAT,(const void*)m_Data);
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
	
		m_Data[loc] = col.r;
		m_Data[loc + 1] = col.g;
		m_Data[loc + 2] = col.b;

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

	color r;

	if (m_Channels == 3)
	{
		
		float cr, cg, cb;

		cr = m_Data[loc];
		cg = m_Data[loc + 1];
		cb = m_Data[loc + 2];
		if (cr < 0 || cr>1)
		{
			int aa = 5;
		}
		return color(cr, cg, cb);

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_Width, m_Height, 0, form, GL_FLOAT, (const void*)m_Data);
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

	x = u * (float)m_Width;
	y = v * (float)m_Height;

	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x >= m_Width) x = m_Width - 1;
	if (y >= m_Height) y = m_Height - 1;

	auto mm = getPixel(x, y);

	//return getPixel(x, y);
	return mm;

}