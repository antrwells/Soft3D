#include "SoftApp.h"

#include "pixelMap.h"
#include "depthBuffer.h"
#include <time.h>
#include "gameInput.h"
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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
	gameInput::mouseX = 0;
	gameInput::mouseY = 0;
	gameInput::mouseDeltaX = 0;
	gameInput::mouseDeltaY = 1;

}

void SoftApp::Run() {

	m_ColorBuffer = new pixelMap(m_Width, m_Height, 3);
	m_DepthBuffer = new depthBuffer(m_Width, m_Height);

	m_ColorBuffer->fill(m_BackColor);
	//m_DepthBuffer->fill(color(0, 0, 0, 0));
	m_DepthBuffer->clear();

	Init();

	int next_fps = clock() + 1000;
	int fps = 0;
	int frames = 0;

	double xpos, ypos;
	glfwSetCursorPos(m_Window, 320, 320);
	glfwGetCursorPos(m_Window, &xpos, &ypos);
	gameInput::mouseX = xpos;
	gameInput::mouseY = ypos;

	glfwSwapInterval(0);

	while (true) {

		//double xpos, ypos;
		glfwGetCursorPos(m_Window, &xpos, &ypos);

		float lastX = gameInput::mouseX;
		float lastY = gameInput::mouseY;
		gameInput::mouseX = xpos;
		gameInput::mouseY = ypos;
		gameInput::mouseDeltaX = xpos - 320;
		gameInput::mouseDeltaY = ypos - 320;
		glfwSetCursorPos(m_Window,320,320);


		float mx, my;
		mx = my = 0;
		if (glfwGetKey(m_Window, GLFW_KEY_W))
		{
			my = 1;
		}
		else if (glfwGetKey(m_Window, GLFW_KEY_S))
		{
			my = -1;
		}
		if (glfwGetKey(m_Window, GLFW_KEY_A)) {
			mx = -1;
		}
		else if (glfwGetKey(m_Window, GLFW_KEY_D))
		{
			mx = 1;
		}
		gameInput::moveX = mx;
		gameInput::moveY = my;

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
		glClear(GL_COLOR_BUFFER_BIT);
		m_ColorBuffer->fill(m_BackColor);
		m_DepthBuffer->clear();



		Render();
		m_ColorBuffer->Display(0, 0, m_Width, m_Height);
		glfwSwapBuffers(m_Window);

		glfwPollEvents();
	}

}

pixelMap* SoftApp::getBackBuffer() {

	return m_ColorBuffer;

}

depthBuffer* SoftApp::getDepthBuffer() {

	return m_DepthBuffer;

}

SoftApp* SoftApp::m_This = nullptr;