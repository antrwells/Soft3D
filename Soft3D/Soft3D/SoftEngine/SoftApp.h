#pragma once
#include <string>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include "SoftTypes.h"
class pixelMap;


class SoftApp
{
public:

	SoftApp(int width, int height, std::string app);
	virtual void Init() {};
	virtual void Update() {};
	virtual void Render() {};
	void Run();

	pixelMap* getBackBuffer();
	pixelMap* getDepthBuffer();

	static SoftApp* m_This;

private:

	GLFWwindow* m_Window;

	pixelMap* m_ColorBuffer;
	pixelMap* m_DepthBuffer;
	pixelMap* m_StencilBuffer;

	color m_BackColor;
	int m_Width, m_Height;

};

