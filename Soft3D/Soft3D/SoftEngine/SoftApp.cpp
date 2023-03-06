#include "SoftApp.h"

#include "pixelMap.h"
#include <time.h>

SoftApp::SoftApp(int width, int height, std::string app) {

	if (!glfwInit())
	{
		// Initialization failed
		printf("Failed to create opengl context.\n");
	}
	else {
		printf("Created opengl context successfully.\n");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow* window = glfwCreateWindow(width, height,app.c_str(), NULL, NULL);
	m_Window = window;
	if (!window)
	{
		// Window or context creation failed
		printf("Failed to create glfw window.\n");
	}
	else {
		printf("Successfully created glfw window.\n");
	}
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	m_Width = width;
	m_Height = height;
	
	m_BackColor = color(1, 0, 1, 1);
	
	glMatrixMode(GL_PROJECTION_MATRIX);
	glLoadIdentity();
	glOrtho(0, m_Width, m_Height, 0, 0, 1.0f);
	m_This = this;

}

void SoftApp::Run() {

	m_ColorBuffer = new pixelMap(m_Width, m_Height, 3);
	m_DepthBuffer = new pixelMap(m_Width, m_Height, 1);

	m_ColorBuffer->fill(m_BackColor);
	m_DepthBuffer->fill(color(0, 0, 0, 0));

	Init();

	int next_fps = clock() + 1000;
	int fps = 0;
	int frames = 0;
	while (true) {

		Update();
		int time = clock();
		if (time > next_fps)
		{
			fps = frames;
			frames = 0;
			next_fps = time + 1000;
			printf("FPS:%d\n", fps);
		}
		frames++;

		glClearColor(0.3, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_ColorBuffer->fill(m_BackColor);
		m_DepthBuffer->fill(color(0, 0, 0, 0));

		Render();
		m_ColorBuffer->Display(0, 0, m_Width, m_Height);
		glfwSwapBuffers(m_Window);

		glfwPollEvents();
	}

}

pixelMap* SoftApp::getBackBuffer() {

	return m_ColorBuffer;

}

pixelMap* SoftApp::getDepthBuffer() {

	return m_DepthBuffer;

}

SoftApp* SoftApp::m_This = nullptr;