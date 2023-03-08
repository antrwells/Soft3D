#include "SoftApp.h"

#include "pixelMap.h"
#include "depthBuffer.h"
#include <time.h>
#include "gameInput.h"
#include "AudioSystem.h"

SoftApp::SoftApp(int width, int height, std::string app) {

	if (!glfwInit())
	{
		// Initialization failed
		printf("Failed to create opengl context.\n");
	}
	else {
		printf("Created opengl context successfully.\n");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	//glfwSetWindowMonitor(window, monitor, 0, 0, width, height, 60);


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
	win_Width = width;
	win_Height = height;

}
double lastTime = glfwGetTime(); // initialize the last time to the current time

double getDeltaTime() {
	double currentTime = glfwGetTime();
	double deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	return deltaTime;
}

void SoftApp::Run() {

	m_Audio = new AudioSystem;
	Init();
	m_ColorBuffer->fill(m_BackColor);
	//m_DepthBuffer->fill(color(0, 0, 0, 0));
	m_DepthBuffer->clear();





	int next_fps = clock() + 1000;
	int fps = 0;
	int frames = 0;

	double xpos, ypos;
	glfwSetCursorPos(m_Window, 320, 320);
	glfwGetCursorPos(m_Window, &xpos, &ypos);
	gameInput::mouseX = xpos;
	gameInput::mouseY = ypos;

	glfwSwapInterval(0);

	int pframe = clock();

	const double UPS = 60.0; // desired updates per second
	const double TIME_BETWEEN_UPDATES = 1.0 / UPS; // time between updates in seconds
	double accumulator = 0.0; // time accumulated since the last update


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


		//int et = clock() - pframe;
		//printf("et:%d\n", et);
		//Update(et);
		double deltaTime = getDeltaTime(); // get the time since the last frame
		accumulator += deltaTime;
		gameInput::anyKeyPressed = false;

		for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; key++) {
			if (glfwGetKey(m_Window, key) == GLFW_PRESS) {
				// anyKeyPressed = true;
				gameInput::anyKeyPressed = true;
				break;
			}
		}

		// update the game logic in fixed time steps
		while (accumulator >= TIME_BETWEEN_UPDATES) {
			Update(0); // update the game logic
			if (m_States.size() > 0) {

				auto ts = m_States.top();
				ts->Update();

			}
			accumulator -= TIME_BETWEEN_UPDATES;
		}

		int time = clock();
		if (time > next_fps)
		{
			fps = frames;
			frames = 0;
			next_fps = time + 1000;
			printf("FPS:%d\n", fps);
		}
		frames++;

		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		m_ColorBuffer->fill(m_BackColor);
		m_DepthBuffer->clear();



		Render();
		if (m_States.size() > 0) {

			auto ts = m_States.top();
			ts->Render();

		}
		m_ColorBuffer->Display(0, 0, win_Width, win_Height);
		glClear(GL_DEPTH_BUFFER_BIT);

		RenderAfter3D();
		if (m_States.size() > 0) {

			auto ts = m_States.top();
			ts->RenderAfter3D();

		}

		glfwSwapBuffers(m_Window);

		glfwPollEvents();
	}

}

void SoftApp::InitRenderer(int w, int h) {

	m_ColorBuffer = new pixelMap(w, h, 3);
	m_DepthBuffer = new depthBuffer(w,h);
	m_Width = w;
	m_Height = h;

}

pixelMap* SoftApp::getBackBuffer() {

	return m_ColorBuffer;

}

depthBuffer* SoftApp::getDepthBuffer() {

	return m_DepthBuffer;

}

SoftApp* SoftApp::m_This = nullptr;